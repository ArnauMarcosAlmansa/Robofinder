class I2C
{
  private:
    int speed;

  public:
    I2C():speed(0){};
    void i2cInteruptions(int channel, int option);

    void setDefaultSpeed();
};

void I2C::setDefaultSpeed()
{
  motorLeft.write_speed(this->speed);
  motorRight.write_speed(this->speed);
}


void I2C::i2cInteruptions(int channel, int option)
  {
  Serial.print("Canal ");
  Serial.println(channel);
  Serial.print("Valor ");
  Serial.println(option);

  switch (channel) {

    // 1X Motors:
    case 10:
      //Enable Motors
      this->speed = 100;
      setDefaultSpeed();
      Serial.println("Enable Motors");
      break;

    case 11:
      //Forward
      setDefaultSpeed();

      motorLeft.forward();
      motorRight.forward();

      Serial.println("Forward");
      break;

    case 12:
      //Backward
      setDefaultSpeed();

      motorLeft.backward();
      motorRight.backward();

      Serial.println("Backward");
      break;

    case 13:
      //Left
      motorLeft.write_speed(this->speed + 50);
      motorRight.write_speed(this->speed);

      Serial.println("Left");
      break;

    case 14:
      //Right
      motorLeft.write_speed(this->speed);
      motorRight.write_speed(this->speed + 50);

      Serial.println("Right");
      break;

    case 15:
      // Stop
      motorLeft.stop();
      motorRight.stop();

      Serial.println("Stop");
      break;


    // 2X Encoders:
    case 20:
      // Encoder Left

      break;

    case 21:
      // Encoder Right
      break;





    // 3X Ultrasound:
    case 30:
      // Ultrasound Left
      break;

    case 31:
      // Ultrasound Right
      break;





    }
}