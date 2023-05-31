#include <Arduino.h>

class Encoder
{
  private:
    int pin_a = -1;
    int pin_b = -1;


    long value = 0;
    long correct_value = 0;

    int last_encoded = 0;
  public:
    Encoder(int pin_a, int pin_b): pin_a(pin_a), pin_b(pin_b) {
      reset();
      pinMode(pin_a, INPUT);
      pinMode(pin_b, INPUT);
    }
    void reset();
    void update();
    int16_t read();

    long updates = 0;
};
