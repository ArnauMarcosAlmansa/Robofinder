#pragma once
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <utility>


#define ARDUINO_ADDRESS 0x30

enum class MessageType : unsigned char
{
    FORWARD = 11,
    BACKWARD = 12,
    TURN_LEFT = 13,
    TURN_RIGHT = 14,
    STOP = 15,
    READ_ENCODER_LEFT = 20,
    READ_ENCODER_RIGHT = 21,
    RESET_ENCODER_LEFT = 22,
    RESET_ENCODER_RIGHT = 23,
    ULTRASOUND_LEFT = 30,
    ULTRASOUND_RIGHT = 31,
    LED_OFF = 40
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

    void forward(int vel_left, int vel_right, int encoder_value);
    void backward(int vel_left, int vel_right, int encoder_value);
    void left(int vel_left, int vel_right, int encoder_value);
    void right(int vel_left, int vel_right, int encoder_value);

    void stop();

    std::pair<int, int> getEncoderLeft();
    std::pair<int, int> getEncoderRight();

    void resetEncoderLeft();
    void resetEncoderRight();

    void getUltrasonicLeft();
    void getUltrasonicRight();

    void ledOff();
};
