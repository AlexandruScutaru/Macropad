from flask_smorest import Api

from app.routes.auth import bp as AuthBlueprint
from app.routes.profile_management import bp as ProfilesBlueprint


def register_blueprints(api: Api):
    api.DEFAULT_ERROR_RESPONSE_NAME = None

    api.register_blueprint(AuthBlueprint)
    api.register_blueprint(ProfilesBlueprint)
