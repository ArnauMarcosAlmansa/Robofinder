void i2cInteruptions(int channel, int option)
  {
  Serial.print("Canal ");
  Serial.println(channel);
  Serial.print("Valor ");
  Serial.println(option);

  switch (channel) {
      // Controladora Motor Izquierda
      case 10:
        // Velocidad.
        motorIzquierdo.write_speed(option);
        break;
      case 11:
        // Sentido. 1 Para Forward i 2 para Backward.
        if (option == 1)
          motorIzquierdo.forward();
        else if(option == 2)
          motorIzquierdo.backward();
        break;
      case 12:
        // Stop
        motorIzquierdo.stop();
        break;
      case 13:
        //Aceleración
        motorIzquierdo.accelerate(option);
        break;
      case 14:
        //Deceleración 
        motorIzquierdo.deccelerate(option);
        break;

      // Controladora Motor Derecha.
      case 20:
        // Velocidad.
        motorDerecho.write_speed(option);
        break;
      case 21:
        // Sentido. 1 Para Forward i 2 para Backward.
        if (option == 1)
          motorDerecho.forward();
        else if(option == 2)
          motorDerecho.backward();
        break;
      case 22:
        // Stop
        motorDerecho.stop();
        break;
      case 23:
        //Aceleración
        motorDerecho.accelerate(option);
        break;
      case 24:
        //Deceleración 
        motorDerecho.deccelerate(option);
        break;

    }
}