from typing import List
import sqlalchemy as sql
import sqlalchemy.orm as orm

from app import db
from app.models.layer import LayerModel


class KeyModel(db.Model):
    __tablename__ = "keys"

    id: orm.Mapped[int] = orm.mapped_column(primary_key=True)
    index: orm.Mapped[int] = orm.mapped_column(unique=False, nullable=False)
    action: orm.Mapped[str] = orm.mapped_column(sql.String(32), unique=False, nullable=False)
    layer_id: orm.Mapped[int] = orm.mapped_column(sql.ForeignKey(LayerModel.id), index=True, nullable=False)

    layer: orm.Mapped["LayerModel"] = orm.relationship(back_populates="keys")
    options: orm.Mapped[List["OptionModel"]] = orm.relationship(back_populates="key", cascade="all, delete")

    def __repr__(self):
        return f"<Key ID: {self.id} '{self.action}'@{self.index}>"


from app.models.option import OptionModel
