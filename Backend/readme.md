### Getting started:
- install python (this project uses 3.13)
- install pip
- cd into `Backend` directory
- setup the virtual environment:
    + `python -m venv .venv`
    + `source .venv/bin/activate`
    + `pip install -r requirements.txt`
- optionally (for vscode) select the current `.venv` interpreter:
    + use `Ctrl+Shift+P` to open the command palette and search for `Select interpreter`
    + from the dropdown select the `.venv/bin/python` from the current `workspace` or manually select the `.venv` path
- running the app locally can be done via one of the below options:
    + using vscode - there are launch configurations to start it (either local debug or attach to a debugger remotely)
    + from the root of the project directory run `flask run`
    + `docker-compose up` (optionally with the volume specified so that hot-reload is enabled and changes are reflected in the currently running docker instance)
