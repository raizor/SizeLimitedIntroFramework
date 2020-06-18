//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008                                        //
//--------------------------------------------------------------------------//

#pragma once

//#define SETRESOLUTION

#define M_PI 3.141926

extern int RES_X;
extern int RES_Y;
extern int DRAW_X;
extern int DRAW_Y;
extern int BAR_SIZE;
extern float AR;

#define INTRO_TITLE "Eventide Mini Framework v0.1"

#define ENABLE_ROCKET

#define MUTE_AUDIO

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

#include "zassert.h"