#include "I2C.h"

I2C::I2C() : bus(-1) {}

I2C::~I2C() {
    closeBus();
}

bool I2C::openBus(const std::string& device) {
    bus = open(device.c_str(), O_RDWR);
    if (bus < 0) {
        perror("Error al abrir el bus I2C");
        return false;
    }
    return true;
}

bool I2C::setAddress(int address) {
    if (ioctl(bus, I2C_SLAVE, address) < 0) {
        perror("Error al establecer la dirección del Arduino I2C");
        closeBus();
        return false;
    }
    return true;
}

bool I2C::readBytes(uint8_t* buffer, size_t size) {
    if (read(bus, buffer, size) != static_cast<ssize_t>(size)) {
        perror("Error al leer datos del Arduino I2C");
        return false;
    }
    return true;
}

bool I2C::writeBytes(const uint8_t* buffer, size_t size) {
    if (write(bus, buffer, size) != static_cast<ssize_t>(size)) {
        perror("Error al enviar datos al Arduino I2C");
        return false;
    }
    return true;
}

void I2C::closeBus() {
    if (bus >= 0) {
        close(bus);
        bus = -1;
    }
}
