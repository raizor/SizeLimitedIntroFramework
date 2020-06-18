
#include "vstxsynth.h"

#define _USE_MATH_DEFINES
#include <math.h>

static const int FILTER_LENGTH = 24;
static const float FILTER_FACTOR = 1.0f/1024.0f;

static float resample_filter[FILTER_LENGTH];

static signed char filter_coefficients[FILTER_LENGTH] = {
	-1,-2,-4,-4,-2,3,14,30,51,98,116,126,126,116,98,51,30,14,3,-2,-4,-4,-2,-1
};

//-----------------------------------------------------------------------------------------
// VstXSynth
//-----------------------------------------------------------------------------------------
void VstXSynth::initProcess ()
{
	// Normalize filter
	for (int i = 0 ; i < FILTER_LENGTH ; i++)
	{
		resample_filter[i] = filter_coefficients[i] * FILTER_FACTOR;
	}
}

//-----------------------------------------------------------------------------------------
void VstXSynth::setSampleRate (float sampleRate)
{
	AudioEffectX::setSampleRate (sampleRate);
	samplerate = sampleRate;
	internal_samplerate = samplerate * 4;
}

//-----------------------------------------------------------------------------------------
void VstXSynth::setBlockSize (VstInt32 blockSize)
{
	AudioEffectX::setBlockSize (blockSize);
	// you may need to have to do something here...
}

//-----------------------------------------------------------------------------------------
void VstXSynth::startNote(VstXSynthProgram *program, VstInt32 channel, VstInt32 note, VstInt32 velocity, VstInt32 delta)
{
	for (int i = 0 ; i < kMaxNotes ; i++)
	{
		if (currentNotes[i] == NULL)
		{
			currentNotes[i] = new MyNote(program, channel, note, velocity, delta, internal_samplerate);
			return;
		}
	}
	// No more slots for simultaneous notes available
}

void VstXSynth::stopNote(VstInt32 channel, VstInt32 note, VstInt32 velocity, VstInt32 delta)
{
	for (int i = 0 ; i < kMaxNotes ; i++)
	{
		if (currentNotes[i] != NULL && currentNotes[i]->thisNote(channel, note))
		{
			currentNotes[i]->stop(delta, velocity);
			return;
		}
	}
	// Note not found, hmm
}

void VstXSynth::stopAllNotes()
{
	// Emergency stop
	for (int i = 0 ; i < kMaxNotes ; i++)
	{
		if (currentNotes[i] != NULL)
		{
			delete currentNotes[i];
			currentNotes[i] = 0;
		}
	}
}

//-----------------------------------------------------------------------------------------
void VstXSynth::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	memset (outputs[0], 0, sampleFrames * sizeof (float));
	memset (outputs[1], 0, sampleFrames * sizeof (float));

	for (int i = 0 ; i < kMaxNotes ; i++)
	{
		if (currentNotes[i] != NULL)
		{
			VstXSynthNote *new_value = currentNotes[i]->process(this, outputs, sampleFrames);
			if (new_value != currentNotes[i])
			{
				delete currentNotes[i];
				currentNotes[i] = new_value;
			}
		}
	}

	int j = 0;
	for (int i = 0 ; i < kMaxNotes ; i++)
	{
		if (currentNotes[i] != NULL)
		{
			currentNotes[j++] = currentNotes[i];
		}
	}
	for (; j < kMaxNotes ; j++)
	{
		currentNotes[j] = NULL;
	}
}

