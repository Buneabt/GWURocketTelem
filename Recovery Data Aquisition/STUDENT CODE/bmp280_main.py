#BMP280 GATHERS TEMPERATURE & PRESSURE - can be used to calculate altitude above sea level.
# micropython imports
import machine
import uos

# other imports (built-in libraries and online packages)
import time
from LIBRARIES import bmp280

""" DOCUMENTATION: class machine.I2C(id, *, scl, sda, freq=400000, timeout=50000)
- Construct and return a new I2C object using the following parameters:
- id identifies a particular I2C peripheral. Allowed values for depend on the particular port/board
- scl should be a pin object specifying the pin to use for SCL.
- sda should be a pin object specifying the pin to use for SDA.
- freq should be an integer which sets the maximum frequency for SCL.
- timeout is the maximum time in microseconds to allow for I2C transactions. This parameter is not allowed on some ports."""

# https://github.com/pimoroni/bmp280-python/blob/main/bmp280/__init__.py

"""
'r': Open for reading (default).
'w': Open for writing, truncating the file if it exists.
'x': Open for exclusive creation, failing if the file already exists.
'a': Open for writing, appending to the end of the file if it exists.
'b': Open in binary mode (e.g., 'rb' for reading in binary mode). 
't': Open in text mode (default, e.g., 'rt' for reading in text mode).
"""
# Create a file to save data to
#file = open("altimeter.txt", "x")
#file.close()

# Configure i2c (basically tell the Raspberry Pi which pins the sensor will connect to)
i2c = machine.I2C(1, sda=machine.Pin(26), scl=machine.Pin(27))

# Create bmp280 object, inputting i2c object as parameter
SENSOR = bmp280.BMP280(i2c_bus=i2c)
print("Altimeter object created:", SENSOR)

# Number of readings after which to autosave
autosave_interval = 60

# Open the previously created file to be written in
temperature_file = open("temperature.txt", "a")
pressure_file = open("pressure.txt", "a")
counter = 0

try:
    while True:
        temperature = SENSOR.temperature
        pressure = SENSOR.pressure

        #write data to file
        temperature_file.write(str(temperature) + "\n")
        pressure_file.write(str(pressure) + "\n")
        
        print("Temp (C): " + str(temperature))
        print("Pressure (Pa): " + str(pressure))

        # Autosave feature
        counter += 1
        if counter % autosave_interval == 0:
            temperature_file.flush() # Writes temporary memory to file on disk
            pressure_file.flush()
            uos.sync() # Double layer of safety, ensuring that temporary data is stored

        time.sleep(0.5) # Take a data reading every half-second
finally:
    temperature_file.close() # Close the file in case of technical failure
    pressure_file.close()












