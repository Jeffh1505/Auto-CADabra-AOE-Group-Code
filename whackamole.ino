#include <Servo.h>

const int limitSwitchPins[] = {1, 2, 3, 4, 5};
const int servoPins[] = {6, 7, 8, 9, 10};
Servo servos[5];

const enum LimitSwitchState { UP, DOWN };
const enum ServoPosition { MOLE_UP = 180, MOLE_DOWN = 0 };

LimitSwitchState limitSwitchStates[5] = {DOWN, DOWN, DOWN, DOWN, DOWN};

constexpr unsigned long gameDuration = 90000;
constexpr int HIT_DELAY = 500;

unsigned long previousMillis[5] = {0, 0, 0, 0, 0};
unsigned long hitTime[5] = {0, 0, 0, 0, 0};
unsigned long moleHitStartTime[5] = {0, 0, 0, 0, 0};
bool moleHitDelay[5] = {false, false, false, false, false};
int moleStayTime[5] = {800, 800, 800, 800, 800};

int servoMoveTime = 15;
unsigned long gameStartTime;
int score = 0;

void setup() {
  for (int i = 0; i < 5; i++) {
    pinMode(limitSwitchPins[i], INPUT);
  }

  for (int i = 0; i < 5; i++) {
    servos[i].attach(servoPins[i]);
    servos[i].write(MOLE_DOWN);
  }

  Serial.begin(9600);
  gameStartTime = millis();
}

void loop() {
  unsigned long currentMillis = millis();
  if ((currentMillis - gameStartTime) <= gameDuration) { // Ensure game conditions only occur during set game duration
    const unsigned long gameSpeed = 800 - ((currentMillis - gameStartTime) / 1000) * 10;
    const uint8_t randomChance = random(0, 10);
    const int randomStayTime = random(500, 800);

    for (int i = 0; i < 5; i++) {
      const uint8_t limitSwitchState = digitalRead(limitSwitchPins[i]);
      limitSwitchStates[i] = (limitSwitchState == HIGH) ? UP : DOWN;


      if (limitSwitchStates[i] == UP) { // Mole is Up (Waiting to be Hit)
        if (randomChance < 3) { // 30% chance of it going back down
          servos[i].write(MOLE_DOWN);
          previousMillis[i] = currentMillis;
        }
        if (currentMillis - previousMillis[i] >= moleStayTime[i]) { // Comes back up again after staying down for a random amount of time
          servos[i].write(MOLE_UP);
          moleStayTime[i] = randomStayTime; //Give it another random for next time
        }
      } else if (limitSwitchStates[i] == DOWN) { // Mole is Down (Hit)
        if (!moleHitDelay[i]) { //Ensure we're not checking a servo that is currently in the hit delay (prevents false positives)
          hitTime[i] = currentMillis;
          score++;
          
          moleHitStartTime[i] = currentMillis;
          moleHitDelay[i] = true;
          
          char buffer[100];
          sprintf(buffer, "Mole %d Hit, Score: %d", i + 1, score);
          Serial.println(buffer);
        }
      }

      if (moleHitDelay[i] && (currentMillis - moleHitStartTime[i] >= HIT_DELAY)) { //Make sure the moles that are hit come back after the delay
        servos[i].write(MOLE_UP);
        moleHitDelay[i] = false;
      }

      if (moleStayTime[i] > 100) { //Make game go zoooooom
        moleStayTime[i] = gameSpeed;
        if (moleStayTime[i] < 100) {
          moleStayTime[i] = 100;
        }
      }

    }

  } else if ( (currentMillis - gameStartTime) >= gameDuration) {
    for (int i = 0; i < 5; i++) {
      servos[i].write(MOLE_DOWN);
    }
    Serial.print("Game Over! Total Score: ");
    Serial.println(score);
    delay(1000);
    exit(0);
  }
  delay(100);
}
