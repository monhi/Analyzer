#pragma once

#include <SDL.h>
#include <SDL_audio.h>

class CAudioPlayer
{
private:
	// local variables
	SDL_AudioSpec	wavSpec;
	Uint32			wavLength;
	Uint8			*wavBuffer;

public:
	CAudioPlayer();
	~CAudioPlayer();
	int PlayWave(const char*  fileName);
};

