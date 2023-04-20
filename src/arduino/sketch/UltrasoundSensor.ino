
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
}

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

