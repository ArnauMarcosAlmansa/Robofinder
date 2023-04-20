
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
        digitalWrite(pin_enable, HIGH);
      }

    void stop();
    void forward();
    void backward();

    void write_speed(int speed);
    void accelerate(int increment);
    void deccelerate(int decrement);
}

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
}

void Motor::backward()
{
  digitalWrite(pin_in1, LOW);
  digitalWrite(pin_in2, HIGH);
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

void Motor::decelerate(int decrement)
{
  write_speed(speed - decrement);
}
