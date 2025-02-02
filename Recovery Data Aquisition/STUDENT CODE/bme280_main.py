import machine 
import time
import machine
from LIBRARIES import bme280_int  # Assuming the above code is saved as bme280.py

# Create File to save data to
#file = open("bme280.txt" , "x")
#file.close()

#Configure i2c (basiclly tell the Raspberry Pi which pins the sensor will connec to)
i2c = machine.I2C(1, sda=machine.Pin(26), scl=machine.Pin(27))

# Create BME280 object
bme = bme280_int.BME280(i2c=i2c)

# Check if the sensor is connected
'''try:
    # Try reading the sensor once to ensure it's connected
    ## this is a just written for the purpose of making sure the data from the sensor is being sent to the computer properly.
    bme.read_compensated_data()
    print("BME280 Sensor sucess.")
    # this exception just redirects the computer in what to do in case the run through of the information from the sensor to the computer fails to work.
except Exception as e:
    print("Error connecting to BME280 sensor:", e)
    # Exit if there's an issue connecting to the sensor
    raise
    # raise simply allows for us t signal that an error has occured in the program
'''
# Read and print compensated data (temperature, pressure, humidity)
while True:
    #try:
        temp, pressure, humidity = bme.read_compensated_data()
        
        #write data
        temp_file.write(str(temp) + '\n')
        pressure_file.write(str(pressure) + '\n')
        humidity_file.write(str(humidity) + '\n')
        
        # Convert to human-readable values and format output
        print("Sensor Data:\n")
        print("Temperature: °C " + str(temp/100))
        print("Pressure: hPa " + str(pressure/10000))
        print("Humidity: %" + str(humidity/100))
        
        #Autosave
        
        counter += 1
        if counter % autosave_interval == 0:
            temp_file.flush() 
            pressure_file.flush()
            humidity_file.flush()
            uos.sync()
            
        time.sleep(0.5)

finally:
    temp_file.close()
    pressure_file.close()
    humidity_file.close()