from typing import List

import sqlalchemy as sql
import sqlalchemy.orm as orm

from app import db


class UserModel(db.Model):
    __tablename__ = "users"

    id: orm.Mapped[int] = orm.mapped_column(primary_key=True)
    email: orm.Mapped[str] = orm.mapped_column(sql.String(64), index=True, unique=True, nullable=False)
    username: orm.Mapped[str] = orm.mapped_column(sql.String(64), index=True, unique=True, nullable=False)
    password_hash: orm.Mapped[str] = orm.mapped_column(sql.String(), unique=False, nullable=False)

    profiles: orm.WriteOnlyMapped[List["ProfileModel"]] = orm.relationship(back_populates="user", cascade="all,delete", passive_deletes=True)

    def __repr__(self):
        return f"<User ID: {self.id} '{self.username}'>"


from app.models.profile import ProfileModel
