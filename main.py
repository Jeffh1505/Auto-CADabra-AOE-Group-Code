from flask import Flask, render_template
import serial

app = Flask(__name__)

serialPort = "Port here"
ser = serial.Serial(serialPort, 9600)

@app.route('/')
def index():
    score_data = ser.readline().decode('utf-8').strip()
    score = score_data.split(":")[-1].strip()  
    return render_template('index.html', score=score)

if __name__ == '__main__':
    app.run(debug=True)