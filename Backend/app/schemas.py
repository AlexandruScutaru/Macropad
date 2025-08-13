from marshmallow import Schema, fields, validates_schema, ValidationError


class Base(Schema):
    id = fields.Int(dump_only=True)


class UserSchema(Base):
    email = fields.Str(required=False)
    username = fields.Str(required=False)
    password = fields.Str(required=True, load_only=True)

    @validates_schema
    def validate_email_or_username(self, data, **kwargs):
        if "email" not in data and "username" not in data:
            raise ValidationError("`email` or `username` is required")


class OptionSchema(Base):
    name = fields.Str(required=True)
    value = fields.Str(required=True)

class KeySchema(Base):
    index = fields.Int(required=True)
    action = fields.Str(required=True)
    options = fields.List(fields.Nested(OptionSchema()), required=False)

class LayerSchema(Base):
    index = fields.Int(required=True)
    color = fields.Str(required=True)
    keys = fields.List(fields.Nested(KeySchema()), required=False)

class ProfileSchema(Base):
    name = fields.Str(required=True)
    layers = fields.List(fields.Nested(LayerSchema()), required=False)


class TokensSchema(Schema):
    access_token = fields.Str(required=True)
    refresh_token = fields.Str(required=True)

class ErrorSchema(Schema):
    status = fields.Integer(required=True, metadata={"description": "HTTP status code"})
    error = fields.String(required=True, metadata={"description": "HTTP error message"})
    message = fields.String(required=True, metadata={"description": "Error message"})
