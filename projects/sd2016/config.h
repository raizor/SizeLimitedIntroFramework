//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008                                        //
//--------------------------------------------------------------------------//

#pragma once

//#define SETRESOLUTION

//#define XRES        800
//define YRES        600

#define INTRO_TITLE "Eventide Mini Framework v0.1"

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

