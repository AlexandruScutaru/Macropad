from marshmallow import Schema, fields


class UserSchema(Schema):
    id = fields.Int(dump_only=True)
    username = fields.Str(required=True)
    password = fields.Str(required=True, load_only=True)


class TokensSchema(Schema):
    access_token = fields.Str(required=True)
    refresh_token = fields.Str(required=True)


class ErrorSchema(Schema):
    status = fields.Integer(required=True, metadata={"description": "HTTP status code"})
    error = fields.String(required=True, metadata={"description": "HTTP error message"})
    message = fields.String(required=True, metadata={"description": "Error message"})
