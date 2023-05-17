import smbus
import time

def enviar_entero(canal, valor):

    bus.write_byte(arduino_address, canal)

    byte1 = valor & 0xFF # Los 8 bits menos significativos
    byte2 = (valor >> 8) & 0xFF # Los 8 bits mas significativos

    bus.write_byte(arduino_address, byte1)
    bus.write_byte(arduino_address, byte2)
    
    
    
    #print("Canal: {}" .format(canal))
    #print("Valor: {}".format(valor))


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
  enviar_entero(int(input("Canal (1Byte): ")), int(input("Valor (2Bytes): ")))
  time.sleep(0.1)
  print(readNumber())
