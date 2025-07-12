import serial, time
from config import *

if RASPBERRY: 
    # ser = serial.Serial ("/dev/ttyAMA0") #Open named port
    ser = serial.Serial ("COM3") #Open named port
else:
    # python -m serial.tools.list_ports
    ser = serial.Serial ("/dev/cu.usbmodemF412FA9F73AC2", 9600) #Open named port

# ser.baudrate = 9600 #Set baud rate to 9600


# ser.close()


def sendCmd(payload):
    ser.write(bytes(payload,   'utf-8')) #Send back the received data
    print("send value: {}".format(bytes(payload,'utf-8')))
    # time.sleep(1)
    ser.write(bytes(payload,   'utf-8')) #Send back the received data

def receiveCmd():
    while (ser.in_waiting > 0):
        # pass

        data = ser.readline() #Read ten characters from serial port to data
        print("Received value: {}".format(data))
    
    
    
if __name__ == "__main__":
    time.sleep(6)
    sendCmd("NHD\n")
    receiveCmd()
    sendCmd("HD\n")
    receiveCmd()
    sendCmd("MC\n")
    receiveCmd()
    sendCmd("MO\n")
    receiveCmd()
