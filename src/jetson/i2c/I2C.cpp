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

void I2C::forward(int vel_left, int vel_right, int encoder_value) {
    uint8_t buffer[5] = {static_cast<uint8_t>(MessageType::FORWARD), static_cast<uint8_t>(vel_left),
                         static_cast<uint8_t>(vel_right), static_cast<uint8_t>(encoder_value & 0xFF),
                         static_cast<uint8_t>((encoder_value >> 8) & 0xFF)};

    writeBytes(buffer, sizeof(buffer));
}

void I2C::backward(int vel_left, int vel_right, int encoder_value) {
    uint8_t buffer[5] = {static_cast<uint8_t>(MessageType::BACKWARD), static_cast<uint8_t>(vel_left),
                         static_cast<uint8_t>(vel_right), static_cast<uint8_t>(encoder_value & 0xFF),
                         static_cast<uint8_t>((encoder_value >> 8) & 0xFF)};

    writeBytes(buffer, sizeof(buffer));
}

void I2C::left(int vel_left, int vel_right, int encoder_value) {
    uint8_t buffer[5] = {static_cast<uint8_t>(MessageType::TURN_LEFT), static_cast<uint8_t>(vel_left),
                         static_cast<uint8_t>(vel_right), static_cast<uint8_t>(encoder_value & 0xFF),
                         static_cast<uint8_t>((encoder_value >> 8) & 0xFF)};

    writeBytes(buffer, sizeof(buffer));
}

void I2C::right(int vel_left, int vel_right, int encoder_value) {
    uint8_t buffer[5] = {static_cast<uint8_t>(MessageType::TURN_RIGHT), static_cast<uint8_t>(vel_left),
                         static_cast<uint8_t>(vel_right), static_cast<uint8_t>(encoder_value & 0xFF),
                         static_cast<uint8_t>((encoder_value >> 8) & 0xFF)};

    writeBytes(buffer, sizeof(buffer));
}

void I2C::stop() {
    writeBytes(reinterpret_cast<const uint8_t *>(&MessageType::STOP), sizeof(MessageType::STOP));
}

void I2C::getEncoderLeft() {
    writeBytes(reinterpret_cast<const uint8_t *>(&MessageType::READ_ENCODER_LEFT), sizeof(MessageType::READ_ENCODER_LEFT));

    uint8_t buffer[3];
    if (!readBytes(buffer, sizeof(buffer)))
        break;

    channel = buffer[0];
    message = (buffer[1] << 8) | buffer[2];
    return (channel, message)
}

void I2C::getEncoderRight() {
    writeBytes(reinterpret_cast<const uint8_t *>(&MessageType::READ_ENCODER_RIGHT), sizeof(MessageType::READ_ENCODER_RIGHT));

    uint8_t buffer[3];
    if (!readBytes(buffer, sizeof(buffer)))
        break;

    channel = buffer[0];
    message = (buffer[1] << 8) | buffer[2];
    return (channel, message)
}

void I2C::resetEncoderLeft() {
    writeBytes(reinterpret_cast<const uint8_t *>(&MessageType::RESET_ENCODER_LEFT), sizeof(MessageType::RESET_ENCODER_LEFT));
}

void I2C::resetEncoderRight() {
    resetEncoderRight(reinterpret_cast<const uint8_t *>(&MessageType::RESET_ENCODER_RIGHT), sizeof(MessageType::RESET_ENCODER_RIGHT));
}

void I2C::getUltrasonicLeft() {
    writeBytes(reinterpret_cast<const uint8_t *>(&MessageType::ULTRASOUND_LEFT), sizeof(MessageType::ULTRASOUND_LEFT));

    uint8_t buffer[3];
    if (!readBytes(buffer, sizeof(buffer)))
        break;

    channel = buffer[0];
    message = (buffer[1] << 8) | buffer[2];
    return (channel, message)
}

void I2C::getUltrasonicRight() {
    writeBytes(reinterpret_cast<const uint8_t *>(&MessageType::ULTRASOUND_RIGHT), sizeof(MessageType::ULTRASOUND_RIGHT));

    uint8_t buffer[3];
    if (!readBytes(buffer, sizeof(buffer)))
        break;

    channel = buffer[0];
    message = (buffer[1] << 8) | buffer[2];
    return (channel, message)
}

void I2C::ledOff() {
    resetEncoderRight(reinterpret_cast<const uint8_t *>(&MessageType::LED_OFF), sizeof(MessageType::LED_OFF));
}
