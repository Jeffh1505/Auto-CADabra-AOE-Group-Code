#include <Servo.h>
#include <ezButton.h>

const int limitSwitchPin = 7;
const int servoPin = 9;
Servo servo;
ezButton limitSwitch(limitSwitchPin);

enum LimitSwitchState { UP,
                        DOWN };
enum ServoPosition { MOLE_UP = 180,
                     MOLE_DOWN = 10 };
//25
LimitSwitchState limitSwitchState = DOWN;

bool startedPrinted = false;
bool hitScorePrinted = false;
bool endedPrinted = false;

constexpr unsigned long gameDuration = 60000;  
constexpr int HIT_DELAY = 1500;

unsigned long previousMillis = 0;
unsigned long hitTime = 0;
unsigned long moleHitStartTime = 0;
bool moleHitDelay = false;
int moleStayTime = 800;  // Start with a higher value

unsigned long gameStartTime;

void setup() {
  limitSwitch.setDebounceTime(50);
  servo.write(MOLE_DOWN);
  servo.attach(servoPin);

  Serial.begin(9600);
  gameStartTime = millis();
}

void loop() {
  unsigned long currentMillis = millis();
  if ((currentMillis - gameStartTime) <= gameDuration) {  // Ensure game conditions only occur during set game duration
    const uint8_t randomChance = random(0, 10);
    const int randomStayTime = random(1000, 2000);
    
    limitSwitch.loop();
    const uint8_t switchState = limitSwitch.getState();
    limitSwitchState = (switchState == HIGH) ? UP : DOWN;

    if (!startedPrinted) {
      Serial.println("Start");
      startedPrinted = true;
    }

    if (limitSwitchState == UP) {  // Mole is Up (Waiting to be Hit)
      if (hitScorePrinted) {
        hitScorePrinted = false;
      }
      
      

    } else if (limitSwitchState == DOWN) {  // Mole is Down (Hit)
      if (!moleHitDelay) {
        servo.write(MOLE_DOWN);  // Ensure we're not checking a servo that is currently in the hit delay (prevents false positives)
        hitTime = currentMillis;
        moleHitStartTime = currentMillis;
        moleHitDelay = true;
        if (limitSwitchState == DOWN && !hitScorePrinted) {
          Serial.println("harvard");
          hitScorePrinted = true;
        }
      }
    }

    if (moleHitDelay && (currentMillis - moleHitStartTime >= HIT_DELAY)) {  // Make sure the moles that are hit come back after the delay
      servo.write(MOLE_UP);
      moleHitDelay = false;
    }

    // Gradually decrease the moleStayTime to increase the game speed over 1 minute
    if (moleStayTime > 200) {  
      moleStayTime -= 25;  // Decrease moleStayTime gradually
    }

  } else if ((currentMillis - gameStartTime) >= gameDuration) {
    if (!endedPrinted) {
      Serial.println("End");
      endedPrinted = true;
    }
    delay(1000);
    exit(0);
  }
  delay(100);
}
