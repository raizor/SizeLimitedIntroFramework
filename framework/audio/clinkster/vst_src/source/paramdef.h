// name, title, displayfunc, min, max, default, quantization levels, post-quantization clamp

PARAM(Waveform1, "BaseWave", Wave, 0.f, 1.f, 0.0f, 6, 0)
PARAM(Waveform2, "ModWave", Wave, 0.f, 1.f, 0.0f, 6, 0)
PARAM(DetuneSpread1, "B Detune", Detune, 0.0f, 29.00242657210449f, 1.4501213286f, 101, 0)
PARAM(DetuneSpread2, "M Detune", Detune, 0.0f, 29.00242657210449f, 1.4501213286f, 101, 0)
PARAM(PitchStrength1, "B Pitch", Pitch, -10.0f, 10.0f, 0.0f, 241, 0)
PARAM(PitchDecay1, "B PitchD", Decay, -7.010401204f, 3.943350677253f, 0.0f, 201, 0)
PARAM(PitchStrength2, "M Pitch", Pitch, -10.0f, 10.0f, 0.0f, 241, 0)
PARAM(PitchDecay2, "M PitchD", Decay, -7.010401204f, 3.943350677253f, 0.0f, 201, 0)
PARAM(Index, "Index", Float, 0.f, 6.25f, 0.3125f, 101, 0)
PARAM(IndexSpread, "IndexSpr", Float, 0.f, 6.25f, 0.3125f, 101, 0)
PARAM(IndexDecay, "IndexD", Decay, -7.010401204f, 3.943350677253f, 0.0f, 201, 0)
PARAM(Layers, "Layers", Integer, 0.f, 50.f, 5.0f, 51, 1)
PARAM(RandomSeed, "RandomSeed", Integer, 0.f, 127.f, 5.0f, 128, 0)	
PARAM(Attack, "Attack", Envelope, -16.0f, 9.0f, -4.0f, 201, 0)
PARAM(Decay, "Decay", Envelope, -16.0f, 9.0f, 0.0f, 201, 0)
PARAM(Sustain, "Sustain", Float, -1.0f, 1.0f, 0.375f, 65, 0)
PARAM(Release, "Release", Envelope, -16.0f, 9.0f, 1.5f, 201, 0)
PARAM(Gain, "Gain", ExpFloat, -5.0f, 7.5f, 0.0f, 201, 0)

/*
Changes to v2:

Detune:        Half range, same factor
PitchStrength: Range shifted, same factor
PitchDecay:    Range shifted, same factor with opposite sign
Index:         Same range, double factor
IndexSpread:   Same range, double factor
RandomSeed:    Range shifted, same factor
Attack:        Same range, double factor
Decay:         Double range, quadruple factor
Sustain:       Double shifted range, double factor
Release:       Same range, double factor
Gain:          Double range, double factor

*/

