
#include "vstxsynth.h"

#define _USE_MATH_DEFINES
#include <math.h>

const unsigned int kNumFrequencies = 128;	// 128 midi notes
const unsigned int kWaveSize = 65536;		// samples (must be power of 2 here)

#define PARAM(name, title, displayfunc, min, max, def, quan, low) \
	const float MIN_##name = min; \
	const float MAX_##name = max; \
	const float QUAN_##name = quan;
#include "paramdef.h"
#undef PARAM
#define P(name) (program.f##name * (MAX_##name - MIN_##name) + MIN_##name)
#define I(name) (iround((program.f##name + MIN_##name / (MAX_##name - MIN_##name)) * (QUAN_##name - 1)))

const double midiScaler = (1. / 127.);
static float sine[kWaveSize];
static float sawtooth[kWaveSize];
static float parabola[kWaveSize];
static float square[kWaveSize];
static float noise[kWaveSize];
static float triangle[kWaveSize];
static double freqtab[kNumFrequencies];

//-----------------------------------------------------------------------------------------
void VstXSynth::initProcessInternal ()
{
	VstInt32 i;

	// make waveforms
	double s = -1.0;
	VstInt32 wh = kWaveSize / 2;	// 1:1 square
	for (i = 0; i < kWaveSize; i++)
	{
		sine[i] = (float)sin(i*(2.0f*M_PI/(float)kWaveSize));
		sawtooth[i] = (float)(-1. + (2. * ((double)i / (double)kWaveSize)));
		parabola[i] = sawtooth[i] * sawtooth[i] * 2.0f - 1.0f;
		square[i] = (i < wh) ? -1.f : 1.f;
		s = sin((s + 2.0f/(float)kWaveSize) * M_PI);
		noise[i] = (float)s;
		triangle[i] = fabs(sawtooth[i])*2.0f-1.0f;
	}

	// make frequency (Hz) table
	double k = 1.059463094359;	// 12th root of 2
	double a = 6.875;	// a
	a *= k;	// b
	a *= k;	// bb
	a *= k;	// c, frequency of midi note 0
	for (i = 0; i < kNumFrequencies; i++)	// 128 midi notes
	{
		freqtab[i] = a;
		a *= k;
	}
}

#define SETPAR(par, mean, spr, fac) do { \
	layers[i][c].par = (mean + (((double)(signed short)r) / (65536.0/2.0)) * spr) * fac; \
	r = ((r >> (r & 31)) | (r << (32-(r & 31))))-1; \
} while (0)

MyNote::MyNote (VstXSynthProgram *programp, VstInt32 channel, VstInt32 note, VstInt32 velocity, VstInt32 delta, float _internal_samplerate)
	: VstXSynthNote(programp, channel, note, velocity, delta)
{
	internal_samplerate = _internal_samplerate;

	int nlayers = I(Layers);
	double freq = freqtab[note & 0x7f];
	for (int c = 0 ; c < 2 ; c++)
	{
		unsigned int r = I(RandomSeed) * 16307 + c * 12042;
		for (int i = 0 ; i < nlayers ; i++)
		{
			SETPAR(freq1, freq, (P(DetuneSpread1) * P(DetuneSpread1)), (kWaveSize / internal_samplerate));
			SETPAR(freq2, freq, (P(DetuneSpread2) * P(DetuneSpread2)), (kWaveSize / internal_samplerate));
			SETPAR(index, P(Index), P(IndexSpread), kWaveSize);
			layers[i][c].phase1 = 0.0;
			layers[i][c].phase2 = 0.0;
		}
	}
	currpitch1 = exp2(P(PitchStrength1));
	currpitch2 = exp2(P(PitchStrength2));
	pitchdecay1 = exp2(P(PitchDecay1) * P(PitchDecay1) * P(PitchDecay1) / internal_samplerate);
	pitchdecay2 = exp2(P(PitchDecay2) * P(PitchDecay2) * P(PitchDecay2) / internal_samplerate);
	indexdecay = exp2(P(IndexDecay) * P(IndexDecay) * P(IndexDecay) / internal_samplerate);
}


