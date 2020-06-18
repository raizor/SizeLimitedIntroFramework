#include "framework_config.h"

float RES_X = 0;
float RES_Y = 0;
float ASPECT_RATIO = 0;

float DRAW_X = 0;
float DRAW_Y = 0;

float BAR_SIZE = 0;

int* randomSeed = 0;
char* introTitle = 0;

void InitializeFramework(float viewportWidth, float viewportHeight, int randSeed)
{
	RES_X = viewportWidth;
	RES_Y = viewportHeight;
#ifdef ENABLE_RANDOM
	randomSeed = new int();
	*randomSeed = randSeed;
#endif
}