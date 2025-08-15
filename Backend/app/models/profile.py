from typing import List
import sqlalchemy as sql
import sqlalchemy.orm as orm

from app import db
from app.models.user import UserModel


class ProfileModel(db.Model):
    __tablename__ = "profiles"

    id: orm.Mapped[int] = orm.mapped_column(primary_key=True)
    name: orm.Mapped[str] = orm.mapped_column(sql.String(64), unique=False, nullable=False)
    user_id: orm.Mapped[int] = orm.mapped_column(sql.ForeignKey(UserModel.id, onupdate="CASCADE", ondelete="CASCADE"), index=True, nullable=False)

    user: orm.Mapped["UserModel"] = orm.relationship(back_populates="profiles")
    layers: orm.Mapped[List["LayerModel"]] = orm.relationship(back_populates="profile", cascade="all,delete,delete-orphan")

    def __repr__(self):
        return f"<Profile ID: {self.id} `{self.name}`>"


from app.models.layer import LayerModel
