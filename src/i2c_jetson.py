import smbus

def enviar_entero(canal, valor):

    bus.write_byte(arduino_address, canal)

    byte1 = valor & 0xFF # Los 8 bits menos significativos
    byte2 = (valor >> 8) & 0xFF # Los 8 bits mas significativos

    bus.write_byte(arduino_address, byte1)
    bus.write_byte(arduino_address, byte2)
    
    
    
    #print("Canal: {}" .format(canal))
    #print("Valor: {}".format(valor))


# Enviar datos de la Jetson al Arduino.
arduino_address = 0x04
bus = smbus.SMBus(1)

while True:
  enviar_entero(int(input("Canal (1Byte): ")), int(input("Valor (2Bytes): ")))
  
