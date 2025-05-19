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
}

void updateFromTouch() {
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      int sensorValue = analogRead(touchSenors[r][c]);
      bool touched = sensorValue > TOUCH_THRESHOLD;
      digitalWrite(leds[r][c], touched ? HIGH : LOW);
    }
  }
}


void turnOnLED(int row, int col) {
  if (row >= 0 && row < rows && col >= 0 && col < cols) {
    digitalWrite(leds[row][col], HIGH);
  }
}

void turnOffLED(int row, int col) {
  if (row >= 0 && row < rows && col >= 0 && col < cols) {
    digitalWrite(leds[row][col], LOW);
  }
}
