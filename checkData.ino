// Checks for txt file in data folder

// CSV file headings:
// Game id (integer),
// Total LEDs (integer) (Total LEDs used in the game)
// LEDs Hit (integer) (LEDs that was correctly selected)
// Success (boolean)

//https://www.youtube.com/watch?v=FI7yXDi-fKA&t=786s  
#include <SD.h>
#include "checkdata.h"

#define SD_CLK 52
#define SD_MISO 50
#define SD_MOSI 51

#define SD_CS 10

void checkfiles() {
    Serial.begin(115200);

    // avoid chip select contention
    pinMode(SD_CS, OUTPUT);
    digitalWrite(SD_CS, HIGH);

    if (!SD.begin(SD_CS)) {
    Serial.println("SD Card initialization failed!");
    while(true){
        ;
    }
    }
    else {
    Serial.println("SD Card OK!");
    }

    // Check if directory / folder exists and create
    if (!SD.exists("/data")){
    Serial.println("Data folder is being created");
    SD.mkdir("/data");
    }

    //Check data folder for txt file
    if (!SD.exists("/data/memory_game_data.txt")) {
    Serial.println("Data file not found in folder");
    File newFile = SD.open("/data/memory_game_data.txt", FILE_WRITE);
    if (newFile) {
        //Construct csv headers
        newFile.println("GameID,TotalLEDs,LEDsHit,Success");
        newFile.close();
        Serial.println("Memory game data file created successfully.");
    } else {
        Serial.println("Failed to create data file");
    }
    } else {
    Serial.println("File already exists");
    }
}
void logGameData(int gameId, int totalLEDs, int ledsHit, bool success) {
  File file = SD.open("/data/memory_game_data.txt", FILE_WRITE);
  if (file) {
    // Construct the CSV line
    String logEntry = String(gameId) + "," +
                      String(totalLEDs) + "," +
                      String(ledsHit) + "," +
                      (success ? "true" : "false");

    // Write to the file
    file.println(logEntry);
    file.close();
    Serial.println(logEntry);
  } else {
    Serial.println("Data file cannot be ammended");
  }
}
