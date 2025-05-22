#include "sensors.h"
// Need to change based on position of leds and touch sensors
int touchSenors[rows][cols] = {
  {2, 3, 4, 5},
  {6, 7, 8, 9},
  {10, 11, 12, 13},
  {A0, A1, A2, A3}
};

int leds[rows][cols] = {
  {22, 23, 24, 25},
  {26, 27, 28, 29},
  {30, 31, 32, 33},
  {34, 35, 36, 37}
};

//Initilizing grid to connect the led to touch sensor
void initializeGrid() {
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      if (touchSenors[r][c] < A0) {
        pinMode(touchSenors[r][c], INPUT);
      }
      pinMode(leds[r][c], OUTPUT);
      digitalWrite(leds[r][c], LOW);
    }
  }
  // Text validation
  Serial.println("Grid initialization complete.");
}


// The following functions is to update the led from touch to the opposite state.
// Keeps track of LED states (on/off)
bool ledStates[rows][cols] = {false};
// Keeps track of whether the sensor was touched last time
// This is to ensure when holding the sensor it does not 
bool lastTouchStates[rows][cols] = {false};

void updateFromTouch() {
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      int sensorValue = analogRead(touchSenors[r][c]);
      bool touchedNow = sensorValue > TOUCH_THRESHOLD;

      // Only toggle if it's a *new* touch (not held down)
      if (touchedNow && !lastTouchStates[r][c]) {
        ledStates[r][c] = !ledStates[r][c]; // toggle
        digitalWrite(leds[r][c], ledStates[r][c] ? HIGH : LOW);
      // Text validation
        Serial.print("Toggled LED at [");
        Serial.print(r); Serial.print("]["); Serial.print(c);
        Serial.print("] to ");
        Serial.println(ledStates[r][c] ? "ON" : "OFF");
      }
      // Update the last touch state
      lastTouchStates[r][c] = touchedNow;
    }
  }
}

//Function to turn specific led on
void turnOnLED(int row, int col) {
  if (row >= 0 && row < rows && col >= 0 && col < cols) {
    digitalWrite(leds[row][col], HIGH);
    // Text validation
    Serial.print("LED at [");
    Serial.print(row); Serial.print("]["); Serial.print(col);
    Serial.println("] turned ON.");
  }
}

//Function to turn specific led off
void turnOffLED(int row, int col) {
  if (row >= 0 && row < rows && col >= 0 && col < cols) {
    digitalWrite(leds[row][col], LOW);
    // Text validation
    Serial.print("LED at [");
    Serial.print(row); Serial.print("]["); Serial.print(col);
    Serial.println("] turned OFF.");
  }
}
