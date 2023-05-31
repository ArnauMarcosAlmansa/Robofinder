#pragma once
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "libi2c.h"
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
    I2CDevice device;
    int file_descriptor;

public:
    I2C();
    I2C(const std::string& device, int address);
    ~I2C();
    bool readBytes(uint8_t* bytes, size_t len);
    bool writeBytes(uint8_t* bytes, size_t len); 
    void closeBus();

    void forward(int vel_left, int vel_right, int encoder_value);
    void backward(int vel_left, int vel_right, int encoder_value);
    void turn_left(uint8_t vel_left, uint8_t vel_right, uint16_t encoder_value);
    void turn_right(uint8_t vel_left, uint8_t vel_right, uint16_t encoder_value);

    void stop();

    std::pair<int, int> getEncoderLeft();
    std::pair<int, int> getEncoderRight();

    void resetEncoderLeft();
    void resetEncoderRight();

    void getUltrasonicLeft();
    void getUltrasonicRight();

    void ledOff();
};
