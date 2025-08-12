import os
import logging
from logging.handlers import RotatingFileHandler

from flask import Flask, jsonify
from flask_jwt_extended import JWTManager


def init_file_logger(app: Flask):
    if app.debug:
        return

    logs_path = os.path.join(app.instance_path, "logs")
    os.makedirs(logs_path, exist_ok=True)
    file_handler = RotatingFileHandler(os.path.join(logs_path, "macropad.log"), maxBytes=1024*1024*10, backupCount=5)
    file_handler.setFormatter(logging.Formatter(
        '%(asctime)s %(levelname)s: %(message)s [in %(pathname)s:%(lineno)d]')
    )
    file_handler.setLevel(logging.DEBUG)
    app.logger.addHandler(file_handler)


def set_jwt_loaders(jwt: JWTManager):
    unauthorized = {"status": 401, "error": "Unauthorized"}

    @jwt.unauthorized_loader
    def unauthorized_callback(error):
        return (jsonify(unauthorized | {"message": "The token is missing."}), 401)

    @jwt.expired_token_loader
    def expired_token_callback(jwt_header, jwt_payload):
        return (jsonify(unauthorized | {"message": "The token has expired."}), 401)

    @jwt.revoked_token_loader
    def revoked_token_callback(jwt_header, jwt_payload):
        return (jsonify(unauthorized | {"message": "The token has been revoked."}), 401)

    @jwt.needs_fresh_token_loader
    def needs_fresh_token_callback(jwt_header, jwt_payload):
        return (jsonify(unauthorized | {"message": "The token is not fresh."}), 401)

    @jwt.invalid_token_loader
    def invalid_token_callback(error):
        return (jsonify({"status": 422, "error": "Unprocessable Content", "message": "Invalid token."}), 422)

    @jwt.token_in_blocklist_loader
    def token_in_blocklist_callback(jwt_header, jwt_payload):
        return JtiModel.query.filter(JtiModel.jti == jwt_payload["jti"]).first() is not None


from app.models import JtiModel
