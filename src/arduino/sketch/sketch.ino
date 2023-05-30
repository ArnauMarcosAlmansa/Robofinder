#include "Encoder.h"
#include "Motor.h"
#include "UltrasoundSensor.h"
#include "led.h"
#include <Wire.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


Encoder encoderLeft(19,18);
Encoder encoderRight(3,2);

UltrasoundSensor ultrasoundsensorLeft(24, 25);
UltrasoundSensor ultrasoundsensorRight(22, 23);

Motor motorLeft(13, 38, 39);
Motor motorRight(12, 40, 41);

Led led(6, 256);  // Configurar el objeto Led con el pin y el número de LEDs


int encoder_left_final_position = -1;
int encoder_right_final_position = -1;

int bytes_messages_i2c[40] = {
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  4,
  4,
  4,
  4,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0
  }; // Numero de Bytes que tendran los mensajes por canal (Numero -1). El canal es el indice de la lista.

#include "i2c.h"


bool i2c_interrupt = false;             // Hacemos la condicion dentro del loop al tener el mensaje completo
int channel_received_i2c = 0;           // El canal que especifica el usuario que quiere usar. (Es el primer mensaje de 1Byte que recibimos).

int response_count = 0;                 // Temporal. Numero de mensajes enviados a la Jetson.
int repsonse_channel = 0;               // Canal con el enviamos los mensajes a la Jetson.
int response_i2c = 0;                   // Valor que devolvemos por I2C a la Jetson.

int message_count = 0;                  // Temporal. Cuenta los mensajes que hemos recibido.
int total_number_message = 0;           // Temporal. Guardamos el numero total de mensajes que tienen que llegar para el canal seleccionado.
uint8_t message_i2c[4] = {0,0,0,0};     // Mensajes de recibimos de 1Bytes cada uno.

int count_loop = 0;                     // Contador de iteraciones en el loop. Para realizar la medicion de los ultrasonidos.
int ultrasonic_limit = 10;              // Limite de distancia en el que los ultrasonidos pararan el movimineto del robot.

void updateEncoderLeft() {
  encoderLeft.update();

  // Comprobamos si el motor ha llegado a la posicion que queremos.
  if (abs(encoderLeft.read()) >= abs(encoder_left_final_position)) {
    motorLeft.stop(); // Paramos el motor
    encoder_left_final_position = 0; // Reiniciamos la variable con un valor no valido.
    //led.off();
    led.ide();
  }
}

void updateEncoderRight() {
  encoderRight.update();

  // Comprobamos si el motor ha llegado a la posicion que queremos.
  if (abs(encoderRight.read()) >= abs(encoder_right_final_position)) {
    motorRight.stop(); // Paramos el motor
    encoder_left_final_position = 0; // Reiniciamos la variable con un valor no valido.
    //led.off();
    led.ide();
  }
}

void i2c_interrupcion() {
  Serial.println("i2c_interrupcion START");
  Serial.print("AVAILABLE BEGINNING: ");
  Serial.println(Wire.available());
  // Comprobamos si tenemos un canal guardado. Sino tenemos ninguno, el mensaje sera el canal.

  channel_received_i2c = Wire.read();                                 // Leemos el valor y lo guardamos.
  total_number_message = bytes_messages_i2c[channel_received_i2c];    // Miramos el numero de bytes de los mensajes que tendremos para el canal recibido.
  
  message_count = 0;
  while (message_count < total_number_message) {
    message_i2c[message_count] = Wire.read();
    message_count++;   
  }

  response_i2c = I2C().i2cInteruptions(channel_received_i2c, message_i2c);
  repsonse_channel = channel_received_i2c;

  for (int i=0; i<= 4; i++) {
    message_i2c[i] = 0;
  }
  
  total_number_message = 0;
  message_count = 0;
  Serial.print("AVAILABLE END: ");
  Serial.println(Wire.available());
  Serial.println("i2c_interrupcion END");
}

void i2c_sendData() { 
  if (response_count == 0) {
     Wire.write(repsonse_channel);
     response_count = 1;
  } else if (response_count == 1) {
    byte byte1 = response_i2c & 0xFF; // obtiene los 8 bits menos significativos
    Wire.write(byte1);
    response_count = 2;
  } else if (response_count == 2) {
    byte byte2 = (response_i2c >> 8) & 0xFF; // desplaza 8 bits hacia la derecha y obtiene los siguientes 8 bits
    Wire.write(byte2);
    response_count = 0;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("BEGIN");
  attachInterrupt(digitalPinToInterrupt(18), updateEncoderLeft, CHANGE);
  attachInterrupt(digitalPinToInterrupt(19), updateEncoderLeft, CHANGE);

  attachInterrupt(digitalPinToInterrupt(2), updateEncoderRight, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), updateEncoderRight, CHANGE);

  //attachInterrupt(digitalPinToInterrupt(SDA), i2c_interrupcion, RISING); // Seria para un arduino master no esclavo.
  Wire.begin(0x30);
  Wire.onReceive(i2c_interrupcion);
  Wire.onRequest(i2c_sendData);

  led.begin();  // Iniciar los LEDs NeoPixel
  led.startup();
}

void loop() {

  // Serial.println("LOOP START");
  delay(1000);
  // Serial.println("LOOP END");
  
  /*
  led.stop();  // Mostrar color rojo
  delay(2000);
  
  led.forward();  // Mostrar color verde
  delay(2000);

  led.backward();
  delay(2000);

  led.ide();
*/
  //delay(2000);
  //delay(2000);*/
  /*
  led.arrowLeft();
  led.arrowRight();
*/  
  //led.ide();

  /*

  if (count_loop > 50){

    if ((ultrasoundsensorLeft.ping_blocking() <= ultrasonic_limit) || (ultrasoundsensorRight.ping_blocking() <= ultrasonic_limit)) { // Comprobamos si hemos llegado a una distancia limite.
      motorLeft.stop();
      motorRight.stop();
      led.stop();
    }

    count_loop = 0;
  }

  count_loop += 1;
  */
}
