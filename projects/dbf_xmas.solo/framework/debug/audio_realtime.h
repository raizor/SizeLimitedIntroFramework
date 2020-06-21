#pragma once

#include <dsound.h>

#define	REPLAY_RATE				44100
#define	REPLAY_DEPTH			16
#define	REPLAY_CHANNELS			2
#define	REPLAY_SAMPLESIZE		(REPLAY_DEPTH/8)
#define	REPLAY_BUFFERLEN		(4*32*32*1024)

class ZSynth;

class ZAudioRealtime 
{
public:
	ZAudioRealtime(ZSynth* synth);
	~ZAudioRealtime(void);

	bool Setup();
	void FillBuffer();
	void Destroy();
	void Play();	
	virtual void Run();

private:
	LPDIRECTSOUND		DS;
	LPDIRECTSOUNDBUFFER	PrimaryBuffer;
	LPDIRECTSOUNDBUFFER	DSBuffer;
	DWORD				writePos;

	ZSynth* synth;
	bool isRunning;
};

extern ZAudioRealtime * g_pAudioRealtime;
