class I2C
{
  private:
    int speed;

  public:
    I2C():speed(0){};
    int i2cInteruptions(int channel, int option);

    void setDefaultSpeed();
};

void I2C::setDefaultSpeed()
{
  this->speed = 100;
  motorLeft.write_speed(this->speed);
  motorRight.write_speed(this->speed);
}


int I2C::i2cInteruptions(int channel, int option)
  {
  Serial.print("Canal ");
  Serial.println(channel);
  Serial.print("Valor ");
  Serial.println(option);

  switch (channel) {

    // 1X Motors:
    case 10:
      //Enable Motors
      setDefaultSpeed();
      Serial.println("Enable Motors");
      break;

    case 11:
      //Forward
      motorLeft.forward();
      motorRight.forward();
      
      setDefaultSpeed();

      Serial.println("Forward");
      break;

    case 12:
      //Backward

      motorLeft.backward();
      motorRight.backward();

      setDefaultSpeed();

      Serial.println("Backward");
      break;

    case 13:
      //Left
      motorLeft.forward();
      motorRight.backward();

      setDefaultSpeed();

      Serial.println("Left");
      break;

    case 14:
      //Right
      motorLeft.backward();
      motorRight.forward();

      setDefaultSpeed();

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
      return encoderLeft.read();
      break;

    case 21:
      // Encoder Right
      return (65536 - encoderRight.read());
      break;





    // 3X Ultrasound:
    case 30:
      // Ultrasound Left
      break;

    case 31:
      // Ultrasound Right
      break;


    }
    return 0;
}