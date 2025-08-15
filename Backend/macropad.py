import sqlalchemy as sql
import sqlalchemy.orm as orm

from app import create_app, db
from app.models.user import UserModel
from app.models.profile import ProfileModel

app = create_app()

# added this to help me debug some DB operatios as I had some issues with deletes and foreign key cascades
@app.shell_context_processor
def make_shell_context():
    return {
        "sql": sql,
        "orm": orm,
        "db": db,
        "UserModel": UserModel,
        "ProfileModel": ProfileModel
    }
