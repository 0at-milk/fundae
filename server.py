from flask import Flask
from flask import render_template
from flask import Response, request, jsonify
app = Flask(__name__, template_folder)


@app.route('/')
def home():
    return render_template('index.html') 

@app.route('/module')
def module():
    return render_template('documentation.html')


if __name__ == '__main__':
   app.run(debug = True)




