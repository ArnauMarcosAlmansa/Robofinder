#include "Encoder.h"
#include "Motor.h"
#include "UltrasoundSensor.h"
#include <Wire.h>
#include <string.h>
#include <stdio.h>


Encoder encoderLeft(19,18);
Encoder encoderRight(3,2);

UltrasoundSensor ultrasoundsensorLeft(22, 23);
UltrasoundSensor ultrasoundsensorRight(24, 25);

Motor motorLeft(13, 38, 39);
Motor motorRight(12, 40, 41);

int encoder_left_final_position = -1;
int encoder_right_final_position = -1;

// Numero de Bytes que tendran los mensajes por canal. El canal es el indice de la lista.
int bytes_messages_i2c[40] = {0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

#include "i2c.h"


bool i2c_interrupt = false;             // Hacemos la condicion dentro del loop al tener el mensaje completo
int channel_received_i2c = 0;           // El canal que especifica el usuario que quiere usar. (Es el primer mensaje de 1Byte que recibimos).
uint8_t byte1_received_message_i2c = 0; // El primer byte del valor que recibimos.
uint16_t received_message_i2c = 0;      // El valor final del mensaje (2Bytes).
int response_count = 0;
int repsonse_channel = 0;
int response_i2c = 0;

int message_count = 0;
int total_number_message = -1;
int message_i2c[4] = {0,0,0,0};



void updateEncoderLeft() {
  encoderLeft.update();

  // Comprobamos si el motor ha llegado a la posicion que queremos.
  if (encoderLeft.read() == encoder_left_final_position)
  {
    motorLeft.stop(); // Paramos el motor
    encoder_left_final_position = -1; // Reiniciamos la variable con un valor no valido.
  }
  //Serial.println(encoderLeft.read());
}

void updateEncoderRight() {
  encoderRight.update();

  // Comprobamos si el motor ha llegado a la posicion que queremos.
  if (encoderRight.read() == encoder_right_final_position)
  {
    motorLeft.stop(); // Paramos el motor
    encoder_left_final_position = -1; // Reiniciamos la variable con un valor no valido.
  }
  //Serial.println(encoderRight.read());
}

void i2c_interrupcion() {
  // Comprobamos si tenemos un canal guardado. Sino tenemos ninguno, el mensaje sera el canal.
  if (channel_received_i2c == 0){
    channel_received_i2c = Wire.read();                                 // Leemos el valor y lo guardamos.
    total_number_message = bytes_messages_i2c[channel_received_i2c];    // Miramos el numero de bytes de los mensajes que tendremos para el canal recibido.
  } else {                                              // Si tenemos el canal guardado. Estamos recibindo el mensaje. 
    bytes_messages_i2c[message_count] = Wire.read();    // Guardamos el mensaje en la array de bytes.
    message_count++;                                    // Sumamos 1 al contador.
    if (message_count == total_number_message) {        // Si el contador de mensajes recibidos es igual al numero de mensajes para el canal que hemos establecido. Marcamos la interupccion.
      i2c_interrupt = true;                             // Esta interupcion funciona sobre el propio main.
    }

  }
  /*
  
   if(byte1_received_message_i2c == 0) {
    byte1_received_message_i2c = Wire.read();
  }else{    
    uint8_t byte2_received_message_i2c = Wire.read(); // Leer el segundo byte recibido
    
    // Combinar los dos bytes en un entero de 16 bits
    received_message_i2c = (byte2_received_message_i2c << 8) | byte1_received_message_i2c;
    byte1_received_message_i2c = 0;
    i2c_interrupt = true;
  }
  */
}

void i2c_sendData() { 
  if (response_count == 0) {
     Wire.write(repsonse_channel);
     response_count = 1;
  } else if (response_count == 1) {
    byte byte1 = response_i2c & 0xFF; // obtiene los 8 bits menos significativos
    Wire.write(byte1);
    response_count = 2;
  } else if (response_count == 2){
    byte byte2 = (response_i2c >> 8) & 0xFF; // desplaza 8 bits hacia la derecha y obtiene los siguientes 8 bits
    Wire.write(byte2);
    response_count = 0;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(18), updateEncoderLeft, CHANGE);
  attachInterrupt(digitalPinToInterrupt(19), updateEncoderLeft, CHANGE);

  attachInterrupt(digitalPinToInterrupt(2), updateEncoderRight, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), updateEncoderRight, CHANGE);

  //attachInterrupt(digitalPinToInterrupt(SDA), i2c_interrupcion, RISING); // Seria para un arduino master no esclavo.
  Wire.begin(0x30);
  Wire.onReceive(i2c_interrupcion);
  Wire.onRequest(i2c_sendData);

}

void loop() {
  // Distancia sensor ultrasonido.
  //int distancia1 = ultrasoundsensor1.ping_blocking();
  //int distancia2 = ultrasoundsensor2.ping_blocking();

  //Serial.print(distancia1);
  if (i2c_interrupt){
    response_i2c = I2C().i2cInteruptions(channel_received_i2c, message_i2c);
    repsonse_channel = channel_received_i2c;
    channel_received_i2c = 0;
    /*byte1_received_message_i2c = 0;
    received_message_i2c = 0;*/
    
    for (int i=0; i<=4; i++)
      message_i2c[i] = 0;

    i2c_interrupt = false;
  }

}
