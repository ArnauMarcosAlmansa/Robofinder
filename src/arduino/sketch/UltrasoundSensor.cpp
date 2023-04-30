#include <Arduino.h>
#include "UltrasoundSensor.h"

long UltrasoundSensor::ping_blocking()
{
  digitalWrite(pin_trigger, LOW); 
  delayMicroseconds(4);
  digitalWrite(pin_trigger, HIGH); //generamos Trigger (disparo) de 10us
  
  delayMicroseconds(10);

  digitalWrite(pin_trigger, LOW);
  long duration = pulseIn(pin_echo, HIGH); //medimos el tiempo entre pulsos, en microsegundos
  
  long distanceCm = duration * 10 / 292/ 2;//convertimos a distancia, en cm
  
  return distanceCm;
}