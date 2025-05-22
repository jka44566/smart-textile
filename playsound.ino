// Plays sound effect depending on the need
// Result 1 is correct sound (correct)
// Result 2 is place sound (lighting up leds on grid)
// Result 3 is wrong sound (incorrect) 

#include <SimpleSDAudio.h>
#include "PlaySound.h"

// Array of available songs files
const char* songs[] = {
  "correct.wav",
  "place.wav",
  "wrong.wav"
};

void PlaySound::begin() {
  // Text validation
  Serial.println("Initializing Audio...");
  // Initialize audio player
  SdPlay.init(0);
  // Text validation
  Serial.println("Audio initialized.");
}

void PlaySound::play(int songIndex) {
    if (songIndex < 0 || songIndex >= 3) {
        Serial.println("Invalid song index.");
        return;
    }
    const char* song = songs[songIndex];
    // Create filename to be called 
    // (pass in copy due to chance setFile might modify string causing errors )
    char filename[10];
    strcpy(filename, song);

    // Text validation
    Serial.print("Playing: ");
    Serial.println(filename);

    SdPlay.setFile(filename);
    SdPlay.play();
}

