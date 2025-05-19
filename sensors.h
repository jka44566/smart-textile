#ifndef SENSORS_H
#define SENSORS_H
//Need to change based of how sensitive it needs to be
#define TOUCH_THRESHOLD 20

// Grid dimensions
const int rows = 4;
const int cols = 4;

// External declarations for the sensor and LED arrays
extern int touchSenors[rows][cols];
extern int leds[rows][cols];

// Function declarations
void initializeGrid();
void updateFromTouch();
void turnOnLED(int row, int col);
void turnOffLED(int row, int col);

#endif
