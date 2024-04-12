#include <Servo.h>
#include <ezButton.h>

const int limitSwitchPins[] = {7};
const int servoPins[] = {9};
Servo servos[1];
ezButton limitSwitches[1] = {ezButton(limitSwitchPins[0])};

enum LimitSwitchState {
  UP,
  DOWN
};

LimitSwitchState limitSwitchStates[1] = {DOWN};

bool startedPrinted = false;
bool hitScorePrinted = false;
bool endedPrinted = false;
bool startUpDelay[1] = {true};

constexpr unsigned long gameDuration = 45000;  
unsigned long gameStartTime;

struct Mole {
  int MOLE_DOWN;
  unsigned long moleHitStartTime;
  bool moleHitDelay;
  unsigned long hitTime;
  unsigned long moleStayTime;
  unsigned long HIT_DELAY;
};

Mole moles[1] = {{25, 0, false, 0, 800, 0}};

const char* comboNames[1] = {"Harvard"};

void setup() {
  for (int i = 0; i < 1; i++) {
    limitSwitches[i].setDebounceTime(50);
    servos[i].write(moles[i].MOLE_DOWN);
    servos[i].attach(servoPins[i]);
  }

  Serial.begin(9600);
  gameStartTime = millis();
}

void loop() {
  unsigned long currentMillis = millis();
  
  if ((currentMillis - gameStartTime) <= gameDuration) {
    for (int i = 0; i < 1; i++) {
      limitSwitches[i].loop();
      const uint8_t switchState = limitSwitches[i].getState();
      limitSwitchStates[i] = (switchState == HIGH) ? UP : DOWN;

      if (!startedPrinted) {
        Serial.println("Start");
        startedPrinted = true;
      }

      if (limitSwitchStates[i] == UP) {
        if (startUpDelay[i] && (currentMillis - gameStartTime >= random(500, 2000))) {
          servos[i].write(180);
          startUpDelay[i] = false;
        }

        if (hitScorePrinted) {
          hitScorePrinted = false;
        }
      } else if (limitSwitchStates[i] == DOWN) {
        if (!moles[i].moleHitDelay) {
          servos[i].write(moles[i].MOLE_DOWN);
          moles[i].hitTime = currentMillis;
          moles[i].moleHitStartTime = currentMillis;
          moles[i].moleHitDelay = true;
          if (!hitScorePrinted) {
            Serial.println(comboNames[i]);
            hitScorePrinted = true;
          }
          moles[i].HIT_DELAY = random(1000, 2000);
        }
      }

      if (moles[i].moleHitDelay && (currentMillis - moles[i].moleHitStartTime >= moles[i].HIT_DELAY)) {
        servos[i].write(180);
        moles[i].moleHitDelay = false;
      }

      if (moles[i].moleStayTime > 200) {
        moles[i].moleStayTime -= 25;
      }
    }
  } else {
    if (!endedPrinted) {
      Serial.println("End");
      endedPrinted = true;
    }
    delay(1000);
    exit(0);
  }
  delay(100);
}
