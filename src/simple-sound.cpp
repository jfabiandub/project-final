// Aline Normoyle, 2023 
// Console application for playing sounds
// See: https://www.fmod.com/docs/2.02/api/core-api.html

#include "fmod_errors.h"
#include <cstdlib>
#include <iostream>
#include "fmod.hpp"

void ERRCHECK(FMOD_RESULT result) {
  if (result != FMOD_OK)
  {
    printf("FMOD error! (%d) %s\n", 
       result, FMOD_ErrorString(result));
    exit(-1);
  }
}

int main(int argc, char* argv[])
{
  FMOD_RESULT result;

  FMOD::System *system = NULL;
	FMOD::Channel *backgroundChannel = NULL;
	FMOD::Sound *music;
	FMOD::Sound *meow;
	
  result = FMOD::System_Create(&system);		
	ERRCHECK(result);

	result = system->init(100, FMOD_INIT_NORMAL, 0);	
	ERRCHECK(result);

  // Initialize background music
	result = system->createStream(
      "../Sounds/beat-n-bass-128-bpm.wav", 
      FMOD_DEFAULT, 0, &music);
	ERRCHECK(result);

  result = music->setMode(FMOD_LOOP_NORMAL);
	ERRCHECK(result);

	result = system->playSound(music, 0, true, &backgroundChannel);
  ERRCHECK(result);

  // Set volume while sound is paused
	result = backgroundChannel->setVolume(0.5f); 
	ERRCHECK(result);

	result = backgroundChannel->setPaused(false); 
	ERRCHECK(result);

  // Initialize foreground sound
	result = system->createStream(
      "../Sounds/cat-meow.wav", 
      FMOD_DEFAULT, 0, &meow);
	ERRCHECK(result);

  std::cout << "Press the '1' key to play a sound on top of the music." << std::endl;
  std::cout << "Press the 'q' key to quit." << std::endl;
  char key;
  do 
  {
    std::cin >> key; 
    if (key == '1') 
    {
      result = system->playSound(meow, 0, false, 0);
			ERRCHECK(result);			
    }
    system->update();

  } while (key != 'q');

  if (music != NULL) {
		result = music->release();
		ERRCHECK(result);
	}

	result = system->release();
  ERRCHECK(result);

	return 0;
}
