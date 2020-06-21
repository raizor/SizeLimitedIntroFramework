//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008                                        //
//--------------------------------------------------------------------------//

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <math.h>
#include "framework/gl/gl8k.h"
#include "framework/gl/shader.h"
#include "framework/dialog/fwzSetup.h"
#include <GL/glu.h>

#include "intro.h"
#include "clinkster.h"
#include "shader_code_intro.h"
#include "config.h"
#include "rocket/rocketControl.h"
#include "music/syncData.h"
#include "framework/debug/watchfile.h"
#include "framework/gl/fbo.h"


#include "effects/snowscene//effect_snowscene.h"

#ifndef SYNC_PLAYER	
	#include "../rocket/sync/device.h"
	#include <stdio.h>
#endif

#include "framework/utils8k.h"
#include "global.h"

ZVector eye;
ZVector look;
ZVector cube;
ZVector objectPos;
float introTimeSecs = 0;
float introLength = 0;

GLuint vertexBufferPointGridID = 0;
GLuint vertexBufferQuadID = 0;

effect_showscene* effectSnowscene;

double rocketRow;

//---------------------------------------------------------------------

static const float projectionmatrix[16] = {
    1.0f, 0.00f,  0.0f,                    0.0f,
    0.0f, 1.25f,  0.0f,                    0.0f,
    0.0f, 0.00f, -(fzf+fzn)/(fzf-fzn),    -1.0f,
    0.0f, 0.00f, -2.0f*fzf*fzn/(fzf-fzn),  0.0f };

shader* shaderBlit;

//----------------------------------------
// Initialize intro - set stuff up here
//----------------------------------------
void intro_init( void )
{	
	AR = 1.75f;

	IntroState* ste = global::GetState();
	
	RES_X = 1280;
	RES_Y = 720;

	DRAW_X = (int)RES_X;
	DRAW_Y = (int)(RES_Y / AR);
	BAR_SIZE = (int)(RES_Y - DRAW_Y) / 2;

	glViewport(0, 0, (int)RES_X, (int)RES_Y);
	
	global::InitializeFramework(RES_X, RES_Y, 0);
	InitGlExtensions();

#ifdef DEBUG
	shaderBlit = new shader(
		"shaders\\include\\tv.vs.glsl",
		NULL,
		"shaders\\include\\tv.fs.glsl",
		"",
		"TV"
		);
#else
	shaderBlit = new shader(
		(char*)tv_vs_glsl,
		NULL,
		(char*)tv_fs_glsl,
		"",
		""
		);
#endif
	
	
//#ifdef DEBUG
	syncDataInit();
//#endif

	// create the point grid and upload to GPU (for the snow!)
	ZPointGridVertex *vertices = new ZPointGridVertex[GRID_SIZE*GRID_SIZE];

	// Generate vertices
	for (int y=0; y<GRID_SIZE; y++)
	{
		for (int x=0; x<GRID_SIZE; x++)
		{
			float fx = (float)x/(float)GRID_SIZE;
			float fy = (float)y/(float)GRID_SIZE;

			//fx-=0.5f;
			//fy-=0.5f;

			int i = y*GRID_SIZE + x; // index

			vertices[i].x = fx; // pos
			vertices[i].y = fy;
			vertices[i].z = 0.f;
			vertices[i].w = 1.f;
			vertices[i].fx = fx; // coord
			vertices[i].fy = fy;
		}
	}
		
	glGenBuffers(1, &vertexBufferPointGridID);
	glGenBuffers(1, &vertexBufferQuadID);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPointGridID);
	glBufferData(GL_ARRAY_BUFFER, GRID_SIZE*GRID_SIZE*sizeof(ZPointGridVertex), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	FBO = new ZFbo(ZFboDesc((int)(RES_X*1.0), (int)(RES_Y*1.0), 0, GL_RGBA, GL_RGBA32F_ARB, GL_FLOAT, 1, true, GL_DEPTH_COMPONENT32, GL_FLOAT, false, GL_TEXTURE_2D));
	FBO_CULLING = new ZFbo(ZFboDesc((int)(RES_X*1.0), (int)(RES_Y*1.0), 0, GL_RGBA, GL_RGBA32F_ARB, GL_FLOAT, 1, true, GL_DEPTH_COMPONENT32, GL_FLOAT, false, GL_TEXTURE_2D));

	effectSnowscene = new effect_showscene();
}

