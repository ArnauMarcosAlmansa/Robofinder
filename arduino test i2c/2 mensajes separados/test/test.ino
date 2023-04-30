#include <Wire.h>

void setup() {
  // Iniciar la comunicaci�n I2C
  Wire.begin(0x04); // Direcci�n del dispositivo en la red I2C
  Wire.onReceive(receiveEvent); // Funci�n para manejar los datos recibidos
  Serial.begin(9600); // Iniciar la comunicaci�n serial para imprimir los datos recibidos
}

int canal = 0;
int valor = 0;
bool mensaje = false;

void loop() {
  // Nada que hacer aqu�
  if (mensaje){
    Serial.print("Canal: ");
    Serial.println(canal);
    Serial.print(" Valor: ");
    Serial.println(valor);

    canal = 0;
    valor = 0;
    mensaje = false;
  }
  
  delay(100);
}

void receiveEvent(int byteCount) {
  // Leer el valor enviado desde la Raspberry Pi
  if (canal == 0){
    canal = Wire.read();
  } else {
    valor = Wire.read();
    mensaje = true;
  }
}
