//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008                                        //
//--------------------------------------------------------------------------//

#ifndef _INTRO_H_
#define _INTRO_H_
#include "framework/math/vector.h"

extern ZVector eye;
extern ZVector look;
extern ZVector cube;
extern ZVector objectPos;
extern float introTimeSecs;
extern float introLength;

void intro_init( void );
void intro_do( float time );
void intro_end( void );

void draw_cube( void );

float sfrand( int *seed );

float floor4k(float x);

#define INTRO_DEBUG_USE_SETUP_DIALOG  false
#define INTRO_RELEASE_USE_SETUP_DIALOG true

#endif
