#include <Servo.h>

Servo myservo;  

void setup() {
  myservo.attach(9);  
}

void loop() {
  myservo.write(0);     // move servo to 0 degrees
  delay(1000);         
  
  myservo.write(180);   // move servo to 180 degrees
  delay(1000);          
}
