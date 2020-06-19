//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008                                        //
//--------------------------------------------------------------------------//

#pragma once

#include "global.h"

//#define SETRESOLUTION

#define MUTE_AUDIO

#define GRID_SIZE 256

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

//INTRO_TITLE = "XMAS 2013";