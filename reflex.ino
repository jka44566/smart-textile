const int ledPins[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
const int fsrPins[16] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15};
float timeLimits[] = {6000, 5500, 5000, 4500, 4000, 3500, 3000, 2500, 2000, 1500}; //timelimit setup for 10 rounds, can adjust based on audience
const int totalRounds = 10;
const int fsrThreshold = 100; //Need to configure based on actual analog sensor inputs
unsigned long startTime;
int currentLed = -1; //indicator for the LED/sensor checks
bool gameActive = false;
int roundIndex = 0;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 16; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  randomSeed(analogRead(0));
  Serial.println("Press sensor 0 to start ");
}

void loop() {
  // Start game by touching an FSR pin
  if (!gameActive && analogRead(fsrPins[0]) > fsrThreshold) {
    delay(300);
    Serial.println("Starting Game");
    gameActive = true;
    roundIndex = 0;
    delay(3000);
  }

  // If game is active, run a round
  if (gameActive) {
    ledOutput(); //randomizes a pin and turns it on

    int result = processUserInput(); // Waits for input or timeout

    if (result == currentLed) {
      logRound(currentLed, result, millis() - startTime);
      roundIndex++;
      delay(1000);
    } else {
      Serial.println("Timeout or incorrect input. Game Over.");
      gameActive = false;
      turnOnAllLeds();
      delay(2000);
      turnOffAllLeds();
    }

    if (roundIndex >= totalRounds) {
      Serial.println("Game Finished!");
      flashAllLeds(3);
      gameActive = false;
    }
  }
}

void ledOutput() {
  turnOffAllLeds();
  currentLed = random(0, 16);
  digitalWrite(ledPins[currentLed], HIGH);
  Serial.print(currentLed);
  Serial.println(" ON");
  startTime = millis();
}

int processUserInput() {
  unsigned long deadline = startTime + timeLimits[roundIndex];

  while (millis() < deadline) {
    int pressedIndex = -1;

    // Check if only one sensor is pressed
    for (int i = 0; i < 16; i++) {
      int value = analogRead(fsrPins[i]);
      if (value > fsrThreshold) {
        // Found a press — return if it’s correct
        if (i == currentLed) {
          return i; // Correct sensor pressed
        } else {
          Serial.print("Wrong sensor (");
          Serial.print(i);
          Serial.println("). Waiting for correct one");
          delay(300);
        }
      }
    }

    delay(50); // Small delay 
  }

  return -1;
}

void logRound(int ledIndex, int sensorIndex, unsigned long reactionTime) { //NEED TO IMPLEMENT FILE SAVING FUNCTIONS
  Serial.print("📒 Round ");
  Serial.print(roundIndex + 1);
  Serial.print(" | LED: ");
  Serial.print(ledIndex);
  Serial.print(" | Sensor: ");
  Serial.print(sensorIndex);
  Serial.print(" | Time: ");
  Serial.print(reactionTime);
  Serial.println(" ms");
}

void turnOffAllLeds() {
  for (int i = 0; i < 16; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}

void turnOnAllLeds() {
  for (int i = 0; i < 16; i++) {
    digitalWrite(ledPins[i], HIGH);
  }
}

void flashAllLeds(int times) {
  for (int i = 0; i < times; i++) {
    turnOnAllLeds();
    delay(250);
    turnOffAllLeds();
    delay(250);
  }
}