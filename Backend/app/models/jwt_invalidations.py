from app import db


class JtiModel(db.Model):
    __tablename__ = "jti_invalidations"

    id = db.Column(db.Integer, primary_key=True)
    jti = db.Column(db.String(), unique=True, nullable=False)
