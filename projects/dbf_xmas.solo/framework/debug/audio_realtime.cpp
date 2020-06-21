#include "../../config.h"
#include "audio_realtime.h"
#include "../../intro.h"
#include <windows.h>

ZAudioRealtime * g_pAudioRealtime;

extern WININFO wininfo;

DSBUFFERDESC bufferDesc =
{
	sizeof(DSBUFFERDESC),   // size
	DSBCAPS_PRIMARYBUFFER | DSBCAPS_STICKYFOCUS | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_TRUEPLAYPOSITION,    // flags
	0,  // buffer bytes
	0,  // reserved
	NULL,   // format
	0       // guid
};
WAVEFORMATEX bufferFormat =
{
	WAVE_FORMAT_PCM,    // format
	REPLAY_CHANNELS,  // channels
	REPLAY_RATE,     // sample rate
	REPLAY_RATE * REPLAY_CHANNELS * REPLAY_SAMPLESIZE,    // bytes per second
	REPLAY_CHANNELS * REPLAY_SAMPLESIZE, // block align
	REPLAY_DEPTH, // sample depth
	0   // extra data
};

void SetFloatingPointRoundingToTruncate()
{
	/*
	__asm
	{
		fstcw   control_word                // store fpu control word
		mov     dx, word ptr [control_word]
		or      dx, 0x0C00                  // rounding: truncate
		mov     control_word2, dx
		fldcw   control_word2               // load modfied control word
	}*/
}


ZAudioRealtime::ZAudioRealtime(ZSynth* synth)
{
	DS = NULL;
	PrimaryBuffer = NULL;
	DSBuffer = NULL;
	writePos = 0;
	synth = synth;
	isRunning = false;
}

ZAudioRealtime::~ZAudioRealtime(void)
{
	Destroy();
}

bool ZAudioRealtime::Setup()
{
	HRESULT hRes = DirectSoundCreate(0, &DS, 0);
	if (hRes == DS_OK)
	{	// Then we have to set a cool cooperative level :-)
		hRes = DS->SetCooperativeLevel(wininfo.hWnd, DSSCL_EXCLUSIVE | DSSCL_PRIORITY);

		if (hRes == DS_OK)
		{	// Create the primary audio buffer.
			hRes = DS->CreateSoundBuffer(&bufferDesc, &PrimaryBuffer, NULL);

			if (hRes == DS_OK)
			{
				hRes = PrimaryBuffer->SetFormat(&bufferFormat);

				if (hRes == DS_OK)
				{
					// Now create a normal sound buffer.
					bufferDesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_TRUEPLAYPOSITION | DSBCAPS_STICKYFOCUS;
					bufferDesc.dwBufferBytes = REPLAY_BUFFERLEN;
					bufferDesc.lpwfxFormat = &bufferFormat;		// Same format as primary
					hRes = DS->CreateSoundBuffer(&bufferDesc,&DSBuffer,NULL);

					return hRes == DS_OK;
				}
			}
		}
	}
	return false;		// TODO: Catch all the errors !! :-)

}

void ZAudioRealtime::FillBuffer()
{
	static float lastPrecalcUpdateTime = 0.f;

	DWORD	playPos;
	DWORD	writeLen;
	LPVOID  p1, p2;
	DWORD	l1,l2;

	signed short *tp1, *tp2;

	writeLen = 1; // dummy value, just to get going

	while (writeLen > 0) // keep trying to fill buffer until we've caught up
	{
		HRESULT hRes;
		hRes = DSBuffer->GetCurrentPosition(&playPos, NULL);

		if (hRes != DS_OK) playPos = 0;

		if (writePos < playPos)
			writeLen = playPos - writePos - 1;
		else
			writeLen = REPLAY_BUFFERLEN - (writePos - playPos) - 1;

		writeLen = writeLen & ~1023;    // Limit to multiples of 1024 (= 4 x 256)

		if (writeLen > 0)
		{
			while (DS_OK != DSBuffer->Lock(writePos,writeLen,&p1,&l1,&p2,&l2,0))
			{
				DSBuffer->Restore();
				DSBuffer->Play(0, 0, DSBPLAY_LOOPING);
			}

			//umsg("render %d, %d", l1, l2);

			tp1=(signed short *)p1;
			tp2=(signed short *)p2;

			int numRenders;

			numRenders=l1/1024;

			for (int i=0; i<numRenders; i++)
			{
				/*
				if (g_pDemo->IsPrecalcing())
				{
					float renderTime = (synth->currentRenderLine * synth->samplesPerLine + synth->currentRenderSample) / 44100.f;

					if ((renderTime - lastPrecalcUpdateTime) > 1.f)
					{
						g_pDemo->UpdatePrecalc(renderTime - lastPrecalcUpdateTime);
						lastPrecalcUpdateTime = renderTime;
					}
				}*/

				//synth->FillBuffer(tp1, 256);
				tp1+=512;
			}

			numRenders=l2/1024;

			for (int i=0; i<numRenders; i++)
			{
				//synth->FillBuffer(tp2, 256);
				tp2+=512;
			}

			DSBuffer->Unlock(p1,l1,p2,l2);
			writePos += writeLen;

			if (writePos >= REPLAY_BUFFERLEN)
				writePos -= REPLAY_BUFFERLEN;
		}
	}
}

void ZAudioRealtime::Destroy()
{
	// kill thread
	//Cancel();

	if (DSBuffer) DSBuffer->Release();
	if (PrimaryBuffer) PrimaryBuffer->Release();
	if (DS) DS->Release();
}

extern void SetFloatingPointRoundingToTruncate();

void ZAudioRealtime::Run()
{
	SetFloatingPointRoundingToTruncate(); // this must be done on a per thread basis

	while (isRunning)
	{
		FillBuffer();
		Sleep(10);
	}
}

void ZAudioRealtime::Play()
{
	HRESULT hRes = DSBuffer->Play(0, 0, DSBPLAY_LOOPING);

	if (hRes != DS_OK)
	{
		//umsg("Couldn't play buffer!");
		PostQuitMessage(-5);
	}

	DWORD playPos = 0;

	while (playPos == 0)
		hRes = DSBuffer->GetCurrentPosition(&playPos, NULL);

	return;
}