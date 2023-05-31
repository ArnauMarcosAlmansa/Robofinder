class I2C
{
  private:
    //int speed;

  public:
    I2C(){};
    int i2cInteruptions(int channel, uint8_t message[4]);

    void setDefaultSpeed(int speedLeft, int speedRight);
};

void I2C::setDefaultSpeed(int speedLeft, int speedRight)
{
  //this->speed = 100;
  motorLeft.write_speed(speedLeft);
  motorRight.write_speed(speedRight);
}

int I2C::i2cInteruptions(int channel, uint8_t message[4])
  {
  // Serial.print("Canal ");
  // Serial.println(channel);
  // Serial.print("Valor ");
  // Serial.println(message[0]);
  // Serial.println(message[1]);
  // Serial.println((message[3] << 8) | message[2]);

  uint16_t encoder_final_position = -1;

  switch (channel) {

    // 1X Motors:
    case 10:
      //Enable Motors
      setDefaultSpeed(100,100);
      // Serial.println("Enable Motors");
      break;

    case 11: // 4 Bytes
      //Forward

      // Combinar los dos bytes en un entero de 16 bits
      encoder_final_position = (message[3] << 8) | message[2];

      // Reset encoders.
      encoderLeft.reset();
      encoderRight.reset();

      // Guardamos los valores finales del encoder, para que pare el movimiento al llegar a ellos.
      encoder_left_final_position = encoder_final_position;
      encoder_right_final_position = 65536 - encoder_final_position;

      // Establecemos la direccion de los motore
      motorLeft.forward();
      motorRight.forward();
      
      setDefaultSpeed(message[0], message[1]);

      // Luz led indicadora.
      led.forward();

      // Serial.println("Forward");
      break;

    case 12: // 4 Bytes
      //Backward

      // Combinar los dos bytes en un entero de 16 bits
      encoder_final_position = (message[3] << 8) | message[2];

      // Reset encoders.
      encoderLeft.reset();
      encoderRight.reset();

      // Guardamos los valores finales del encoder, para que pare el movimiento al llegar a ellos.
      encoder_left_final_position = encoder_final_position;
      encoder_right_final_position = 65536 - encoder_final_position;

      // Establecemos la direccion de los motores
      motorLeft.backward();
      motorRight.backward();

      // Damos velocidad a los motores.
      setDefaultSpeed(message[0], message[1]);

      // Luz led indicadora.
      led.backward();

      // Serial.println("Backward");
      break;

    case 13: // 4 Bytes
      //Left

      // Combinar los dos bytes en un entero de 16 bits
      encoder_final_position = (message[3] << 8) | message[2];

      // Reset encoders.
      encoderLeft.reset();
      encoderRight.reset();

      // Guardamos los valores finales del encoder, para que pare el movimiento al llegar a ellos.
      encoder_left_final_position = encoder_final_position;
      encoder_right_final_position = 65536 - encoder_final_position;
      
      // Establecemos la direccion de los motores
      motorLeft.forward();
      motorRight.backward();

      // Damos velocidad a los motores.
      setDefaultSpeed(message[0], message[1]);

      // Luz led indicadora.
      led.arrowLeft();

      // Serial.println("Left");
      break;

    case 14: // 4 Bytes
      //Right

      // Combinar los dos bytes en un entero de 16 bits
      encoder_final_position = (message[3] << 8) | message[2];

      // Reset encoders.
      encoderLeft.reset();
      encoderRight.reset();

      // Guardamos los valores finales del encoder, para que pare el movimiento al llegar a ellos.
      encoder_left_final_position = encoder_final_position;
      encoder_right_final_position = 65536 - encoder_final_position;

      // Establecemos la direccion de los motores
      motorLeft.backward();
      motorRight.forward();

      // Damos velocidad a los motores.
      setDefaultSpeed(message[0], message[1]);

      // Luz led indicadora.
      led.arrowRight();

      // Serial.println("Right");
      break;

    case 15:
      // Stop
      motorLeft.stop();
      motorRight.stop();

      // Luz led indicadora.
      led.stop();

      // Serial.println("Stop");
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



    case 22:
      // Reset Encoder Left
      encoderLeft.reset();
      break;

    case 23:
      // Reset Encoder Right
      encoderRight.reset();
      break;      


    // 3X Ultrasound:
    case 30:
      // Ultrasound Left
      return ultrasoundsensorLeft.ping_blocking();
      break;

    case 31:
      // Ultrasound Right
      return ultrasoundsensorRight.ping_blocking();
      break;


    case 40:
    // LED OFF.
      led.off();
      break;


    }
    return 0;
}