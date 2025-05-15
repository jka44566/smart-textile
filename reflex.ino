const int ledPins[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
const int fsrPins[16] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15}; 
unsigned long startTime;
const int fsrThreshold = 100;

int currentLed = -1;
bool gameActive = false;
int roundIndex = 0;
unsigned long startTime;

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 16; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  pinMode(startButtonPin, INPUT_PULLUP);
  randomSeed(analogRead(0));
  Serial.println("Press START to begin.");
}

void loop() {
  if (!gameActive &&  == LOW) { //add game starting condition
    delay(300); // debounce
    Serial.println("🎮 Game Started");
    gameActive = true;
    roundIndex = 0;
    delay(3000); // Hold before first LED
  }

  if (gameActive) {




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
    int activeIndex = -1;
    int count = 0;

    for (int i = 0; i < 16; i++) {
      int fsrValue = analogRead(fsrPins[i]);
      if (fsrValue > fsrThreshold) {
        activeIndex = i;
        count++;
      }
    }

    if (count == 1) return activeIndex;
    if (count > 1) {
      Serial.println("⚠️ Multiple sensors pressed! Ignoring.");
      delay(300);
      return -2;
    }
  }
  return -1; // Timeout
}

void logRound(int ledIndex, int sensorIndex, unsigned long reactionTime) {

}