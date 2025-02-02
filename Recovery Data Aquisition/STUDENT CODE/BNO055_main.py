#GATHERS ACCELERATION, ORIENTATION
import machine
import utime
import uos
import time
from LIBRARIES import bno055



"""
'r': Open for reading (default).
'w': Open for writing, truncating the file if it exists.
'x': Open for exclusive creation, failing if the file already exists.
'a': Open for writing, appending to the end of the file if it exists.
'b': Open in binary mode (e.g., 'rb' for reading in binary mode). 
't': Open in text mode (default, e.g., 'rt' for reading in text mode).
"""

'''
BNO055 FUNCTIONS:
mag() Magnetometer vector (x, y, z) in μT (microtesla).
accel() Accelerometer vector (x, y, z) in m.s^-2
lin_acc() Acceleration vector (x, y, z) after removal of gravity component (m.s^-2).*
gravity() Gravity vector (x, y, z) in m.s^-2 after removal of acceleration data.*
gyro() Gyro vector (x, y, z) in deg.s^-1.
euler() Euler angles in degrees (heading, roll, pitch).*
quaternion() Quaternion (w, x, y, z).*
temperature() Chip temperature as an integer °C (Celcius).
calibrated() True if all elements of the device are calibrated.
cal_status() Returns bytearray [sys, gyro, accel, mag]. Each element has a value from 0 (uncalibrated) to 3 (fully calibrated).
sensor_offsets() Returns the current calibration offsets stored on the device as a bytearray.
set_offsets() Sets the current calibration offsets of the device. For example, this can be called with the results of sensor_offsets() after a reset.
external_crystal() True if using an external crystal.
get_config(sensor) See Section 3.3.2.'''


#Configure the I2C
i2c = machine.I2C(0, sda=machine.Pin(0), scl=machine.Pin(1))

#Create the bno055 object
SENSOR = bno055.BNO055(i2c)

#Creation and closing of Acceleration file
acceleration_file = open("acceleration.txt", "a")
#Creation and closing og Gyroscope
gyro_file = open("gyro.txt", "a")
#Creation and closing of Temparture file
temperature_file = open("temp.txt", "a")


# Number of readings after which to autosave
autosave_interval = 60
counter = 0

try:
    while True:
        if not SENSOR.cal_status():
            print('Calibration required: sys {} gyro {} accel {} mag {}'.format(*calibration_status))
    
        #Get data
        acceleration = SENSOR.accel()
        gyro = SENSOR.gyro()
        temperature = SENSOR.temperature()
        
        #Write data
        acceleration_file.write(str(acceleration) + "\n")
        gyro_file.write((str(gyro) + "\n"))
        temperature_file.write(str(temperature) + "\n")
        #str() function converts into string
        #"\n" creates a newline to allow for each string to be printed in its own line
        
        
        print("Acceleration (m/s^2): " + str(acceleration))
        print("Gyro (deg.s^-1): " + str(gyro))
        print("Temparture (Celcius):" + str(temperature))
        #print() allows us to test if the code to funtioning properly and is transfering the reading from the sensor(use for testing purposes)

        # Autosave feature
        counter += 1
        if counter % autosave_interval == 0:
            acceleration_file.flush() # Writes temporary memory to file on disk
            gyro_file.flush()
            temperature_file.flush()
            uos.sync() # Double layer of safety, ensuring that temporary data is stored

        time.sleep(0.5) # Take a data reading every half-second
finally:
    acceleration_file.close()
    gyro_file.close()
    temperature_file.close()
    

        
    


