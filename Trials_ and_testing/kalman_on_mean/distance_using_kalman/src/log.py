import serial
from datetime import datetime

sensor = "DH11"
serial_port = '/dev/ttyUSB0'
baud_rate = 115200
path = "LogFile.txt"
ser = serial.Serial(serial_port, baud_rate)
with open(path, 'w+') as f:
    while True:
        line = ser.readline()
        f.writelines([line])
