import smbus
import time

def enviar_entero(canal):
    bus.write_byte(arduino_address, canal)

def readNumber():
    channel = bus.read_byte(arduino_address)
    
    byte2 = bus.read_byte(arduino_address)
    byte1 = bus.read_byte(arduino_address)
    
    
    message = (byte1 << 8) | byte2
    
    return (channel, message)


# Enviar datos de la Jetson al Arduino.
arduino_address = 0x30
bus = smbus.SMBus(1)

while True:
  enviar_entero(int(input("Canal (1Byte): ")))
  time.sleep(0.1)
  print(readNumber())