//-----------------------------------------------------------------------------------------
VstInt32 VstXSynth::processEvents (VstEvents* ev)
{
	for (VstInt32 i = 0; i < ev->numEvents; i++)
	{
		if ((ev->events[i])->type != kVstMidiType)
			continue;

		VstMidiEvent* event = (VstMidiEvent*)ev->events[i];
		char* midiData = event->midiData;
		VstInt32 channel = midiData[0] & 0x0f;
		VstInt32 status = midiData[0] & 0xf0;	// ignoring channel
		if (status == 0x90 || status == 0x80)	// we only look at notes
		{
			VstInt32 note = midiData[1] & 0x7f;
			VstInt32 velocity = midiData[2] & 0x7f;
			if (status == 0x90)
			{
				startNote(&programs[curProgram], channel, note, velocity, event->deltaFrames);
			} else {
				stopNote(channel, note, velocity, event->deltaFrames);
			}
		}
		else if (status == 0xb0)
		{
			if (midiData[1] == 0x7e || midiData[1] == 0x7b)	// all notes off
			{
				stopAllNotes();
			}
		}
	}
	return 1;
}

bool VstXSynthNote::thisNote(VstInt32 channel, VstInt32 note)
{
	return channel == this->channel && note == this->note && !stopped;
}

void VstXSynthNote::stop(VstInt32 delta, VstInt32 velocity)
{
	stopped = true;
	stopDelta = delta;
	stopVelocity = velocity;
}

VstXSynthNote* VstXSynthNote::process(VstXSynth *synth, float **out, VstInt32 nsamples)
{
	if (stopDelta <= startDelta)
	{
		return NULL;
	}

	if (startDelta >= nsamples)
	{
		startDelta -= nsamples;
		stopDelta -= nsamples;
		return this;
	}

	float *outputs[2];
	outputs[0] = out[0];
	outputs[1] = out[1];

	VstInt32 renderlength = nsamples;
	if (startDelta > 0)
	{
		outputs[0] = &outputs[0][startDelta];
		outputs[1] = &outputs[1][startDelta];
		renderlength -= startDelta;
		stopDelta -= startDelta;
		startDelta = 0;
	}

	VstXSynthNote* newnote = render(synth, outputs, renderlength);

	startDelta -= renderlength;
	stopDelta -= renderlength;
	return newnote;
}

VstXSynthNote* VstXSynthNote::render(VstXSynth *synth, float **outputs, VstInt32 nsamples)
{
	float* out1 = outputs[0];
	float* out2 = outputs[1];

	int read_step = 4;
	int filter_width = 24;
	if (stopDelta >= 0 && stopDelta < nsamples)
	{
		stopOffset = stopDelta * read_step;
	}
	while (nsamples > 0)
	{
		if (internal_buf_write_index == INTERNAL_BUFFER)
		{
			// buffer full - shift data
			for (int i = 0 ; i < INTERNAL_FEEDBACK_BUFFER ; i++)
			{
				internal_buffer[i] = internal_buffer[INTERNAL_BUFFER-INTERNAL_FEEDBACK_BUFFER + i];
			}
			internal_buf_write_index = INTERNAL_FEEDBACK_BUFFER;
			internal_buf_read_index -= INTERNAL_BUFFER-INTERNAL_FEEDBACK_BUFFER;
		}

		int end_read_pos = (int) (internal_buf_read_index + nsamples * read_step);
		int internal_nsamples = end_read_pos - internal_buf_write_index;
		if (internal_nsamples > INTERNAL_BUFFER-internal_buf_write_index)
		{
			internal_nsamples = INTERNAL_BUFFER-internal_buf_write_index;
		}

		bool done = renderInternal(synth, &internal_buffer[internal_buf_write_index], internal_nsamples);
		internal_buf_write_index += internal_nsamples;
		startOffset -= internal_nsamples;
		stopOffset -= internal_nsamples;

		while (internal_buf_read_index < internal_buf_write_index)
		{
			// Filtering
			float left = 0.0f;
			float right = 0.0f;
			int read_index = (int) internal_buf_read_index;
			for (int i = 0 ; i < filter_width ; i++)
			{
				left += (float)(internal_buffer[read_index-i].left * resample_filter[i]);
				right += (float)(internal_buffer[read_index-i].right * resample_filter[i]);
			}

			*out1++ += left;
			*out2++ += right;
			nsamples--;
			internal_buf_read_index += read_step;
		}
		if (done) return NULL;
	}

	return this;
}
