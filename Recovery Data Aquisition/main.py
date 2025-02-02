print("THIS IS MAIN!")
import machine
import time
import machine
import utime
import uos
from LIBRARIES import SDcard
from LIBRARIES import bno055
from LIBRARIES import micropyGPS
from LIBRARIES import bmp280

def kHz_to_hz(kHz):
    return kHz * 1000
def sec_to_ms(secs):
    return secs * 1000

# I2C Clock Speed  |  Effective Data Rate  |  Equivalent UART Baud Rate
#      100 kHz     |       ~10 kBps        |       ~9,600 bps
#      400 kHz     |       ~40 kBps        |       ~38,400 bps
#       1 MHz      |       ~100 kBps       |       ~115,200 bps
#      3.4 MHz     |       ~340 kBps       |       ~345,600 bps

FREQUENCY = kHz_to_hz(100) # Set in kHz for i2c devices
BAUDRATE = 9600
WRITE_INTERVAL = sec_to_ms(1) # Seconds
RUN_TIME = sec_to_ms(120) # Seconds
START_TIME = time.ticks_ms() # Be careful of wraparound point

# Creat SDcard object and mount filesystem
try:
    cs = machine.Pin(9, machine.Pin.OUT)
    spi = machine.SPI(1,
                      baudrate=1000000,
                      polarity=0,
                      phase=0,
                      bits=8,
                      firstbit=machine.SPI.MSB,
                      sck=machine.Pin(10),
                      mosi=machine.Pin(11),
                      miso=machine.Pin(8))
    sd = SDcard.SDCard(spi, cs)
    vfs = uos.VfsFat(sd)
    uos.mount(vfs, "/sd")
except Exception:
    print("No SD Card")
        
#BMP280 INIT
BMP_i2c = machine.I2C(1, sda=machine.Pin(26), scl=machine.Pin(27), freq=FREQUENCY)
BMP_SENSOR = bmp280.BMP280(i2c_bus=BMP_i2c)

#BNO055 INIT
BNO_i2c = machine.I2C(0, sda=machine.Pin(0), scl=machine.Pin(1), freq=FREQUENCY)
BNO_SENSOR = bno055.BNO055(BNO_i2c)

#MTK3339 INIT
gps_serial = machine.UART(1, baudrate=BAUDRATE, tx=20, rx=21)
MTK_SENSOR = micropyGPS.MicropyGPS()

#LoRa RP2040 Feather INIT
LoRa_serial = machine.UART(1, baudrate=BAUDRATE, tx=4, rx=5)

with open('RocketData.csv', 'w') as file:
    file.write(",".join(["Milliseconds", "BMP_temperature", "BMP_pressure", "BNO_acceleration", "BNO_gyro", "BNO_temperature", "MTK_latitude", "MTK_longitude", "MTK_compass"]) + "\n")

    last_write_time = 0
    while True:
        clock = time.ticks_ms() - START_TIME
        
        
        BMP_temperature = BMP_SENSOR.temperature
        BMP_pressure = BMP_SENSOR.pressure

        BNO_acceleration = BNO_SENSOR.accel()
        BNO_gyro = BNO_SENSOR.gyro()
        BNO_temperature = BNO_SENSOR.temperature()
        
        BNO_acceleration_string = "{} : {} : {}".format(BNO_acceleration[0], BNO_acceleration[1], BNO_acceleration[2])
        BNO_gyro_string = "{} : {} : {}".format(BNO_gyro[0], BNO_gyro[1], BNO_gyro[2])
        
        MTK_latitude = 0
        MTK_longitude = 0
        MTK_compass = "N/A"

        if gps_serial.any():
            line = gps_serial.readline()  # Read a complete line from the UART
            if line:
                try:
                    line = line.decode('utf-8').strip()
                    for char in line:
                        MTK_SENSOR.update(char)
                except Exception:
                    print("Decoding error occured")
                                                        
        if MTK_SENSOR.valid:
            MTK_latitude = MTK_SENSOR.latitude_string()
            MTK_longitude = MTK_SENSOR.longitude_string()
            MTK_compass = MTK_SENSOR.compass_direction()
            
        if clock > (last_write_time + WRITE_INTERVAL):
            compiled_data = (",".join(map(str,[clock, BMP_temperature, BMP_pressure, BNO_acceleration_string, BNO_gyro_string, BNO_temperature, MTK_latitude, MTK_longitude, MTK_compass])) +  "\n")
            
            #Write to SD and LORA
            try:
                LoRa_serial.write(compiled_data)
                print("Writing to LORA")
            except Exception:
                print("Couldn't write :(")
            
            file.write(compiled_data)
            file.flush()
            uos.sync()
            
            last_write_time = clock
            
        if clock > RUN_TIME:
            print("All done! :)")
            break

