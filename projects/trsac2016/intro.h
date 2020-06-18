//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008                                        //
//--------------------------------------------------------------------------//

#ifndef _INTRO_H_
#define _INTRO_H_

void intro_init( void );
void intro_do( float time );
void intro_end( void );

void draw_cube( void );

float sfrand( int *seed );

float floor4k(float x);

#endif