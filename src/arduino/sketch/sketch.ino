#include "Encoder.h"
#include "Motor.h"

Encoder encoder1(2, 3);
Encoder encoder2(18, 19);
// Motor motor1(2, 3, 4);

void updateEncoder1() {
  encoder1.update();
  Serial.println(encoder1.read());
}

void updateEncoder2() {
  encoder2.update();
  Serial.println(encoder2.read());
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), updateEncoder1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), updateEncoder1, CHANGE);

  attachInterrupt(digitalPinToInterrupt(18), updateEncoder2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(19), updateEncoder2, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
