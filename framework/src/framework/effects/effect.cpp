#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include "effect.h"

#include <framework_config.h>

#include "framework/utils8k.h"

effect::effect()
{


}

void effect::Render()
{
	// reset the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// ortho view
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glOrtho( 0, RES_X , RES_Y , 0, 0, 1 );             // Select Ortho Mode (ie.640x480)

	glClearColor(0, 0, 0, 0);
  	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void effect::EndRender()
{
	glPopMatrix();
}