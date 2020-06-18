
#include "vstxsynth.h"

#include <stdio.h>

float quantize(float f, int quan, int low)
{
	int i = iround(f*(quan-1));
	if (i < low) i = low;
	return i/(float)(quan-1);
}

void VstXSynth::displayWave(float value, char *text, int maxlen)
{
	switch (iround(value * 5.0f))
	{
	case 0:
		vst_strncpy (text, "Sine", maxlen);
		break;
	case 1:
		vst_strncpy (text, "Sawtooth", maxlen);
		break;
	case 2:
		vst_strncpy (text, "Square", maxlen);
		break;
	case 3:
		vst_strncpy (text, "Parabola", maxlen);
		break;
	case 4:
		vst_strncpy (text, "Triangle", maxlen);
		break;
	case 5:
		vst_strncpy (text, "Noise", maxlen);
		break;
	}
}

void VstXSynth::displayInteger(float value, char *text, int maxlen)
{
	snprintf(text, maxlen, "%d", iround(value));
}

void VstXSynth::displayFloat(float value, char *text, int maxlen)
{
	snprintf(text, maxlen, "%1.3f", value);
}

void VstXSynth::displayExpFloat(float value, char *text, int maxlen)
{
	snprintf(text, maxlen, "%1.3f", exp2(value));
}

void VstXSynth::displayEnvelope(float value, char *text, int maxlen)
{
	snprintf(text, maxlen, "%1.3fs", exp2(value) * ENVELOPE_BASE_TIME);
}

void VstXSynth::displayPitch(float value, char *text, int maxlen)
{
	int ht = iround(value * 12.0f);
	if (ht == 0) {
		snprintf(text, maxlen, "0");
	}
	else {
		char sign = '+';
		if (ht < 0) {
			ht = -ht;
			sign = '-';
		}
		if (ht < 12) {
			snprintf(text, maxlen, "%c%d ht", sign, ht);
		}
		else if (ht % 12 == 0) {
			snprintf(text, maxlen, "%c%d oct", sign, ht / 12);
		}
		else {
			snprintf(text, maxlen, "%c%d oct %d ht", sign, ht / 12, ht % 12);
		}
	}
}

void VstXSynth::displayDecay(float value, char *text, int maxlen)
{
	if (value < 0.000001f && value > -0.000001f) {
		snprintf(text, maxlen, "0");
	}
	else if (value < 0.0f) {
		snprintf(text, maxlen, "h %dms", iround(-1000.0f / (value * value * value)));
	}
	else {
		snprintf(text, maxlen, "d %dms", iround(1000.0f / (value * value * value)));
	}
}

void VstXSynth::displayDetune(float value, char *text, int maxlen)
{
	snprintf(text, maxlen, "%1.2fHz", value * value);
}

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new VstXSynth (audioMaster);
}

