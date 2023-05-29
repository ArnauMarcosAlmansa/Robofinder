#pragma once
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define ARDUINO_ADDRESS 0x30


enum class MessageType : unsigned char
{
    FORWARD = 11,
    BACKWARD = 12,
    TURN_LEFT = 13,
    TURN_RIGHT = 14,
};

class I2C {
private:
    int bus;

public:
    I2C();
    ~I2C();
    bool openBus(const std::string& device);
    bool setAddress(int address);
    bool readBytes(uint8_t* buffer, size_t size);
    bool writeBytes(const uint8_t* buffer, size_t size);
    void closeBus();
};
