#include <Arduino.h>

class Motor
{
  private:
    int pin_enable;
    int pin_in1;
    int pin_in2;

    int speed;

  public:
    Motor(int pin_enable, int pin_in1, int pin_in2): 
      pin_enable(pin_enable), pin_in1(pin_in1), pin_in2(pin_in2), speed(0)
      {
        pinMode(pin_enable, OUTPUT);
        pinMode(pin_in1, OUTPUT);
        pinMode(pin_in2, OUTPUT);
        digitalWrite(pin_in1, LOW);
        digitalWrite(pin_in2, LOW);
        digitalWrite(pin_enable, LOW);
      }

    void stop();
    void forward();
    void backward();

    void write_speed(int speed);
    void accelerate(int increment);
    void deccelerate(int decrement);
};
