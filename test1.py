import pyfirmata2
import time
board = pyfirmata2.Arduino('COM3')
ledpin = board.get_pin('d:11:p')

for i in range(5):
    ledpin.write(1.0)
    time.sleep(1)
    ledpin.write(0.0)
    time.sleep(1)