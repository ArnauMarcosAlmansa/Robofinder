#include <Arduino.h>

class UltrasoundSensor
{
  private:
    int pin_trigger;
    int pin_echo;

  public:
    UltrasoundSensor(int pin_trigger, int pin_echo):
      pin_trigger(pin_trigger), pin_echo(pin_echo)
      {
        pinMode(pin_trigger, OUTPUT);
        pinMode(pin_echo, INPUT);
      }

    long ping_blocking();
};
