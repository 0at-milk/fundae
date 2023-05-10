from flask import Flask
from flask import render_template
from flask import Response, request, jsonify
app = Flask(__name__, template_folder)


@app.route('/')
def home():
    return render_template('index.html') 

@app.route('/mod1')
def module():
    return render_template('documentation_mod1.html')

@app.route('/mod2')
def module():
    return render_template('documentation_mod2.html')

@app.route('/final')
def module():
    return render_template('documentation_final.html')


if __name__ == '__main__':
   app.run(debug = True)




