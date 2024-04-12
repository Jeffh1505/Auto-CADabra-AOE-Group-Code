#include <ezButton.h>

ezButton startButton(PinNumberHere);  
void setup() {
  Serial.begin(9600);
  startButton.setDebounceTime(50); 
}
void loop() {
  startButton.loop();
  
  if(startButton.isPressed())
    Serial.println("The startButton: UNTOUCHED -> TOUCHED");
  
  if(startButton.isReleased())
    Serial.println("The startButton: TOUCHED -> UNTOUCHED");
  
  int state = startButton.getState();
  if(startButton == HIGH)
    Serial.println("The startButton: UNTOUCHED");
  else
    Serial.println("The startButton: TOUCHED");
}
