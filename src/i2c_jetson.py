import smbus

# Enviar datos de la Jetson al Arduino.
arduino_address = 0x04
bus = smbus.SMBus(1)
bus.write_byte(arduino_address, 10)
bus.write_byte(arduino_address, 60)
