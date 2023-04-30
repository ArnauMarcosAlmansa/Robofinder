import smbus2
import time

DEVICE_ADDRESS = 0x04
REGISTRO_LECTURA = 0x00
REGISTRO_ESCRITURA = 0x01

bus = smbus2.SMBus(1)

def read_data():
    data = bus.read_byte_data(DEVICE_ADDRESS, REGISTRO_LECTURA)
    return data

def write_data(value):
    bus.write_byte_data(DEVICE_ADDRESS, REGISTRO_ESCRITURA, value)

while True:
    value = int(input("Ingrese un valor entre 0 y 255: "))
    write_data(value)
    time.sleep(1)
    data = read_data()
    print("El valor leido es: ", data)
