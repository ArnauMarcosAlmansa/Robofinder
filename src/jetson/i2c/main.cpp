#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define ARDUINO_ADDRESS 0x30

int arduino_bus;

void setup() {
    arduino_bus = open("/dev/i2c-1", O_RDWR);
    if (arduino_bus < 0) {
        perror("Error al abrir el bus I2C");
        return;
    }

    if (ioctl(arduino_bus, I2C_SLAVE, ARDUINO_ADDRESS) < 0) {
        perror("Error al establecer la dirección del Arduino I2C");
        close(arduino_bus);
        return;
    }
}

void read_1byte_2bytes(int* channel, int* message) {
    uint8_t buffer[3];
    if (read(arduino_bus, buffer, sizeof(buffer)) != sizeof(buffer)) {
        perror("Error al leer datos del Arduino I2C");
        return;
    }

    *channel = buffer[0];
    *message = (buffer[1] << 8) | buffer[2];
}

void send_1byte(int byte1) {
    if (write(arduino_bus, &byte1, sizeof(byte1)) != sizeof(byte1)) {
        perror("Error al enviar datos al Arduino I2C");
    }
}

void send_1byte_1byte_1byte_2byte(int byte1, int byte2, int byte3, int twoBytes) {
    uint8_t buffer[5] = { (uint8_t)byte1, (uint8_t)byte2, (uint8_t)byte3, (uint8_t)(twoBytes & 0xFF), (uint8_t)((twoBytes >> 8) & 0xFF) };
    if (write(arduino_bus, buffer, sizeof(buffer)) != sizeof(buffer)) {
        perror("Error al enviar datos al Arduino I2C");
    }
}

int main() {
    setup();

    int channel, message;
    while (1) {
        printf("Enviar mensaje. 0 No/1 Sí: ");
        int option;
        scanf("%d", &option);

        if (option) {
            int byte1, byte2, byte3, twoBytes;
            printf("Canal (1Byte): ");
            scanf("%d", &byte1);
            printf("Valor (1Bytes): ");
            scanf("%d", &byte2);
            printf("Valor (1Bytes): ");
            scanf("%d", &byte3);
            printf("Valor (2Bytes): ");
            scanf("%d", &twoBytes);

            send_1byte_1byte_1byte_2byte(byte1, byte2, byte3, twoBytes);
        } else {
            int byte1;
            printf("Canal (1Byte): ");
            scanf("%d", &byte1);

            send_1byte(byte1);
        }

        usleep(100000);
        read_1byte_2bytes(&channel, &message);
        printf("Respuesta recibida: Canal = %d, Mensaje = %d\n", channel, message);
    }

    close(arduino_bus);

    return 0;
}

