#pragma once
#include "vstxsynth.h"

class ClinksterRealtime : VstXSynth
{
public:
	ClinksterRealtime(void);
	~ClinksterRealtime(void);

private:
	audioMasterCallback callback;
	VstXSynth* synth;
};

