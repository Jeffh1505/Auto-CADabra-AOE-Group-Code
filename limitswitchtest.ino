const int switchPin1 = 1;

void setup() {
  Serial.begin(9600);
  pinMode(switchPin1, INPUT);  
}

void loop() {
  int state1 = digitalRead(switchPin1);
  Serial.print("Limit Switch");
  Serial.print(state1 == HIGH ? "Activated" : "Idle");
  delay(100);
}
