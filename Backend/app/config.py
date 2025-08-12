import os
from dotenv import load_dotenv

ROOT_DIR = os.path.abspath(os.path.dirname(__file__))
load_dotenv(os.path.join(ROOT_DIR, ".env"))


class Config:
    PROPAGATE_EXCEPTIONS = True

    API_TITLE = "Macropad Profiles REST API"
    API_VERSION = "v1"
    OPENAPI_VERSION = "3.0.3"
    OPENAPI_URL_PREFIX = "/"
    OPENAPI_SWAGGER_UI_PATH = "/swagger-ui"
    OPENAPI_SWAGGER_UI_URL = "https://cdn.jsdelivr.net/npm/swagger-ui-dist/"
    API_SPEC_OPTIONS = {
        "components": {
            "securitySchemes": {
                "Bearer Auth": {
                    "name": "Authorization",
                    "type": "http",
                    "scheme": "bearer",
                    "bearerFormat": "JWT",
                    "description": "Authorization: Bearer <access_token>"
                }
            }
        }
    }

    SQLALCHEMY_TRACK_MODIFICATIONS = False
    SQLALCHEMY_DATABASE_URI = os.getenv("DATABASE_URL")
    JWT_SECRET_KEY = os.getenv("JWT_SECRET_KEY")
