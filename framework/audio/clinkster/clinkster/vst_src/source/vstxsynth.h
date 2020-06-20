
#ifndef __vstxsynth__
#define __vstxsynth__

#include "vst2.x/audioeffectx.h"

#include <xmmintrin.h>
#include <emmintrin.h>

#include <math.h>

//------------------------------------------------------------------------------------------
enum
{
	// Parameters Tags
#define PARAM(name, title, displayfunc, min, max, def, quan, low) \
	k##name,
#include "paramdef.h"
#undef PARAM
	kNumParams
};

// Global
const int kNumPrograms = 128;
const int kNumOutputs = 2;
const int kMaxNotes = 100;

const int INTERNAL_FEEDBACK_BUFFER = 10000;
const int INTERNAL_RENDER_BUFFER = 100000;
const int INTERNAL_BUFFER = INTERNAL_FEEDBACK_BUFFER + INTERNAL_RENDER_BUFFER;
const int MAX_LAYERS = 50;

const float ENVELOPE_BASE_TIME = 0.18575396825f;

struct sampleC
{
	float left,right;
};

inline double exp2(double f)
{
	return exp(f*0.69314718056);
}

inline int iround(double f) {
	return _mm_cvtsd_si32(_mm_set_sd(f));
}

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

class VstXSynth;

//------------------------------------------------------------------------------------------
// VstXSynthProgram
//------------------------------------------------------------------------------------------
class VstXSynthProgram
{
public:
	VstXSynthProgram ();
	~VstXSynthProgram () {}

#define PARAM(name, title, displayfunc, min, max, def, quan, low) \
	float f##name;
#include "paramdef.h"
#undef PARAM
	char name[kVstMaxProgNameLen+1];
};

class VstXSynthNote
{
public:
	VstXSynthNote (VstXSynthProgram *program, VstInt32 channel, VstInt32 note, VstInt32 velocity, VstInt32 delta)
		: program(*program), channel(channel), note(note), startVelocity(velocity), stopped(false),
		startDelta(delta), stopDelta(2000000000), startOffset(0), stopOffset(2000000000)
	{
		internal_buffer = new sampleC[INTERNAL_BUFFER];
		internal_buf_write_index = INTERNAL_FEEDBACK_BUFFER;
		internal_buf_read_index = (double)internal_buf_write_index;
	}
	~VstXSynthNote ()
	{
		delete internal_buffer;
	}

	void stop(VstInt32 delta, VstInt32 velocity);
	VstXSynthNote* process(VstXSynth *synth, float **out, VstInt32 nsamples);
	bool thisNote(VstInt32 channel, VstInt32 note);

private:
	VstInt32 channel;

	VstInt32 startDelta;
	VstInt32 stopDelta;
	bool stopped;

	sampleC *internal_buffer;
	int internal_buf_write_index;
	double internal_buf_read_index;

	VstXSynthNote* render(VstXSynth *synth, float **out, VstInt32 nsamples);

protected:
	VstXSynthProgram program;
	VstInt32 note;
	VstInt32 startVelocity;
	VstInt32 stopVelocity;
	VstInt32 startOffset;
	VstInt32 stopOffset;

	virtual bool renderInternal(VstXSynth *synth, sampleC *out, VstInt32 nsamples) = 0;
};

struct layer
{
	double freq1,freq2;
	double phase1,phase2;
	double index;
};

class MyNote : public VstXSynthNote
{
public:
	MyNote (VstXSynthProgram *program, VstInt32 channel, VstInt32 note, VstInt32 velocity, VstInt32 delta, float _internal_samplerate);

protected:
	float internal_samplerate;

	layer layers[MAX_LAYERS][2];
	double currpitch1,currpitch2;
	double pitchdecay1,pitchdecay2;
	double indexdecay;
	virtual bool renderInternal(VstXSynth *synth, sampleC *out, VstInt32 nsamples);
};



//------------------------------------------------------------------------------------------
// VstXSynth
//------------------------------------------------------------------------------------------
class VstXSynth : public AudioEffectX
{
friend class VstXSynthNote;
public:
	VstXSynth (audioMasterCallback audioMaster);
	~VstXSynth ();

	virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
	virtual VstInt32 processEvents (VstEvents* events);

	virtual void setProgram (VstInt32 program);
	virtual void setProgramName (char* name);
	virtual void getProgramName (char* name);
	virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

	virtual void setParameter (VstInt32 index, float value);
	virtual float getParameter (VstInt32 index);
	virtual void getParameterLabel (VstInt32 index, char* label);
	virtual void getParameterDisplay (VstInt32 index, char* text);
	virtual void getParameterName (VstInt32 index, char* text);
	
	virtual void setSampleRate (float sampleRate);
	virtual void setBlockSize (VstInt32 blockSize);
	
	virtual bool getOutputProperties (VstInt32 index, VstPinProperties* properties);
		
	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual VstInt32 getVendorVersion ();
	virtual VstInt32 canDo (char* text);

	virtual VstInt32 getNumMidiInputChannels ();
	virtual VstInt32 getNumMidiOutputChannels ();

	virtual VstInt32 getMidiProgramName (VstInt32 channel, MidiProgramName* midiProgramName);
	virtual VstInt32 getCurrentMidiProgram (VstInt32 channel, MidiProgramName* currentProgram);
	virtual VstInt32 getMidiProgramCategory (VstInt32 channel, MidiProgramCategory* category);
	virtual bool hasMidiProgramsChanged (VstInt32 channel);
	virtual bool getMidiKeyName (VstInt32 channel, MidiKeyName* keyName);

private:
	VstXSynthProgram* programs;
	VstInt32 channelPrograms[16];

	VstXSynthNote *currentNotes[kMaxNotes];

	float samplerate;
	float internal_samplerate;

	void initProcess ();
	void initProcessInternal ();

	void startNote (VstXSynthProgram *program, VstInt32 channel, VstInt32 note, VstInt32 velocity, VstInt32 delta);
	void stopNote (VstInt32 channel, VstInt32 note, VstInt32 velocity, VstInt32 delta);
	void stopAllNotes();
	void fillProgram (VstInt32 channel, VstInt32 prg, MidiProgramName* mpn);

	void displayWave(float value, char *text, int maxlen);
	void displayDetune(float value, char *text, int maxlen);
	void displayPitch(float value, char *text, int maxlen);
	void displayDecay(float value, char *text, int maxlen);
	void displayFloat(float value, char *text, int maxlen);
	void displayInteger(float value, char *text, int maxlen);
	void displayEnvelope(float value, char *text, int maxlen);
	void displayExpFloat(float value, char *text, int maxlen);
};

#endif
