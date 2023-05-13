#include "Motor.h"
#include <Arduino.h>

void Motor::stop()
{
  speed = 0;
  digitalWrite(pin_in1, LOW);
  digitalWrite(pin_in2, LOW);
  analogWrite(pin_enable, 0);
}

void Motor::forward()
{
  digitalWrite(pin_in1, HIGH);
  digitalWrite(pin_in2, LOW);
  analogWrite(pin_enable, speed);
}

void Motor::backward()
{
  digitalWrite(pin_in1, LOW);
  digitalWrite(pin_in2, HIGH);
  analogWrite(pin_enable, speed);
}

void Motor::write_speed(int speed)
{
  this->speed = speed;
  analogWrite(pin_enable, speed);
}

void Motor::accelerate(int increment)
{
  write_speed(speed + increment);
}

void Motor::deccelerate(int decrement)
{
  write_speed(speed - decrement);
}
