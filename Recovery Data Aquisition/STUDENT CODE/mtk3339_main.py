import time
#import busio
#import board
from LIBRARIES import micropyGPS


'''
from LIBRARIES import adafruit_gps

#Configure UART interface
uart = busio.UART(board.tx, board.RX, baudrate=9600, timeout=10)

#Create sensor object
SENSOR = adafruit_gps.GPS(uart)
print("GPS object created:", SENSOR)

while True:
    sentence = SENSOR._read_sentence()
    print(sentence)
'''

gps_serial = machine.UART(1, baudrate=9600, tx=20, rx=21)
SENSOR = micropyGPS.MicropyGPS()


#while True:
while True:
    if gps_serial.any():
        line = gps_serial.readline()  # Read a complete line from the UART
        if line:
            try:
                line = line.decode('utf-8').strip()
                #print(line)
                for char in line:
                    SENSOR.update(char)
            except Exception:
                print("Decoding error occured")
        else:
            print("No data.")
    if SENSOR.valid:
        print("Latitude: ", SENSOR.latitude_string())
        print("Longitude: ", SENSOR.longitude_string())
        print("Compass: ", SENSOR.compass_direction())
        print("Current Course: ", SENSOR.course)
    else:
        print("Waiting for valid GPS data... Fix Status:", SENSOR.fix_stat)
    
    time.sleep(.5)
