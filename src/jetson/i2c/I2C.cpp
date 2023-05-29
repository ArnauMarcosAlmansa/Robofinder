#include "I2C.h"
#include <stdexcept>

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
    uint8_t buffer[1] = {static_cast<uint8_t>(MessageType::STOP)};

    writeBytes(buffer, sizeof(buffer));
}

std::pair<int, int> I2C::getEncoderLeft() {
    uint8_t send_buffer[1] = {static_cast<uint8_t>(MessageType::READ_ENCODER_LEFT)};
    writeBytes(send_buffer, sizeof(send_buffer));

    uint8_t buffer[3];
    if (!readBytes(buffer, sizeof(buffer)))
        throw std::runtime_error("No se ha leido.");

    int channel = buffer[0];
    int message = (buffer[1] << 8) | buffer[2];
    return std::make_pair(channel, message);
}

std::pair<int, int> I2C::getEncoderRight() {
    uint8_t send_buffer[1] = {static_cast<uint8_t>(MessageType::READ_ENCODER_RIGHT)};
    writeBytes(send_buffer, sizeof(send_buffer));

    uint8_t buffer[3];
    if (!readBytes(buffer, sizeof(buffer)))
        throw std::runtime_error("No se ha leido.");

    int channel = buffer[0];
    int message = (buffer[1] << 8) | buffer[2];
    return std::make_pair(channel, message);
}

void I2C::resetEncoderLeft() {
    uint8_t buffer[1] = {static_cast<uint8_t>(MessageType::RESET_ENCODER_LEFT)};

    writeBytes(buffer, sizeof(buffer));
}

void I2C::resetEncoderRight() {
    uint8_t buffer[1] = {static_cast<uint8_t>(MessageType::RESET_ENCODER_RIGHT)};

    writeBytes(buffer, sizeof(buffer));
}

void I2C::getUltrasonicLeft() {
    uint8_t send_buffer[1] = {static_cast<uint8_t>(MessageType::ULTRASOUND_LEFT)};
    writeBytes(send_buffer, sizeof(send_buffer));

    uint8_t buffer[3];
    if (!readBytes(buffer, sizeof(buffer)))
        return;

    int channel = buffer[0];
    int message = (buffer[1] << 8) | buffer[2];
    return;
    // return (channel, message);
}

void I2C::getUltrasonicRight() {
    uint8_t send_buffer[1] = {static_cast<uint8_t>(MessageType::ULTRASOUND_RIGHT)};
    writeBytes(send_buffer, sizeof(send_buffer));

    uint8_t buffer[3];
    if (!readBytes(buffer, sizeof(buffer)))
        return;

    int channel = buffer[0];
    int message = (buffer[1] << 8) | buffer[2];
    return;
    // return (channel, message);
}

void I2C::ledOff() {
    uint8_t send_buffer[1] = {static_cast<uint8_t>(MessageType::LED_OFF)};
    writeBytes(send_buffer, sizeof(send_buffer));
}
