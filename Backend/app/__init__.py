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
    if app.debug or "sqlite" in app.config["SQLALCHEMY_DATABASE_URI"]:
        def fk_pragma_on_connect(dbapi_con, con_record):
            dbapi_con.execute("PRAGMA foreign_keys=ON")

        with app.app_context():
            from sqlalchemy import event
            event.listen(db.engine, "connect", fk_pragma_on_connect)

    @app.before_request
    def create_tables():
        db.create_all()

    api = DocApi(app)
    jwt = JWTManager(app)

    routes.register_blueprints(api)
    helpers.set_jwt_loaders(jwt)
    helpers.init_file_logger(app)

    app.logger.setLevel(logging.DEBUG)
    app.logger.info("App started")

    return app


from app import models, routes, helpers
from app.routes.doc_api import DocApi
