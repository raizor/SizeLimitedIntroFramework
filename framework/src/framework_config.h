/*
 * Raizor/DESiRE 2020
 */

#ifndef _FRAMEWORK_CONFIG_H_
#define _FRAMEWORK_CONFIG_H_

//#define SETRESOLUTION

// resolution/screen setting set by parent intro project
extern float RES_X;
extern float RES_Y;
extern float ASPECT_RATIO;

extern float DRAW_X;
extern float DRAW_Y;

extern float BAR_SIZE; // black bars at bottom/top for widescreen/letterbox

extern char* introTitle; // set by intro

#define M_PI 3.141926

#define ENABLE_ROCKET

#ifdef DEBUG
	#define ENABLE_SCRUBBER
	#define ENABLE_MUSIC	
#else
	#define SYNC_PLAYER
#endif

#define CLINKSTER_EVENTS
#define VBO_INDEX_BUFFER

// utils toggles

#define ENABLE_ZSTREQL

void InitializeFramework(float viewportWidth, float viewportHeight, int randSeed);

#endif