#ifdef DEBUG

	void ReloadShaders()
	{
		//shaderSpinner->reload();
		//ppSsao->prog->reload();
		effectSnowscene->reload();
	}

#endif

	// vars
float camPos[3] = {0.0f, 0.0f, 0.0f};
float lightPos[3] = {-1.5f, 1.5f, 15.0f};
float objColor[4] = {0.2f, 0.29f, 0.39f, 1.0f};

void DrawFbo(float time)
{
	glClearColor(0, 0, 0, 0);	
	glClear( GL_COLOR_BUFFER_BIT );

	glClearColor(0, 0, 0, 0);	
	glClear( GL_DEPTH_BUFFER_BIT );

	shaderBlit->SetActive(true);

	float vp[2] = {(float)RES_X, (float)RES_Y};
	//effectTv->Render(0, time, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, vp );

	//int iLoc = glGetUniformLocation(shaderBlit->shaderProg, "t");
	//glUniform1i(iLoc, 0);

	GLuint iLoc = glGetUniformLocation(shaderBlit->shaderProg, "input0");
	glUniform1i(iLoc, 0);

	float sz [] = {(float)RES_X, (float)RES_Y};
	iLoc = glGetUniformLocation(shaderBlit->shaderProg, "viewportSize");
	glUniform2fv(iLoc, 1, sz);

	iLoc = glGetUniformLocation(shaderBlit->shaderProg, "fTime");
	glUniform1f(iLoc, time);

	iLoc = glGetUniformLocation(shaderBlit->shaderProg, "fFlashAmount");
	glUniform1f(iLoc, 0.0);

	iLoc = glGetUniformLocation(shaderBlit->shaderProg, "fReflectionSize");
	glUniform1f(iLoc, 0.0);

	iLoc = glGetUniformLocation(shaderBlit->shaderProg, "fDeformValue");
	glUniform1f(iLoc,  (float)sync_get_val(screenDeform, rocketRow));

	iLoc = glGetUniformLocation(shaderBlit->shaderProg, "fScreenSize1");
	glUniform1f(iLoc, (float)sync_get_val(screenSize1, rocketRow));

	iLoc = glGetUniformLocation(shaderBlit->shaderProg, "fScreenSize2");
	glUniform1f(iLoc, (float)sync_get_val(screenSize2, rocketRow));

	iLoc = glGetUniformLocation(shaderBlit->shaderProg, "fScreenSize3");
	glUniform1f(iLoc, (float)sync_get_val(screenSize3, rocketRow));

	// reset the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// ortho view
	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//glOrtho( 0, XRES , YRES , 0, 0, 1 );             // Select Ortho Mode (ie.640x480)
	glColor4f(1,1,1,1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, FBO->colorBufferID[0]);
	//glBindTexture(GL_TEXTURE_2D, fbo->depthBufferID);
	
	// draw fbo to screen
	glRects(-1, -1, 1, 1);

	glBindTexture(GL_TEXTURE_2D, 0);

	//glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	shaderBlit->SetActive(false);

	//effectTv->EndRender();
}

//----------------------------------------
// Main Intro loop - update and draw
//----------------------------------------
void intro_do( float itime )
{
	/*	
	glEnable( GL_CULL_FACE );
	glDisable( GL_BLEND );
	glEnable( GL_NORMALIZE );
	*/

	//glEnable( GL_DEPTH_TEST );

	rocketRow = rocket_get_row();

#if DEBUG
	// reload shader if updated
	shaderBlit->refresh();
#endif
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);

	// rocket
	UpdateRocket();
	global::GetState()->eye.x = (float)sync_get_val(syncEyeX, rocketRow);
	global::GetState()->eye.y = (float)sync_get_val(syncEyeY, rocketRow);
	global::GetState()->eye.z = (float)sync_get_val(syncEyeZ, rocketRow);

	const double rocketRow = rocket_get_row();

	glMatrixMode( GL_PROJECTION ); 
	glLoadIdentity();	
	gluPerspective(60.0, (double)RES_X/(double)RES_Y,  fzn, fzf);
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
				
	effectSnowscene->draw(itime);

	DrawFbo(itime);
}

float sfrand( int *seed )
{
    float res;

    seed[0] *= 16807;

    *((unsigned int *) &res) = ( ((unsigned int)seed[0])>>9 ) | 0x40000000;

    return( res-3.0f );
}

float floor4k(float x) {
	int ix = (int) x;
	if((float) ix < x) ix--;
	return (float) ix;
}
