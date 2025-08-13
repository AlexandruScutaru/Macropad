from flask import current_app
from flask.views import MethodView
from flask_smorest import abort
from flask_jwt_extended import get_jwt_identity
from sqlalchemy.exc import SQLAlchemyError

from app import db
from app.schemas import ProfileSchema, ErrorSchema
from app.models import UserModel, ProfileModel, LayerModel, KeyModel, OptionModel
from app.routes.doc_blueprint import DocBlueprint
from .common_messages import COMMON_MESSAGES

bp = DocBlueprint("profiles", __name__, description="Profile management related endpoints")

MESSAGES = {
    "profile_not_found": "Profile not found.",
    "profile_created": "Profile created successfully.",
    "already_exists": "Profile with the same name already exist for the user.",
    "profile_create_failure": "Error while creating the profile.",
}


@bp.route("/me/profile")
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

        # will have to look more into creating models in-place with multiple levels of o-to-m nested relationships
        profile = ProfileModel(user_id=user_id, name=profile_data["name"])

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

        try:
            db.session.add(profile)
            db.session.commit()
        except SQLAlchemyError:
            abort(500, message=MESSAGES["profile_create_failure"])

        return profile


@bp.route("/me/profile/<int:profile_id>")
class ProfileById(MethodView):
    @bp.response(200, ProfileSchema)
    @bp.alt_response(404, description=MESSAGES["profile_not_found"], schema=ErrorSchema)
    @bp.jwt_required_with_doc()
    def get(self, profile_id):
        # maybe a get_or_404 should work just fine for this, after all id are unique across the entire table
        # but I'll go the long way and search only in the user-profiles relationship
        user = UserModel.query.get_or_404(int(get_jwt_identity()), description=COMMON_MESSAGES["user_not_found"])
        profile = db.session.scalars(user.profiles.select().where(ProfileModel.id == profile_id)).first()
        if not profile:
            abort(404, message=MESSAGES["profile_not_found"])

        return profile
