#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
// #include <linux/i2c-dev.h>
#include <sys/ioctl.h>
// #include <i2c/smbus.h>
#include <linux/i2c-dev.h>

#define I2C_BUS "/dev/i2c-1"  // Cambiar según el bus I2C utilizado
#define ARDUINO_ADDRESS 0x30  // Cambiar según la dirección del Arduino en el bus I2C

int enviar_entero(int canal, int valor, int file) {
    uint8_t byte1 = valor & 0xFF;  // Los 8 bits menos significativos
    uint8_t byte2 = (valor >> 8) & 0xFF;  // Los 8 bits más significativos

    if (write(file, &canal, 1) != 1) {
        perror("Error al enviar el canal I2C");
        return 1;
    }

    if (write(file, &byte1, 1) != 1) {
        perror("Error al enviar el byte1 I2C");
        return 1;
    }

    if (write(file, &byte2, 1) != 1) {
        perror("Error al enviar el byte2 I2C");
        return 1;
    }

    return 0;
}

int readNumber(int file, int* channel, int* message) {
    if (read(file, channel, 1) != 1) {
        perror("Error al leer el canal I2C");
        return 1;
    }
    
    uint8_t byte2, byte1;

    if (read(file, &byte2, 1) != 1) {
        perror("Error al leer el byte2 I2C");
        return 1;
    }

    if (read(file, &byte1, 1) != 1) {
        perror("Error al leer el byte1 I2C");
        return 1;
    }

    *message = (byte1 << 8) | byte2;

    return 0;
}

int main() {
    int file;

    // Abrir el bus I2C
    file = open(I2C_BUS, O_RDWR);
    if (file < 0) {
        perror("Error al abrir el bus I2C");
        return 1;
    }

    // Establecer la dirección del Arduino en el bus I2C
    if (ioctl(file, I2C_SLAVE, ARDUINO_ADDRESS) < 0) {
        perror("Error al establecer la dirección del Arduino I2C");
        close(file);
        return 1;
    }

    int canal, valor;
    int channel, message;

    while (1) {
        printf("Canal (1Byte): ");
        scanf("%d", &canal);

        printf("Valor (2Bytes): ");
        scanf("%d", &valor);

        enviar_entero(canal, valor, file);
        usleep(100000);  // Esperar 0.1 segundos

        if (readNumber(file, &channel, &message) != 0) {
            close(file);
            return 1;
        }

        printf("Respuesta recibida: Canal = %d, Mensaje = %d\n", channel, message);
    }

    // Cerrar el bus I2C
    close(file);

    return 0;
}
