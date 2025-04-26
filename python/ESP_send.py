import flask

app = flask.Flask(__name__)

@app.route('/')
def index():
    return "test\r"

app.run(port=8080,host='0.0.0.0')