#include <ezButton.h>

ezButton limitSwitch(PinNumberHere);  
void setup() {
  Serial.begin(9600);
  limitSwitch.setDebounceTime(50); 
}
void loop() {
  limitSwitch.loop();
  
  if(limitSwitch.isPressed())
    Serial.println("The limit switch: UNTOUCHED -> TOUCHED");
  
  if(limitSwitch.isReleased())
    Serial.println("The limit switch: TOUCHED -> UNTOUCHED");
  
  int state = limitSwitch.getState();
  if(state == HIGH)
    Serial.println("The limit switch: UNTOUCHED");
  else
    Serial.println("The limit switch: TOUCHED");
}
