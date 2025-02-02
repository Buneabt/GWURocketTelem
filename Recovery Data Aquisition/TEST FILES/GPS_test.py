import machine
from time import sleep

#Define the UART pins and create a UART object
gps_serial = machine.UART(0, baudrate=9600, tx=16, rx=17)

while True:
    if gps_serial.any():
        line = gps_serial.readline()  # Read a complete line from the UART
        if line:
            line = line.decode('utf-8')
            print(line.strip())
        else:
            print("No data.")
    sleep(0.5)