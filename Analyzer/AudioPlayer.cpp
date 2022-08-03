#include "stdafx.h"
#include "AudioPlayer.h"
#include "general.h"

CAudioPlayer::CAudioPlayer()
{
}

int CAudioPlayer::PlayWave(const char*  fileName)
{
	if ( SDL_LoadWAV(fileName, &wavSpec, &wavBuffer, &wavLength) == NULL )
	{
		return FAILURE;
	}

	SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
	int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
	SDL_PauseAudioDevice(deviceId, 0);
	SDL_Delay(3000);
	SDL_CloseAudioDevice(deviceId);
	SDL_FreeWAV(wavBuffer);
	return SUCCESS;
}

CAudioPlayer::~CAudioPlayer()
{
}
