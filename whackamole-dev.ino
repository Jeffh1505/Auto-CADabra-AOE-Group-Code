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

LimitSwitchState limitSwitchState = DOWN;

bool startedPrinted = false;
bool hitScorePrinted = false;
bool endedPrinted = false;

constexpr unsigned long gameDuration = 60000;
constexpr int HIT_DELAY = 1500;
constexpr unsigned long moleStayTimeMin = 500; // control how long the mole stays down. (WHEN HIT)
constexpr unsigned long moleStayTimeMax = 2000; // control how long the mole stays down.(WHEN HIT)
constexpr unsigned long moleWaitTimeMin = 2000; // control the wait time when the mole is up and has not been hit.
constexpr unsigned long moleWaitTimeMax = 3000; // control the wait time when the mole is up and has not been hit.
 
unsigned long previousMillis = 0;
unsigned long hitTime = 0;
unsigned long moleHitStartTime = 0;
bool moleHitDelay = false;
unsigned long moleStayTime = 800;
unsigned long moleWaitTime = 2000;
unsigned long moleUpStartTime = 0;
bool moleUpDelay = false;

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
  if ((currentMillis - gameStartTime) <= gameDuration) {
    limitSwitch.loop();
    const uint8_t switchState = limitSwitch.getState();
    limitSwitchState = (switchState == HIGH) ? UP : DOWN;

    if (!startedPrinted && !moleUpDelay) {
      Serial.println("Start");
      startedPrinted = true;
      moleUpStartTime = currentMillis;
      moleUpDelay = true;
    }

    if (moleUpDelay && (currentMillis - moleUpStartTime >= random(500, 2000))) {
      servo.write(MOLE_UP);
      moleWaitTime = random(moleWaitTimeMin, moleWaitTimeMax);
      moleUpDelay = false;
    }

    if (!moleUpDelay) {
      if (limitSwitchState == UP) {
        if (currentMillis - moleHitStartTime >= moleStayTime) {
          servo.write(MOLE_DOWN);
          moleHitStartTime = currentMillis;
          moleStayTime = random(moleStayTimeMin, moleStayTimeMax);
        }
      } else if (limitSwitchState == DOWN) {
        if (!moleHitDelay) {
          hitTime = currentMillis;
          moleHitStartTime = currentMillis;
          moleHitDelay = true;
          if (!hitScorePrinted) {
            Serial.println("harvard");
            hitScorePrinted = true;
          }
        }
      }

      if (moleHitDelay && (currentMillis - moleHitStartTime >= HIT_DELAY)) {
        servo.write(MOLE_UP);
        moleHitDelay = false;
        moleWaitTime = random(moleWaitTimeMin, moleWaitTimeMax);
      }

      if (currentMillis - hitTime >= moleStayTime && limitSwitchState == DOWN) {
        servo.write(MOLE_UP);
        moleWaitTime = random(moleWaitTimeMin, moleWaitTimeMax);
      }
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
