from flask import current_app
from flask.views import MethodView
from flask_smorest import abort
from flask_jwt_extended import get_jwt_identity
from sqlalchemy.exc import IntegrityError, SQLAlchemyError

from app import db
from app.schemas import ProfileSchema, ProfileNamesListSchema, ErrorSchema
from app.models import UserModel, ProfileModel, LayerModel, KeyModel, OptionModel
from app.routes.doc_blueprint import DocBlueprint
from .common_messages import COMMON_MESSAGES

bp = DocBlueprint("profiles", __name__, description="Profile management related endpoints")

MESSAGES = {
    "profile_created": "Profile created successfully.",
    "profile_updated": "Profile updated.",
    "already_exists": "Profile with the same name already exist for the user.",
    "profile_create_failure": "Error while creating the profile.",
    "profile_update_failure": "Error while updating the profile.",
    "delete_success": "Profile deleted.",
    "delete_failure": "Error while deleting the profile.",
    "profile_not_found": "Profile not found.",
}


@bp.route("/me/profile-names")
class ProfileNamesList(MethodView):
    @bp.response(200, ProfileNamesListSchema(many=True))
    @bp.jwt_required_with_doc()
    def get(self):
        user = UserModel.query.get_or_404(int(get_jwt_identity()), description=COMMON_MESSAGES["user_not_found"])
        return db.session.scalars(user.profiles.select()).all()


@bp.route("/me/profiles")
class Profile(MethodView):
    @bp.response(200, ProfileSchema(many=True))
    @bp.jwt_required_with_doc()
    def get(self):
        user = UserModel.query.get_or_404(int(get_jwt_identity()), description=COMMON_MESSAGES["user_not_found"])
        return db.session.scalars(user.profiles.select()).all()

    @bp.arguments(ProfileSchema)
    @bp.response(201, ProfileSchema, description=MESSAGES["profile_created"])
    @bp.alt_response(409, description=MESSAGES["already_exists"])
    @bp.alt_response(422, description=COMMON_MESSAGES["invalid_request"])
    @bp.alt_response(500, description=MESSAGES["profile_create_failure"], schema=ErrorSchema)
    @bp.jwt_required_with_doc()
    def post(self, profile_data):
        user_id = int(get_jwt_identity())
        if ProfileModel.query.filter(ProfileModel.user_id == user_id, ProfileModel.name == profile_data["name"]).first():
            abort(409, message=MESSAGES["already_exists"])

        profile = ProfileModel(user_id=user_id, name=profile_data["name"])
        self.populate_profile_with_data(profile=profile, profile_data=profile_data)

        try:
            db.session.add(profile)
            db.session.commit()
        except (IntegrityError, SQLAlchemyError) as e:
            current_app.logger.error(f"Error while creating profile: '{e}'")
            abort(500, message=MESSAGES["profile_create_failure"])

        return profile

    @staticmethod
    def populate_profile_with_data(profile: ProfileModel, profile_data: ProfileSchema):
        # will have to look more into creating models in-place with multiple levels of o-to-m nested relationships
        layers = profile_data.get("layers", [])
        for layer_data in layers:
            layer = LayerModel(profile_id = profile.id, index=layer_data["index"], color=layer_data["color"])
            keys = layer_data.get("keys", [])
            for key_data in keys:
                key = KeyModel(layer_id=layer.id, index=key_data["index"], action=key_data["action"])
                options = key_data.get("options", [])
                for option_data in options:
                    option = OptionModel(key_id=key.id, name=option_data["name"], value=option_data["value"])
                    key.options.append(option)
                layer.keys.append(key)
            profile.layers.append(layer)


@bp.route("/me/profile/<int:profile_id>")
class ProfileById(MethodView):
    @bp.response(200, ProfileSchema)
    @bp.alt_response(404, description=MESSAGES["profile_not_found"], schema=ErrorSchema)
    @bp.jwt_required_with_doc()
    def get(self, profile_id):
        profile = self.get_profile_from_user_or_abort(profile_id)
        return profile

    @bp.arguments(ProfileSchema)
    @bp.response(200, description=MESSAGES["profile_updated"])
    @bp.response(201, ProfileSchema, description=MESSAGES["profile_created"])
    @bp.alt_response(422, description=COMMON_MESSAGES["invalid_request"])
    @bp.alt_response(500, description=MESSAGES["profile_update_failure"], schema=ErrorSchema)
    @bp.jwt_required_with_doc()
    def put(self, profile_data, profile_id):
        profile = self.get_profile_from_user(profile_id)

        if profile:
            current_app.logger.debug(f"profile {profile!r} exists, updating it")
            # not feeling like trying to pin point the exact ones that are already present or not
            # simply removing all of them and them re-applying the entire data
            # I might look for a more granular implementation for PATCH (that is IF I will get to implement it)
            profile.layers.clear()
        else:
            current_app.logger.debug(f"profile does not exist, creating it")
            profile = ProfileModel(user_id=int(get_jwt_identity()), name=profile_data["name"])

        Profile.populate_profile_with_data(profile=profile, profile_data=profile_data)

        try:
            db.session.add(profile)
            db.session.commit()
        except (IntegrityError, SQLAlchemyError) as e:
            current_app.logger.error(f"Error while updating profile {profile!r}: '{e}'")
            abort(500, message=MESSAGES["profile_update_failure"])

        return profile

    @bp.response(204, description=MESSAGES["delete_success"])
    @bp.alt_response(404, description=MESSAGES["profile_not_found"])
    @bp.alt_response(500, description=MESSAGES["delete_failure"], schema=ErrorSchema)
    @bp.jwt_required_with_doc()
    def delete(self, profile_id):
        profile = self.get_profile_from_user_or_abort(profile_id)
        try:
            db.session.delete(profile)
            db.session.commit()
        except (IntegrityError, SQLAlchemyError) as e:
            current_app.logger.error(f"Error deleting profile {profile!r}: '{e}'")
            abort(500, message=MESSAGES["delete_failure"])

    @staticmethod
    def get_profile_from_user(profile_id: str):
        # maybe a get_or_404 on ProfileModel should work just fine for this, after all id is unique across the entire table
        # but I'll go the long(er) way and search only in the user-profiles relationship
        user = UserModel.query.get_or_404(int(get_jwt_identity()), description=COMMON_MESSAGES["user_not_found"])
        return db.session.scalars(user.profiles.select().where(ProfileModel.id == profile_id)).first()

    @staticmethod
    def get_profile_from_user_or_abort(profile_id: str):
        profile = ProfileById.get_profile_from_user(profile_id)
        if not profile:
            abort(404, message=MESSAGES["profile_not_found"])

        return profile
