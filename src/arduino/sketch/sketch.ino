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

bool interrupcion_i2c = false;
int canal_recibido_i2c = 0;
int valor_recibido_i2c = 0;

void updateEncoder1() {
  encoder1.update();
  Serial.println(encoder1.read());
}

void updateEncoder2() {
  encoder2.update();
  Serial.println(encoder2.read());
}

void i2c_interrupcion() {
  if (canal_recibido_i2c == 0){
    canal_recibido_i2c = Wire.read();
  } else {
    valor_recibido_i2c = Wire.read();
    interrupcion_i2c = true;
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
  int distancia1 = ultrasoundsensor1.ping_blocking();
  int distancia2 = ultrasoundsensor2.ping_blocking();


  if (interrupcion_i2c) {
    Serial.print("Canal ");
    Serial.println(canal_recibido_i2c);

    Serial.print("Valor ");
    Serial.println(valor_recibido_i2c);

    switch (canal_recibido_i2c) {
      // Controladora Motor Izquierda
      case 10:
        // Velocidad.
        motorIzquierdo.write_speed(valor_recibido_i2c);
        break;
      case 11:
        // Sentido. 1 Para Forward i 2 para Backward.
        if (valor_recibido_i2c == 1)
          motorIzquierdo.forward();
        else if(valor_recibido_i2c == 2)
          motorIzquierdo.backward();
        break;
      case 12:
        // Stop
        motorIzquierdo.stop();
        break;
      case 13:
        //Aceleración
        motorIzquierdo.accelerate(valor_recibido_i2c);
        break;
      case 14:
        //Deceleración 
        motorIzquierdo.deccelerate(valor_recibido_i2c);
        break;

      // Controladora Motor Derecha.
      case 20:
        // Velocidad.
        motorDerecho.write_speed(valor_recibido_i2c);
        break;
      case 21:
        // Sentido. 1 Para Forward i 2 para Backward.
        if (valor_recibido_i2c == 1)
          motorDerecho.forward();
        else if(valor_recibido_i2c == 2)
          motorDerecho.backward();
        break;
      case 22:
        // Stop
        motorDerecho.stop();
        break;
      case 23:
        //Aceleración
        motorDerecho.accelerate(valor_recibido_i2c);
        break;
      case 24:
        //Deceleración 
        motorDerecho.deccelerate(valor_recibido_i2c);
        break;

    }
    
    // Reiniciamos las variables.
    canal_recibido_i2c = 0;
    valor_recibido_i2c = 0;
    interrupcion_i2c = false;
  }




  delay(100);
}
