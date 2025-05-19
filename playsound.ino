// Plays sound effect depending on the need
// Result 1 is correct sound (correct)
// Result 2 is place sound (lighting up leds on grid)
// Result 3 is wrong sound (incorrect) 

#include <SimpleSDAudio.h>
#include "PlaySound.h"

// Array of available songs
const char* songs[] = {
  "correct.wav",
  "place.wav",
  "wrong.wav"
};

void PlaySound::begin() {
  Serial.println("Initializing Audio...");
  // Initialize audio playback library
  SdPlay.init(0);

  Serial.println("Audio initialized.");
}

void PlaySound::play(int songIndex) {
    if (songIndex < 0 || songIndex >= 3) {
        Serial.println("Invalid song index.");
        return;
    }

    const char* song = songs[songIndex];
    Serial.print("Playing: ");
    Serial.println(song);

    SdPlay.setFile(song);
    SdPlay.play();
}
