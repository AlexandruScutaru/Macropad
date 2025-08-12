import logging

from flask import Flask
from flask_sqlalchemy import SQLAlchemy
from flask_jwt_extended import JWTManager

from app.config import Config

db = SQLAlchemy()


def create_app(config_obj: Config = Config):
    app = Flask(__name__)
    app.config.from_object(config_obj)

    db.init_app(app)
    api = DocApi(app)
    jwt = JWTManager(app)

    routes.register_blueprints(api)
    helpers.set_jwt_loaders(jwt)

    @app.before_request
    def create_tables():
        db.create_all()

    helpers.init_file_logger(app)
    app.logger.setLevel(logging.DEBUG)
    app.logger.info("App started")

    return app


from app import models, routes, helpers
from app.routes.doc_api import DocApi
