#include "checkdata.h"
#include <SimpleSDAudio.h>
#include "PlaySound.h"
PlaySound audio;

void setup() {
  checkfiles();
  logGameData(1, 2, 3, true);
  audio.begin();
  audio.play(0); 
}

void loop() {

}
