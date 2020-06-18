//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008                                        //
//--------------------------------------------------------------------------//

#pragma once

//#define SETRESOLUTION

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