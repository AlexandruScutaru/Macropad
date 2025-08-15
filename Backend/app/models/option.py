import sqlalchemy as sql
import sqlalchemy.orm as orm

from app import db
from app.models.key import KeyModel


class OptionModel(db.Model):
    __tablename__ = "options"

    id: orm.Mapped[int] = orm.mapped_column(primary_key=True)
    name: orm.Mapped[str] = orm.mapped_column(sql.String(32), unique=False, nullable=False)
    value: orm.Mapped[str] = orm.mapped_column(sql.String(), unique=False, nullable=False)
    key_id: orm.Mapped[int] = orm.mapped_column(sql.ForeignKey(KeyModel.id, onupdate="CASCADE", ondelete="CASCADE"), index=True, nullable=False)

    key: orm.Mapped["KeyModel"] = orm.relationship(back_populates="options")

    def __repr__(self):
        return f"<Option ID: {self.id} '{self.name}': '{self.value}'>"
