int pin_trigger1 = 9;
int pin_echo1 = 7;

int pin_trigger2 = 2;
int pin_echo2 = 4;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);//Inicializamos el led incorporado en la propia placa como una salida  
  Serial.begin(9600);
  pinMode(pin_trigger1, OUTPUT);
  pinMode(pin_echo1, INPUT);
  pinMode(pin_trigger2, OUTPUT);
  pinMode(pin_echo2, INPUT);
}

void loop() {
  digitalWrite(pin_trigger1, LOW);
  digitalWrite(pin_trigger2, LOW); 
  delayMicroseconds(4);
  digitalWrite(pin_trigger1, HIGH);
  digitalWrite(pin_trigger2, HIGH); //generamos Trigger (disparo) de 10us
  
  delayMicroseconds(10);

  digitalWrite(pin_trigger1, LOW);
  long duration1 = pulseIn(pin_echo1, HIGH);
  digitalWrite(pin_trigger2, LOW);
  long duration2 = pulseIn(pin_echo2, HIGH); //medimos el tiempo entre pulsos, en microsegundos
  
  long distanceCm1 = duration1 * 10 / 292/ 2;//convertimos a distancia, en cm
  long distanceCm2 = duration2 * 10 / 292/ 2;
  Serial.print("Distance Sensor 1: ");
  Serial.println(distanceCm1);
  Serial.print("Distance Sensor 2: ");
  Serial.println(distanceCm2);
  delay(1000);
}