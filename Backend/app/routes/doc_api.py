from flask_smorest import Api, ErrorHandlerMixin
from werkzeug.exceptions import HTTPException
from app.schemas import ErrorSchema


class DocApi(Api, ErrorHandlerMixin):
    ERROR_SCHEMA = ErrorSchema

    def handle_http_exception(self, error: HTTPException):
        headers = {}
        payload = {
            "status": error.code,
            "error": error.name,
            "message": error.description
        }

        data = getattr(error, "data", None)
        if data:
            if "message" in data:
                payload["message"] = data["message"]

        return payload, error.code, headers
