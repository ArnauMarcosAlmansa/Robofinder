#include "I2C.h"
#include <stdexcept>
#include <iostream>
#include <cstring>
I2C::I2C() {
    this->file_descriptor = -1;
}

I2C::I2C(const std::string& device, int address) {
    this->file_descriptor = open(device.c_str(), O_RDWR);

    if (this->file_descriptor == -1) {
        std::cerr << "Error al abrir el bus I2C" << std::endl;
        return;
    }

    if (ioctl(this->file_descriptor, I2C_SLAVE, address) == -1) {
        std::cerr << "Error al establecer la dirección del esclavo" << std::endl;
        return;
    }
    
    std::cout << "Bus abierto y direccion del esclavo establecida" << std::endl;
}

I2C::~I2C() {
    closeBus();
}

bool I2C::readBytes(uint8_t* bytes, size_t len) {

    std::cout << "read " << len << ": ";
    for (size_t i = 0; i < len; i++)
    {
        if (read(file_descriptor, &bytes[i], 1) != 1) {
            std::cerr << "Error al leer el canal" << std::endl;
            return false;
        }

        std::cout << (int) bytes[i] << " ";
    }

    std::cout << std::endl;

    return true;
}

bool I2C::writeBytes(uint8_t* bytes, size_t len) {
    if (write(this->file_descriptor, bytes, len) != (ssize_t) len) {
        std::cerr << "Error al enviar los bytes" << std::endl;
        return -1;
    }
    return 0;
}

void I2C::closeBus() {
    if (file_descriptor >= 0) {
        close(file_descriptor);
        file_descriptor = -1;
    }
}

void I2C::forward(int vel_left, int vel_right, int encoder_value) {
    uint8_t buffer[5] = {
        static_cast<uint8_t>(MessageType::FORWARD),
        static_cast<uint8_t>(vel_left),
        static_cast<uint8_t>(vel_right),
        static_cast<uint8_t>(encoder_value & 0xFF),
        static_cast<uint8_t>((encoder_value >> 8) & 0xFF)
    };

    std::cout << "forward ";
    for (int i = 0; i < 5; i++)
        std::cout << (int) buffer[i] << " ";
    std::cout << std::endl;

    writeBytes(buffer, 5);
    readBytes(buffer, 3);
}

void I2C::backward(int vel_left, int vel_right, int encoder_value) {
    uint8_t buffer[5] = {static_cast<uint8_t>(MessageType::BACKWARD), static_cast<uint8_t>(vel_left),
                         static_cast<uint8_t>(vel_right), static_cast<uint8_t>(encoder_value & 0xFF),
                         static_cast<uint8_t>((encoder_value >> 8) & 0xFF)};

    writeBytes(buffer, sizeof(buffer));
}

/*
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
*/

void I2C::stop() {
    uint8_t canal = static_cast<uint8_t>(MessageType::STOP);
    int result = I2C::writeBytes(&canal, 1);
    if (result != 0){
        throw std::runtime_error("Error al intenar parar");
    }else{
        std::cout <<"Parado correctamente" << std::endl;
    }
}

std::pair<int, int> I2C::getEncoderLeft()
{
    uint8_t canal = static_cast<uint8_t>(MessageType::READ_ENCODER_LEFT);
    bool result = writeBytes(&canal, 1);
    if (result != 0)
        throw std::runtime_error("No se ha escrito encoder left.");

    usleep(10000);
    uint8_t buff[3];
    if (!readBytes(buff, 3))
        throw std::runtime_error("No se ha leido encoder left.");

    int16_t payload = ((int16_t) buff[2]) << 8 | (int16_t) buff[1];
    return std::make_pair((int) buff[0], (int) payload);
}

std::pair<int, int> I2C::getEncoderRight()
{
    uint8_t canal = static_cast<uint8_t>(MessageType::READ_ENCODER_RIGHT);
    bool result = writeBytes(&canal, 1);
    if (result != 0)
        throw std::runtime_error("No se ha escrito encoder right.");

    usleep(10000);
    uint8_t buff[3];
    if (!readBytes(buff, 3))
        throw std::runtime_error("No se ha leido encoder right.");

    int16_t payload = ((int16_t) buff[2]) << 8 | (int16_t) buff[1] ;
    return std::make_pair((int) buff[0], (int) payload);
}

