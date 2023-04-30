#include <Wire.h>

#define SLAVE_ADDRESS 0x04

int data = 0;

void setup() {
  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
}

void loop() {
  delay(100);
}

void requestEvent() {
  Wire.write(data);
}

void receiveEvent(int byteCount) {
  while (Wire.available()) {
    data = Wire.read();
    Serial.println(data);
  }
}