//-----------------------------------------------------------------------------------------
// VstXSynthProgram
//-----------------------------------------------------------------------------------------
VstXSynthProgram::VstXSynthProgram ()
{
	// Default Program Values
#define PARAM(name, title, displayfunc, min, max, def, quan, low) \
	f##name = quantize((def-(min))/((max)-(min)),quan,low);
#include "paramdef.h"
#undef PARAM
	vst_strncpy (name, "Basic", kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
// VstXSynth
//-----------------------------------------------------------------------------------------
VstXSynth::VstXSynth (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, kNumPrograms, kNumParams)
{
	// initialize programs
	programs = new VstXSynthProgram[kNumPrograms];
	for (VstInt32 i = 0; i < 16; i++)
		channelPrograms[i] = i;

	for (int i = 0 ; i < kMaxNotes ; i++)
	{
		currentNotes[i] = NULL;
	}

	if (programs)
		setProgram (0);
	
	if (audioMaster)
	{
		setNumInputs (0);				// no inputs
		setNumOutputs (kNumOutputs);	// 2 outputs, 1 for each oscillator
		canProcessReplacing ();
		isSynth ();
		setUniqueID ('LnsS');			// <<<! *must* change this!!!!
	}

	initProcess ();
	initProcessInternal ();
	suspend ();
}

//-----------------------------------------------------------------------------------------
VstXSynth::~VstXSynth ()
{
	if (programs)
		delete[] programs;
}

//-----------------------------------------------------------------------------------------
void VstXSynth::setProgram (VstInt32 program)
{
	if (program < 0 || program >= kNumPrograms)
		return;
	
	VstXSynthProgram *ap = &programs[program];
	curProgram = program;
}

//-----------------------------------------------------------------------------------------
void VstXSynth::setProgramName (char* name)
{
	vst_strncpy (programs[curProgram].name, name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void VstXSynth::getProgramName (char* name)
{
	vst_strncpy (name, programs[curProgram].name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void VstXSynth::getParameterLabel (VstInt32 index, char* text)
{
	switch (index)
	{
#define PARAM(name, title, displayfunc, min, max, def, quan, low) \
	case k##name: \
		snprintf(text, kVstMaxParamStrLen, "\xa0(%d)", iround((programs[curProgram].f##name+(min)/((max)-(min)))*(quan-1))); \
		break;
#include "paramdef.h"
#undef PARAM
	}
}

//-----------------------------------------------------------------------------------------
void VstXSynth::getParameterDisplay (VstInt32 index, char* text)
{
	VstXSynthProgram* ap = &programs[curProgram];
	text[0] = 0;
	switch (index)
	{
#define PARAM(name, title, displayfunc, min, max, def, quan, low) \
	case k##name: \
		display##displayfunc(ap->f##name * ((max)-(min)) + (min), text, kVstMaxParamStrLen); \
		break;
#include "paramdef.h"
#undef PARAM
	}
}

//-----------------------------------------------------------------------------------------
void VstXSynth::getParameterName (VstInt32 index, char* text)
{
	switch (index)
	{
#define PARAM(name, title, displayfunc, min, max, def, quan, low) \
	case k##name: \
		vst_strncpy (text, title, kVstMaxParamStrLen); \
		break;
#include "paramdef.h"
#undef PARAM
	}
}

//-----------------------------------------------------------------------------------------
void VstXSynth::setParameter (VstInt32 index, float value)
{
	VstXSynthProgram *ap = &programs[curProgram];
	switch (index)
	{
#define PARAM(name, title, displayfunc, min, max, def, quan, low) \
	case k##name: \
		ap->f##name = quantize(value,quan,low); \
		break;
#include "paramdef.h"
#undef PARAM
	}
}

//-----------------------------------------------------------------------------------------
float VstXSynth::getParameter (VstInt32 index)
{
	VstXSynthProgram *ap = &programs[curProgram];
	float value = 0;
	switch (index)
	{
#define PARAM(name, title, displayfunc, min, max, def, quan, low) \
	case k##name: \
	value = ap->f##name; \
	break;
#include "paramdef.h"
#undef PARAM
	}
	return value;
}

//-----------------------------------------------------------------------------------------
bool VstXSynth::getOutputProperties (VstInt32 index, VstPinProperties* properties)
{
	if (index < kNumOutputs)
	{
		vst_strncpy (properties->label, "Vstx ", 63);
		char temp[11] = {0};
		int2string (index + 1, temp, 10);
		vst_strncat (properties->label, temp, 63);

		properties->flags = kVstPinIsActive;
		if (index < 2)
			properties->flags |= kVstPinIsStereo;	// make channel 1+2 stereo
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------------------
bool VstXSynth::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
	if (index < kNumPrograms)
	{
		vst_strncpy (text, programs[index].name, kVstMaxProgNameLen);
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------------------
bool VstXSynth::getEffectName (char* name)
{
	vst_strncpy (name, "Clinkster", kVstMaxEffectNameLen);
	return true;
}

//-----------------------------------------------------------------------------------------
bool VstXSynth::getVendorString (char* text)
{
	vst_strncpy (text, "Loonies", kVstMaxVendorStrLen);
	return true;
}

//-----------------------------------------------------------------------------------------
bool VstXSynth::getProductString (char* text)
{
	vst_strncpy (text, "Clinkster 1.0 by Loonies", kVstMaxProductStrLen);
	return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 VstXSynth::getVendorVersion ()
{ 
	return 1000; 
}

//-----------------------------------------------------------------------------------------
VstInt32 VstXSynth::canDo (char* text)
{
	if (!strcmp (text, "receiveVstEvents"))
		return 1;
	if (!strcmp (text, "receiveVstMidiEvent"))
		return 1;
	if (!strcmp (text, "midiProgramNames"))
		return 1;
	return -1;	// explicitly can't do; 0 => don't know
}

//-----------------------------------------------------------------------------------------
VstInt32 VstXSynth::getNumMidiInputChannels ()
{
	return 1; // we are monophonic
}

//-----------------------------------------------------------------------------------------
VstInt32 VstXSynth::getNumMidiOutputChannels ()
{
	return 0; // no MIDI output back to Host app
}

// midi program names:
// as an example, GM names are used here. in fact, VstXSynth doesn't even support
// multi-timbral operation so it's really just for demonstration.
// a 'real' instrument would have a number of voices which use the
// programs[channelProgram[channel]] parameters when it receives
// a note on message.

//------------------------------------------------------------------------
VstInt32 VstXSynth::getMidiProgramName (VstInt32 channel, MidiProgramName* mpn)
{
	VstInt32 prg = mpn->thisProgramIndex;
	if (prg < 0 || prg >= 128)
		return 0;
	fillProgram (channel, prg, mpn);
	if (channel == 9)
		return 1;
	return 128L;
}

//------------------------------------------------------------------------
VstInt32 VstXSynth::getCurrentMidiProgram (VstInt32 channel, MidiProgramName* mpn)
{
	if (channel < 0 || channel >= 16 || !mpn)
		return -1;
	VstInt32 prg = channelPrograms[channel];
	mpn->thisProgramIndex = prg;
	fillProgram (channel, prg, mpn);
	return prg;
}

//------------------------------------------------------------------------
void VstXSynth::fillProgram (VstInt32 channel, VstInt32 prg, MidiProgramName* mpn)
{
	mpn->midiBankMsb = -1;
	mpn->midiBankLsb = -1;
	mpn->reserved = 0;
	mpn->flags = 0;
	vst_strncpy (mpn->name, "Clinkster", 63);
	mpn->midiProgram = 0;
	mpn->parentCategoryIndex = 0;
	mpn->parentCategoryIndex = -1;
}

//------------------------------------------------------------------------
VstInt32 VstXSynth::getMidiProgramCategory (VstInt32 channel, MidiProgramCategory* cat)
{
	cat->parentCategoryIndex = -1;	// -1:no parent category
	cat->flags = 0;					// reserved, none defined yet, zero.
	vst_strncpy (cat->name, "Clinkster", 63);
	return 1;
}

//------------------------------------------------------------------------
bool VstXSynth::hasMidiProgramsChanged (VstInt32 channel)
{
	return false;	// updateDisplay ()
}

//------------------------------------------------------------------------
bool VstXSynth::getMidiKeyName (VstInt32 channel, MidiKeyName* key)
								// struct will be filled with information for 'thisProgramIndex' and 'thisKeyNumber'
								// if keyName is "" the standard name of the key will be displayed.
								// if false is returned, no MidiKeyNames defined for 'thisProgramIndex'.
{
	// key->thisProgramIndex;		// >= 0. fill struct for this program index.
	// key->thisKeyNumber;			// 0 - 127. fill struct for this key number.
	key->keyName[0] = 0;
	key->reserved = 0;				// zero
	key->flags = 0;					// reserved, none defined yet, zero.
	return false;
}