void I2C::resetEncoderLeft() {
    /*uint8_t buffer[1] = {static_cast<uint8_t>(MessageType::RESET_ENCODER_LEFT)};

    writeBytes(buffer, sizeof(buffer));
    */
}

void I2C::resetEncoderRight() {
    /*
    uint8_t buffer[1] = {static_cast<uint8_t>(MessageType::RESET_ENCODER_RIGHT)};

    writeBytes(buffer, sizeof(buffer));
    */
}

void I2C::getUltrasonicLeft() {
    /*
    uint8_t send_buffer[1] = {static_cast<uint8_t>(MessageType::ULTRASOUND_LEFT)};
    writeBytes(send_buffer, sizeof(send_buffer));

    uint8_t buffer[3];
    if (!readBytes(buffer, sizeof(buffer)))
        return;

    int channel = buffer[0];
    int message = (buffer[1] << 8) | buffer[2];
    return;
    // return (channel, message);
    */
}

void I2C::getUltrasonicRight() {
    /*
    uint8_t send_buffer[1] = {static_cast<uint8_t>(MessageType::ULTRASOUND_RIGHT)};
    writeBytes(send_buffer, sizeof(send_buffer));

    uint8_t buffer[3];
    if (!readBytes(buffer, sizeof(buffer)))
        return;

    int channel = buffer[0];
    int message = (buffer[1] << 8) | buffer[2];
    return;
    // return (channel, message);
    */
}

std::pair<int, int> I2C::getMinimumUltraSoundValue()
{
    uint8_t canal = static_cast<uint8_t>(MessageType::MINIMUM_ULTRASOUND_VALUE);
    bool result = writeBytes(&canal, 1);
    if (result != 0)
        throw std::runtime_error("No se ha escrito el valor minimo de ultrasonido.");

    usleep(10000);
    uint8_t buff[3];
    if (!readBytes(buff, 3))
        throw std::runtime_error("No se ha leido el valor minimo.");

    int payload = ((int) buff[2]) << 8 | (int) buff[1] ;
    return std::make_pair((int) buff[0], payload);
}

bool I2C::hasStoped(){
	uint8_t canal = static_cast<uint8_t>(MessageType::TEST_STOP);
	bool result = writeBytes(&canal, 1);
	if (result != 0)
		throw std::runtime_error("No se ha escrito TEST STOP en el canal");
	usleep(10000);
	uint8_t buff[3];
	if (!readBytes(buff, 3))
		throw std::runtime_error("No se ha leido TEST STOP");
	int payload = ((int) buff[2]) << 8 | (int) buff[1];
	if (payload == 110){
		std::cout <<"Ha parado!!!!"<< std::endl;
		return true;
	}else{
		std::cout <<"Valor obtenido: " << payload << std::endl;
		return false;
	}
}

void I2C::ledOff() {
    uint8_t send_buffer[1] = {static_cast<uint8_t>(MessageType::LED_OFF)};
    writeBytes(send_buffer, sizeof(send_buffer));
}


void I2C::turn_left(uint8_t vel_left, uint8_t vel_right, uint16_t encoder_value)
{
    uint8_t send_buffer[] = {
        static_cast<uint8_t>(MessageType::TURN_LEFT),
        vel_left,
        vel_right,
        static_cast<uint8_t>(encoder_value & 0xFF),
        static_cast<uint8_t>((encoder_value >> 8) & 0xFF)
    };
    writeBytes(send_buffer, sizeof(send_buffer));
}

void I2C::turn_right(uint8_t vel_left, uint8_t vel_right, uint16_t encoder_value)
{
    uint8_t send_buffer[] = {
        static_cast<uint8_t>(MessageType::TURN_RIGHT),
        vel_left,
        vel_right,
        static_cast<uint8_t>(encoder_value & 0xFF),
        static_cast<uint8_t>((encoder_value >> 8) & 0xFF)
    };
    writeBytes(send_buffer, sizeof(send_buffer));
}
