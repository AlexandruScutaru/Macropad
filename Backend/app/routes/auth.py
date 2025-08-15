from flask import current_app, request
from flask.views import MethodView
from flask_smorest import abort
from flask_jwt_extended import create_access_token, create_refresh_token, get_jwt_identity, get_jwt
from passlib.hash import pbkdf2_sha256
from sqlalchemy.exc import SQLAlchemyError, IntegrityError

from app import db
from app.schemas import UserSchema, TokensSchema, ErrorSchema
from app.models import UserModel, JtiModel
from app.routes.doc_blueprint import DocBlueprint
from .common_messages import COMMON_MESSAGES

bp = DocBlueprint("user_auth", __name__, description="User and authentication related endpoints")

MESSAGES = {
    "register_success": "User successfully registered.",
    "register_failure": "Error while registering the user.",
    "already_exists": "User already exists.",
    "delete_success": "User deleted.",
    "delete_failure": "Error while deleting the user.",
    "login_success": "User logged in successfully.",
    "logout_success": "User logged out successfully.",
    "refresh_success": "Access token refreshed.",
    "invalid_credentials": "Invalid credentials.",
}


@bp.route("/register")
class UserRegister(MethodView):
    @bp.arguments(UserSchema)
    @bp.response(201, UserSchema, description=MESSAGES["register_success"])
    @bp.alt_response(409, description=MESSAGES["already_exists"])
    @bp.alt_response(422, description=COMMON_MESSAGES["invalid_request"])
    @bp.alt_response(500, description=MESSAGES["register_failure"], schema=ErrorSchema)
    def post(self, user_data):
        user = UserModel(
            email=user_data["email"],
            username=user_data["username"],
            password_hash=pbkdf2_sha256.hash((user_data["password"]))
        )

        try:
            db.session.add(user)
            db.session.commit()
        except IntegrityError:
            # is this considered a security risk to give away such info?
            abort(409, message=MESSAGES["already_exists"])
        except SQLAlchemyError:
            abort(500, message=MESSAGES["register_failure"])

        return user


@bp.route("/user/me")
class User(MethodView):
    @bp.response(200, UserSchema)
    # not sure how this can happen, maybe some sync issues could leave the app in such state?
    @bp.alt_response(404, description=COMMON_MESSAGES["user_not_found"], schema=ErrorSchema)
    @bp.jwt_required_with_doc()
    def get(self):
        user = UserModel.query.get_or_404(int(get_jwt_identity()), description=COMMON_MESSAGES["user_not_found"])
        return user

    @bp.response(204, description=MESSAGES["delete_success"])
    @bp.alt_response(404, description=COMMON_MESSAGES["user_not_found"], schema=ErrorSchema)
    @bp.jwt_required_with_doc()
    def delete(self):
        UserLogout.do_logout(get_jwt()["jti"])

        user = UserModel.query.get_or_404(int(get_jwt_identity()), description=COMMON_MESSAGES["user_not_found"])
        try:
            db.session.delete(user)
            db.session.commit()
        except SQLAlchemyError as e:
            current_app.logger.error(f"Error while deleting user: {e}")
            abort(500, message=MESSAGES["delete_failure"])

        return None


@bp.route("/login")
class UserLogin(MethodView):
    @bp.arguments(UserSchema)
    @bp.response(200, TokensSchema, description=MESSAGES["login_success"])
    @bp.alt_response(401, description=MESSAGES["invalid_credentials"])
    @bp.alt_response(422, description=COMMON_MESSAGES["invalid_request"], schema=ErrorSchema)
    def post(self, user_data):
        user = UserModel.query.filter(
            (UserModel.email == user_data.get("email", "")) | (UserModel.username == user_data.get("username", ""))
            # this can also be written as or_(UserModel.email == user_data.get("email", ""), UserModel.username == user_data.get("username", ""))
        ).first()

        if user and pbkdf2_sha256.verify(user_data["password"], user.password_hash):
            access = create_access_token(identity=str(user.id), fresh=True)
            refresh = create_refresh_token(identity=str(user.id))
            return {"access_token": access, "refresh_token": refresh}

        abort(401, message=MESSAGES["invalid_credentials"])


@bp.route("/logout")
class UserLogout(MethodView):
    @bp.response(204, description=MESSAGES["logout_success"])
    @bp.jwt_required_with_doc()
    def post(self):
        self.do_logout(get_jwt()["jti"])
        return None

    @staticmethod
    def do_logout(jti_value: str):
        current_app.logger.debug("Logout user")
        jti = JtiModel(jti=jti_value)

        # look at redis for this kind of thing
        try:
            db.session.add(jti)
            db.session.commit()
        except IntegrityError:
            current_app.logger.warning(f"jti `{jti_value}` already existing in the db")
        except SQLAlchemyError:
            current_app.logger.warning(f"error inserting jti `{jti_value}` into the db")
        # will look into properly handling error responses for these cases


@bp.route("/refresh")
class TokenRefresh(MethodView):
    @bp.response(200, TokensSchema, description=MESSAGES["refresh_success"])
    @bp.jwt_required_with_doc(refresh=True)
    def post(self):
        current_user = get_jwt_identity()
        new_token = create_access_token(identity=current_user, fresh=False)
        authorization_header = request.headers.get("Authorization")
        refresh_token = authorization_header.split()[1]
        return {"access_token": new_token, "refresh_token": refresh_token}

