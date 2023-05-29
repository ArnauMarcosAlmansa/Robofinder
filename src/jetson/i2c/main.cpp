#include "I2C.h"

int main() {
    I2C i2c;
    if (!i2c.openBus("/dev/i2c-1")) {
        return 1;
    }

    if (!i2c.setAddress(ARDUINO_ADDRESS)) {
        i2c.closeBus();
        return 1;
    }

    int channel, message;
    while (true) {
        std::cout << "Enviar mensaje. 0 No/1 Sí: ";
        int option;
        std::cin >> option;

        if (option) {
            int byte1, byte2, byte3, twoBytes;
            std::cout << "Canal (1Byte): ";
            std::cin >> byte1;
            std::cout << "Valor (1Bytes): ";
            std::cin >> byte2;
            std::cout << "Valor (1Bytes): ";
            std::cin >> byte3;
            std::cout << "Valor (2Bytes): ";
            std::cin >> twoBytes;

            uint8_t buffer[5] = { static_cast<uint8_t>(byte1), static_cast<uint8_t>(byte2),
                                  static_cast<uint8_t>(byte3), static_cast<uint8_t>(twoBytes & 0xFF),
                                  static_cast<uint8_t>((twoBytes >> 8) & 0xFF) };
            if (!i2c.writeBytes(buffer, sizeof(buffer))) {
                break;
            }
        } else {
            int byte1;
            std::cout << "Canal (1Byte): ";
            std::cin >> byte1;

            if (!i2c.writeBytes(reinterpret_cast<const uint8_t*>(&byte1), sizeof(byte1))) {
                break;
            }
        }

        usleep(100000);
        uint8_t buffer[3];
        if (!i2c.readBytes(buffer, sizeof(buffer))) {
            break;
        }

        channel = buffer[0];
        message = (buffer[1] << 8) | buffer[2];
        std::cout << "Respuesta recibida: Canal = " << channel << ", Mensaje = " << message << std::endl;
    }

    i2c.closeBus();
    return 0;
}
