#define WIN32_LEAN_AND_MEAN // need this here in debug mode to stop all the winsock stuff being included with windows.h etc
#define WIN32_EXTRA_LEAN

#include "audio.h"
#include "bass.h"
#include "rocket/rocketControl.h"
#include "rocket/sync/device.h"
#include "framework_intro_config.h"

/*
DEBUG ONLY!
*/
#include <direct.h>
#include <stdio.h>
#include <math.h>
#include <shlwapi.h>

float musicPosNormalized = 0.0f;
float musicLengthSecs = 0.0f;
unsigned long long musicSizeBytes = 0;
unsigned char* musicData = NULL;

HSTREAM streamAudio;
const float bpm = 150.0f; /* beats per minute */
const int rpb = 8; /* rows per beat */
const double row_rate = (double(bpm) / 60) * rpb;

void bass_playPause(int doPause)
{
	HSTREAM h = streamAudio;
	if (doPause)
		BASS_ChannelPause(h);
	else
		BASS_ChannelPlay(h, false);
}

void bass_set_row(int row)
{
	HSTREAM h = streamAudio;
	unsigned long long pos = BASS_ChannelSeconds2Bytes(h, row / row_rate);
	BASS_ChannelSetPosition(h, pos, BASS_POS_BYTE);
}

void audioSetNormalizedPos(float normalizedPos)
{
	HSTREAM h = streamAudio;
	unsigned long long pos = (unsigned long long)(normalizedPos * musicSizeBytes);
	BASS_ChannelSetPosition(h, pos, BASS_POS_BYTE);
}

int bass_is_playing()
{
	HSTREAM h = streamAudio;
	return BASS_ChannelIsActive(h) == BASS_ACTIVE_PLAYING;
}

// per frame update
void audioUpdate()
{
	UpdateRocket();
}

void openAudioFileStream()
{
	HMODULE hModule = GetModuleHandleW(NULL);
	char path[MAX_PATH];
	GetModuleFileName(hModule, path, MAX_PATH);
	PathRemoveFileSpecA(path);
	sprintf(path, "%s\\%s", path, "music.wav");

	// start music
	if (BASS_Init(-1, 44100,0,0,nullptr))
	{
		FILE* fileWaveData = fopen(path, "rb");		
		fseek(fileWaveData, 0, SEEK_END);
		int musicFileLength = ftell(fileWaveData);
		fseek(fileWaveData, 0, SEEK_SET);

		// track length in ticks
		int trackLength = 0;
		float ticksPerSecond = 0.0f;
		fread(&trackLength, sizeof(trackLength), 1, fileWaveData);
		fread(&ticksPerSecond, sizeof(ticksPerSecond), 1, fileWaveData);

		musicData = new unsigned char[musicFileLength];
		fread(musicData, sizeof(unsigned char), musicFileLength, fileWaveData);
		fclose(fileWaveData);

		musicPosNormalized = 0;
		musicLengthSecs = (trackLength / (float)ticksPerSecond);

		void* wavLocation = musicData;

		streamAudio=BASS_StreamCreateFile(true, wavLocation, 0, musicFileLength, BASS_SAMPLE_FLOAT);		

		if (!streamAudio)
		{
			int error = BASS_ErrorGetCode();
			DebugBreak();
		}
	}
	else
	{
		int error = BASS_ErrorGetCode();
		DebugBreak();
	}

	if (streamAudio)
	{
		musicSizeBytes = BASS_ChannelGetLength(streamAudio, BASS_POS_BYTE);
		//BASS_ChannelPlay(streamAudio, true);
	}

}