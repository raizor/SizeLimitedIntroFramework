extern float musicPosNormalized;
extern float musicLengthSecs;
extern unsigned long long musicSizeBytes;
extern unsigned char* musicData;

void openAudioFileStream();
void bass_playPause(int doPause);
void bass_set_row(int row);
void audioUpdate();
int bass_is_playing();
void audioSetNormalizedPos(float normalizedPos);