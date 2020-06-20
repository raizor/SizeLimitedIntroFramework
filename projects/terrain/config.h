//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008                                        //
//--------------------------------------------------------------------------//

#pragma once

//#define SETRESOLUTION

#define RES_X        800
#define RES_Y       600

#define INTRO_TITLE "Eventide Mini Framework v0.1"

#define ENABLE_ROCKET

#define MUTE_AUDIO

#ifdef DEBUG
	#define ENABLE_SCRUBBER
#else
	#define SYNC_PLAYER
#endif

#define CLINKSTER_EVENTS
#define VBO_INDEX_BUFFER

// utils toggles

#define ENABLE_ZSTREQL

