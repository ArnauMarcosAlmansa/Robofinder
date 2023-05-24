import smbus
import time

arduino_address = 0x30
bus = smbus.SMBus(1)

def read_1byte_2bytes():
    channel = bus.read_byte(arduino_address)
    
    byte2 = bus.read_byte(arduino_address)
    byte1 = bus.read_byte(arduino_address)

    message = (byte1 << 8) | byte2
    
    return (channel, message)


def send_1byte(byte1):
    bus.write_byte(arduino_address, byte1)


def send_1byte_1byte_1byte_2byte(byte1, byte2, byte3, twoBytes):
    bus.write_byte(arduino_address, byte1)  # Enviamos el primer Byte, el que usamos para el canal.
    
    # Los siguientes Bytes son parametros para las funciones del arduino.
    bus.write_byte(arduino_address, byte2)  
    bus.write_byte(arduino_address, byte3)

    # Separamos el valor de 2Bytes en dos variables de 1Byte para poderlos enviar.
    byte1_twoBytes = twoBytes & 0xFF                # Los 8 bits menos significativos
    byte2_twoBytes = (twoBytes >> 8) & 0xFF         # Los 8 bits mas significativos

    bus.write_byte(arduino_address, byte1_twoBytes) # Enviamos los menos significativos
    bus.write_byte(arduino_address, byte2_twoBytes) # Enviamos los mas significativos

while True:
    if (int(input("Enviar mensaje. 0 No/ 1 Si"))):
        send_1byte_1byte_1byte_2byte(int(input("Canal (1Byte): ")), int(input("Valor (1Bytes): ")), int(input("Valor (1Bytes): ")), int(input("Valor (2Bytes): ")))
    else:
        send_1byte(int(input("Canal (1Byte): ")))

    time.sleep(0.1)
    channel, message = read_1byte_2bytes()
    print(message)
