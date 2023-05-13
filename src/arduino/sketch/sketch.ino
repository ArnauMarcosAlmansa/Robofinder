#include "Encoder.h"
#include "Motor.h"
#include "UltrasoundSensor.h"
#include <Wire.h>

Encoder encoder1(19,18);
Encoder encoder2(3,2);

UltrasoundSensor ultrasoundsensor1(22, 23);
UltrasoundSensor ultrasoundsensor2(24, 25);

Motor motorLeft(13, 38, 39);
Motor motorRight(12, 40, 41);

#include "i2c.h"


bool i2c_interrupt = false;             // Hacemos la condicion dentro del loop al tener el mensaje completo
int channel_received_i2c = 0;           // El canal que especifica el usuario que quiere usar. (Es el primer mensaje de 1Byte que recibimos).
uint8_t byte1_received_message_i2c = 0; // El primer byte del valor que recibimos.
uint16_t received_message_i2c = 0;      // El valor final del mensaje (2Bytes).

void updateEncoder1() {
  encoder1.update();
  //Serial.println(encoder1.read());
}

void updateEncoder2() {
  encoder2.update();
  //Serial.println(encoder2.read());
}

void i2c_interrupcion() {
  if (channel_received_i2c == 0){
    channel_received_i2c = Wire.read();
  } else if(byte1_received_message_i2c == 0) {
    byte1_received_message_i2c = Wire.read();
  }else{    
    uint8_t byte2_received_message_i2c = Wire.read(); // Leer el segundo byte recibido
    
    // Combinar los dos bytes en un entero de 16 bits
    received_message_i2c = (byte2_received_message_i2c << 8) | byte1_received_message_i2c;
    byte1_received_message_i2c = 0;
    i2c_interrupt = true;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), updateEncoder1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), updateEncoder1, CHANGE);

  attachInterrupt(digitalPinToInterrupt(18), updateEncoder2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(19), updateEncoder2, CHANGE);

  //attachInterrupt(digitalPinToInterrupt(SDA), i2c_interrupcion, RISING); // Seria para un arduino master no esclavo.
  Wire.begin(0x04);
  Wire.onReceive(i2c_interrupcion);

}

void loop() {
  // Distancia sensor ultrasonido.
  //int distancia1 = ultrasoundsensor1.ping_blocking();
  //int distancia2 = ultrasoundsensor2.ping_blocking();

  //Serial.print(distancia1);
  if (i2c_interrupt){
    I2C().i2cInteruptions(channel_received_i2c, received_message_i2c);
    channel_received_i2c = 0;
    byte1_received_message_i2c = 0;
    received_message_i2c = 0;
    i2c_interrupt = false;
  }

}