bool MyNote::renderInternal(VstXSynth *synth, sampleC *out, VstInt32 nsamples)
{
	float *waves[6] = { sine, sawtooth, square, parabola, triangle, noise };
	float* wave1 = waves[I(Waveform1)];
	float* wave2 = waves[I(Waveform2)];
	VstInt32 mask = kWaveSize - 1;

	// loop over samples
	int nlayers = I(Layers);
	for (int i = 0 ; i < nsamples ; i++)
	{
		// loop over layers
		float left = 0.0, right = 0.0;
		for (int l = 0 ; l < nlayers ; l++)
		{
			double w2l = wave2[iround(layers[l][0].phase2) & mask];
			left += wave1[iround(layers[l][0].phase1 + w2l * layers[l][0].index) & mask];
			layers[l][0].phase1 += layers[l][0].freq1 * currpitch1;
			layers[l][0].phase2 += layers[l][0].freq2 * currpitch2;
			layers[l][0].index *= indexdecay;

			double w2r = wave2[iround(layers[l][1].phase2) & mask];
			right += wave1[iround(layers[l][1].phase1 + w2r * layers[l][1].index) & mask];
			layers[l][1].phase1 += layers[l][1].freq1 * currpitch1;
			layers[l][1].phase2 += layers[l][1].freq2 * currpitch2;
			layers[l][1].index *= indexdecay;
		}
		out[i].left = left;
		out[i].right = right;

		currpitch1 = (currpitch1 - 1.0) * pitchdecay1 + 1.0;
		currpitch2 = (currpitch2 - 1.0) * pitchdecay2 + 1.0;
	}

	int attack = iround(exp2(P(Attack)) * ENVELOPE_BASE_TIME * internal_samplerate);
	int decay = iround(exp2(P(Decay)) * ENVELOPE_BASE_TIME * internal_samplerate);
	float sustain = P(Sustain);
	int release = iround(exp2(P(Release)) * ENVELOPE_BASE_TIME * internal_samplerate);
	int a = startOffset;
	int ad = a + attack;
	int ds = ad + decay;
	int sr = stopOffset;
	if (sr < ds) sr = ds;
	int r = sr + release;
	if (ad > nsamples) ad = nsamples;
	if (ds > nsamples) ds = nsamples;
	if (sr > nsamples) sr = nsamples;
	if (r > nsamples) r = nsamples;

	float a_slope = 1.0f / attack;
	float d_slope = (1.0f-sustain) / decay;
	float r_slope = sustain / release;

	// loop over ADSR phases
	float lfac = startVelocity * (1.0f / 127.0f) / nlayers;
	int i = 0;
	for (int p = 0 ; p < 5 ; p++)
	{
		int start, stop;
		float base, slope;
		switch (p) {
			case 0: // Attack
				start = a;
				stop = ad;
				base = 0.0f;
				slope = a_slope;
				break;
			case 1: // Decay
				start = ad;
				stop = ds;
				base = 1.0f;
				slope = -d_slope;
				break;
			case 2: // Sustain
				start = ds;
				stop = sr;
				base = sustain;
				slope = 0.0f;
				break;
			case 3: // Release
				start = sr;
				stop = r;
				base = sustain;
				slope = -r_slope;
				break;
			case 4: // Silence
				start = r;
				stop = nsamples;
				base = 0.0f;
				slope = 0.0f;
				break;
		}
		for (; i < stop ; i++)
		{
			float fac = (base + (i-start) * slope) * lfac;
			out[i].left *= fac;
			out[i].right *= fac;
		}
	}

	// apply gain
	float gain = (float) exp2(P(Gain));
	gain = gain * gain;
	for (int i = 0 ; i < nsamples ; i++)
	{
		float left = out[i].left;
		out[i].left = left * sqrt(gain / (1.0f + (gain - 1.0f) * (left * left)));

		float right = out[i].right;
		out[i].right = right * sqrt(gain / (1.0f + (gain - 1.0f) * (right * right)));
	}

	return (nsamples > r);
}
