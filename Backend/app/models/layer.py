from typing import List
import sqlalchemy as sql
import sqlalchemy.orm as orm

from app import db
from app.models.profile import ProfileModel


class LayerModel(db.Model):
    __tablename__ = "layers"

    id: orm.Mapped[int] = orm.mapped_column(primary_key=True)
    index: orm.Mapped[int] = orm.mapped_column(unique=False, nullable=False)
    color: orm.Mapped[str] = orm.mapped_column(sql.String(6), unique=False, nullable=False)
    profile_id: orm.Mapped[int] = orm.mapped_column(sql.ForeignKey(ProfileModel.id, onupdate="CASCADE", ondelete="CASCADE"), index=True, nullable=False)

    profile: orm.Mapped["ProfileModel"] = orm.relationship(back_populates="layers")
    keys: orm.Mapped[List["KeyModel"]] = orm.relationship(back_populates="layer", cascade="all,delete,delete-orphan")

    def __repr__(self):
        return f"<Layer ID: {self.id} #{self.color}@{self.index}>"


from app.models.key import KeyModel