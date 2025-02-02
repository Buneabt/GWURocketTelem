import machine
i2c = machine.I2C(0, sda=machine.Pin(8), scl=machine.Pin(9))
devices = i2c.scan()

if devices:
    print("I2C devices found:", [hex(device) for device in devices])
else:
    print("No I2C device found.")