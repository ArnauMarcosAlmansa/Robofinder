#include "Encoder.h"
#include <Arduino.h>

void Encoder::reset()
{
  updates = 0;
  value = 0;
  correct_value = 0;
  last_encoded = 0;
}

void Encoder::update()
{
  int MSB = digitalRead(pin_a); //MSB = most significant bit
  int LSB = digitalRead(pin_b); //LSB = least significant bit
 
  int encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number
  int sum  = (last_encoded << 2) | encoded; //adding it to the previous encoded value
 
  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) value ++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) value --;
 
  last_encoded = encoded; //store this value for next time
  updates++;
}

long Encoder::read()
{
  return value / 4;
}
