#include <Servo.h>
#include <ezButton.h>

const int limitSwitchPin = 8;
ezButton limitSwitch(limitSwitchPin);

enum LimitSwitchState { UP,
                        DOWN };

LimitSwitchState limitSwitchState = DOWN;

bool startedPrinted = false;
bool hitScorePrinted = false;
bool endedPrinted = false;

constexpr unsigned long gameDuration = 90000;
constexpr int HIT_DELAY = 500;

unsigned long previousMillis = 0;
unsigned long hitTime = 0;
unsigned long moleHitStartTime = 0;
bool moleHitDelay = false;
int moleStayTime = 800;

unsigned long gameStartTime;
int score = 0;

void setup() {
  limitSwitch.setDebounceTime(50);

  Serial.begin(9600);
  unsigned long startTime = millis();

  while (millis() - startTime < 10000) {  // Wait 10 seconds
    // Do nothing
  }

  if (!startedPrinted) {
    Serial.println("Start");
    startedPrinted = true;
  }
  gameStartTime = millis();
}

void loop() {
  unsigned long currentMillis = millis();
  if ((currentMillis - gameStartTime) <= gameDuration) {  // Ensure game conditions only occur during set game duration
    const unsigned long gameSpeed = 800 - ((currentMillis - gameStartTime) / 1000) * 10;
    const uint8_t randomChance = random(0, 10);
    const int randomStayTime = random(500, 800);

    limitSwitch.loop();
    const uint8_t switchState = limitSwitch.getState();
    limitSwitchState = (switchState == HIGH) ? UP : DOWN;

    if (limitSwitchState == UP) {  // Mole is Up (Waiting to be Hit)
      if (hitScorePrinted) {
        hitScorePrinted = false;
      }
      if (randomChance < 3) {  // 30% chance of it going back down
        previousMillis = currentMillis;
      }
      if (currentMillis - previousMillis >= moleStayTime) {  // Comes back up again after staying down for a random amount of time
        moleStayTime = randomStayTime;                       //Give it another random for next time
      }
    } else if (limitSwitchState == DOWN) {  // Mole is Down (Hit)
      if (!moleHitDelay) {                  //Ensure we're not checking a servo that is currently in the hit delay (prevents false positives)
        hitTime = currentMillis;
        score++;

        moleHitStartTime = currentMillis;
        moleHitDelay = true;
        if (limitSwitchState == DOWN && !hitScorePrinted) {
          Serial.println("harvard");
          hitScorePrinted = true;
        }
      }
    }

    if (moleHitDelay && (currentMillis - moleHitStartTime >= HIT_DELAY)) {  //Make sure the moles that are hit come back after the delay
      moleHitDelay = false;
    }

    if (moleStayTime > 100) {  //Make game go zoooooom
      moleStayTime = gameSpeed;
      if (moleStayTime < 100) {
        moleStayTime = 100;
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
