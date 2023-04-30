#include "Encoder.h"
#include "Motor.h"
#include "UltrasoundSensor.h"
#include <Wire.h>

Encoder encoder1(2, 3);
Encoder encoder2(18, 19);

UltrasoundSensor ultrasoundsensor1(10,11);
UltrasoundSensor ultrasoundsensor2(12,13);
Motor motorIzquierdo(2, 3, 4);
Motor motorDerecho(2, 3, 4);

volatile bool interrupcion_i2c = false;

void updateEncoder1() {
  encoder1.update();
  Serial.println(encoder1.read());
}

void updateEncoder2() {
  encoder2.update();
  Serial.println(encoder2.read());
}

void i2c_interrupcion() {
  interrupcion_i2c = true;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), updateEncoder1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), updateEncoder1, CHANGE);

  attachInterrupt(digitalPinToInterrupt(18), updateEncoder2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(19), updateEncoder2, CHANGE);

  //attachInterrupt(digitalPinToInterrupt(SDA), i2c_interrupcion, RISING); // Seria para un arduino master no esclavo.
  Wire.onReceive(i2c_interrupcion);

}

void loop() {
  // Distancia sensor ultrasonido.
  int distancia1 = ultrasoundsensor1.ping_blocking();
  int distancia2 = ultrasoundsensor2.ping_blocking();


  if (interrupcion_i2c) {
    byte canal;
    int valor;
    Wire.requestFrom(10, 2); // Leer 2 bytes del dispositivo. Direccion 10.
    if(Wire.available() >= 2) {
      byte canal = Wire.read();
      int valor = Wire.read();
    }

    switch (canal) {
      // Controladora Motor Izquierda
      case 10:
        // Velocidad.
        motorIzquierdo.write_speed(valor);
        break;
      case 11:
        // Sentido. 1 Para Forward i 2 para Backward.
        if (valor == 1)
          motorIzquierdo.forward();
        else if(valor == 2)
          motorIzquierdo.backward();
        break;
      case 12:
        // Stop
        motorIzquierdo.stop();
        break;
      case 13:
        //Aceleración
        motorIzquierdo.accelerate(valor);
        break;
      case 14:
        //Deceleración 
        motorIzquierdo.deccelerate(valor);
        break;

      // Controladora Motor Derecha.
      case 20:
        // Velocidad.
        motorDerecho.write_speed(valor);
        break;
      case 21:
        // Sentido. 1 Para Forward i 2 para Backward.
        if (valor == 1)
          motorDerecho.forward();
        else if(valor == 2)
          motorDerecho.backward();
        break;
      case 22:
        // Stop
        motorDerecho.stop();
        break;
      case 23:
        //Aceleración
        motorDerecho.accelerate(valor);
        break;
      case 24:
        //Deceleración 
        motorDerecho.deccelerate(valor);
        break;

    }
    
    interrupcion_i2c = false;
  }




  delay(20);
}
