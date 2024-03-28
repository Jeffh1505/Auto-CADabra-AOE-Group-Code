const int switchPin1 = 1;

void setup() {
  Serial.begin(9600);

  pinMode(switchPin1, INPUT);
  digitalWrite(switchPin1, HIGH);
  
}

void loop() {
  int state1 = digitalRead(switchPin1);
  Serial.print("Limit Switch");
  Serial.print(state1 == HIGH ? "Open" : "Closed");
  delay(100);
}
