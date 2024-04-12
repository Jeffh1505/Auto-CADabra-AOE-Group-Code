#include <Servo.h>
#include <ezButton.h>

const int limitSwitchPins[] = { 7, 8, 9, 10, 11 };
const int servoPins[] = { 9, 10, 11, 12, 13 };
Servo servos[5];
ezButton limitSwitches[5] = { ezButton(limitSwitchPins[0]), ezButton(limitSwitchPins[1]), ezButton(limitSwitchPins[2]), ezButton(limitSwitchPins[3]), ezButton(limitSwitchPins[4]) };

enum LimitSwitchState {
  UP,
  DOWN
};

LimitSwitchState limitSwitchStates[5] = { DOWN, DOWN, DOWN, DOWN, DOWN };

bool startedPrinted = false;
bool hitScorePrinted = false;
bool endedPrinted = false;
bool startUpDelay[5] = { true, true, true, true, true };

constexpr unsigned long gameDuration = 45000;
unsigned long gameStartTime;
unsigned long randomMole = random(0, 5);

struct Mole {
  int MOLE_DOWN;
  unsigned long moleHitStartTime;
  bool moleHitDelay;
  bool startRandomization;
  bool Randomized;
  unsigned long randomizationStart;
  unsigned long hitTime;
  unsigned long moleStayTimeMin;
  unsigned long moleStayTimeMax;
  unsigned long HIT_DELAY;
};

Mole moles[5] = {
  { 25, 0, false, false, false, 0, 0, 3000, 5000, 1500 },
  { 10, 0, false, false, false, 0, 0, 2000, 4000, 1000 },
  { 0, 0, false, false, false, 0, 0, 3000, 5000, 950 },
  { 0, 0, false, false, false, 0, 0, 2000, 4000, 2000 },
  { 25, 0, false, false, false, 0, 0, 1000, 6000, 1750 }
};

const char* comboNames[5] = { "Harvard", "Yale", "Princeton", "Brown", "UPenn" };

void setup() {
  for (int i = 0; i < 5; i++) {
    limitSwitches[i].setDebounceTime(50);
    servos[i].write(moles[i].MOLE_DOWN);
    servos[i].attach(servoPins[i]);
  }

  Serial.begin(9600);
}

void loop() {
  if (!startedPrinted) {
    limitSwitches[randomMole].loop();
    const uint8_t switchState = limitSwitches[randomMole].getState();
    limitSwitchStates[randomMole] = (switchState == HIGH) ? UP : DOWN;

    if (limitSwitchStates[randomMole] == DOWN) {
      servos[randomMole].write(moles[randomMole].MOLE_DOWN);
      Serial.println("Start");
      Serial.println(comboNames[randomMole]);
      startedPrinted = true;
      gameStartTime = millis();
    }
  } else {
    unsigned long currentMillis = millis();
    if ((currentMillis - gameStartTime) <= gameDuration) {
      for (int i = 0; i < 5; i++) {
        limitSwitches[i].loop();
        const uint8_t switchState = limitSwitches[i].getState();
        limitSwitchStates[i] = (switchState == HIGH) ? UP : DOWN;

        if (limitSwitchStates[i] == UP) {
          if (startUpDelay[i] && (currentMillis - gameStartTime >= random(500, 2000))) {
            servos[i].write(180);
            startUpDelay[i] = false;
            moles[i].startRandomization = true;
          }

          if (!moles[i].moleHitDelay && moles[i].startRandomization && !moles[i].Randomized) {
            if (currentMillis - moles[i].randomizationStart >= random(moles[i].moleStayTimeMin, moles[i].moleStayTimeMax)) {
              servos[i].write(moles[i].MOLE_DOWN);
              moles[i].randomizationStart = currentMillis;
              moles[i].Randomized = true;
            }
          } else if (moles[i].Randomized) {
            if (currentMillis - moles[i].randomizationStart >= moles[i].HIT_DELAY) {
              servos[i].write(180);
              moles[i].Randomized = false;
            }
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
            moles[i].startRandomization = false;
            moles[i].Randomized = false;
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
          moles[i].startRandomization = true;
        }
      }
    } else {
      if (!endedPrinted) {
        Serial.println("End");
        endedPrinted = true;
        Serial.println("Restart")
        delay(10000);
        startedPrinted = false;
        hitScorePrinted = false;
        endedPrinted = false;
        randomMole = random(0, 5);
        for (int i = 0; i < 5; i++) {
          LimitSwitchStates[i] = DOWN;
          startUpDelay[i] = true;
        }
      }
    }
  }
  delay(100);
}
