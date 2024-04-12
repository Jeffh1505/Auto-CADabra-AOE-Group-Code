#include <Servo.h>

Servo myservo;  
bool moleUp = false;
bool moleDown = false; 

void setup() {
  myservo.write(0);
  myservo.attach(9); 
  moleDown = true; 
  Serial.begin(9600); 
}

void loop() {
  if(!moleUp) {
    Serial.println("Going to 180");
    myservo.write(180); 
    moleUp = true; 
    moleDown = false; 
    delay(1000);         
  }


  if(!moleDown) {
    Serial.println("Going to 0");
    myservo.write(0); 
    moleDown = true; 
    moleUp = false; 
    delay(1000); 
  }
  

}
