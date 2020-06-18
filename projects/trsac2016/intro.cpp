//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008                                        //
//--------------------------------------------------------------------------//

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include <windows.h>
#include <math.h>
#include "framework/gl/gl8k.h"
#include "framework/dialog/fwzSetup.h"
#include <GL/glu.h>

#include "intro.h"
#include "config.h"
#include "global.h"

#ifndef SYNC_PLAYER	
	#include "../rocket/sync/device.h"
	#include <stdio.h>
#endif

#include "effects/rm/rm.h"
#include "scenes/base.h"
#include "scenes/ice_tunnel.h"
#include "scenes/text.h"
#include "scenes/intro_fnuque.h"
#include "scenes/bucky_greets.h"
#include "scenes/outtro.h"

GLuint vertexBufferPointGridID = 0;
GLuint vertexBufferQuadID = 0;

//effect_glenz* effect_glenz_water;
rm* effect_terrain;
effect_base* scene_base;
ice_tunnel* scene_tunnel;
effect_text* scene_text;
effect_fnuque* scene_fnuque;
effect_outro* scene_outtro;
effect_greets* scene_greets;
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
	#define GRID_SIZE 256

	DRAW_X = RES_X;
	DRAW_Y = (int)(RES_Y / AR);
	BAR_SIZE = (RES_Y - DRAW_Y) / 2;

	glViewport(0, 0, RES_X, RES_Y);
	
	InitializeFramework((float)RES_X, (float)RES_Y, 0);
	InitGlExtensions();

#ifdef DEBUG
	shaderBlit = new shader(
		"shaders\\include\\tv.vert",
		NULL,
		"shaders\\include\\tv.frag",
		"",
		"tv"
		);
#else
	shaderBlit = new shader(
		(char*)tv_vs_glsl,
		NULL,
		(char*)tv_fs_glsl,
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
	global::GetState()->fboCulling = new ZFbo(ZFboDesc((int)(RES_X*1.0), (int)(RES_Y*1.0), 0, GL_RGBA, GL_RGBA32F_ARB, GL_FLOAT, 1, true, GL_DEPTH_COMPONENT32, GL_FLOAT, false, GL_TEXTURE_2D));

	effect_terrain = new rm();
	scene_base = new effect_base(0, 0, 0, 0);
	scene_tunnel = new ice_tunnel(0, 0, 0, 0);
	scene_text = new effect_text(0, 0, 0, 0);
	scene_fnuque = new effect_fnuque(0, 0, 0, 0);
	scene_greets = new effect_greets(0, 0, 0, 0);
	scene_outtro = new effect_outro(0, 0, 0, 0);
}

#ifdef DEBUG

	void ReloadShaders()
	{
		//shaderSpinner->reload();
		//ppSsao->prog->reload();
		effect_terrain->reload();
		scene_base->reload();
		scene_tunnel->reload();
		scene_text->reload();
		scene_fnuque->reload();
		scene_greets->reload();
		scene_outtro->reload();
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
	glUniform2fv(glGetUniformLocation(shaderBlit->shaderProg, "viewportSize"), 1, sz);
	glUniform1f(glGetUniformLocation(shaderBlit->shaderProg, "fTime"), time);
	/*


	glUniform1i(glGetUniformLocation(shaderBlit->shaderProg, "tv_tube_lines"), 0);
	glUniform1f(glGetUniformLocation(shaderBlit->shaderProg, "tv_lines"), 0.5f);
	glUniform1f(glGetUniformLocation(shaderBlit->shaderProg, "tv_lines_speed"), 0.1f);
	glUniform1f(glGetUniformLocation(shaderBlit->shaderProg, "tv_lines_opacity"), 0.5f);
	
	glUniform1i(glGetUniformLocation(shaderBlit->shaderProg, "vhs_stripes"), 1);*/
	/*
	glUniform1i(glGetUniformLocation(shaderBlit->shaderProg, "vhs_bars"), 1);
	glUniform1f(glGetUniformLocation(shaderBlit->shaderProg, "bars_count"), 1.0f);
	*/
	/*
	glUniform1i(glGetUniformLocation(shaderBlit->shaderProg, "scanline_scan"), 1);
	glUniform1f(glGetUniformLocation(shaderBlit->shaderProg, "pScanline"), 1.0f);
	glUniform1i(glGetUniformLocation(shaderBlit->shaderProg, "scanline_distort"), 1);
	*/

	glUniform1i(glGetUniformLocation(shaderBlit->shaderProg, "rgb_offset"), 0);
	glUniform1f(glGetUniformLocation(shaderBlit->shaderProg, "rgb_offset_amount"), 0.5f);

	glUniform1i(glGetUniformLocation(shaderBlit->shaderProg, "add_vignette"), 0);
	glUniform1f(glGetUniformLocation(shaderBlit->shaderProg, "pVignettescale"), 0.5f);
	glUniform1f(glGetUniformLocation(shaderBlit->shaderProg, "pVignettessoftness"), 1.0f);
	
	//glUniform1i(glGetUniformLocation(shaderBlit->shaderProg, "vcr_distortion"), 1);
	//glUniform1f(glGetUniformLocation(shaderBlit->shaderProg, "bars_count"), 1.0f);
	
	/*
	glUniform1i(glGetUniformLocation(shaderBlit->shaderProg, "slow_scan"), 1); // pulse
	glUniform1f(glGetUniformLocation(shaderBlit->shaderProg, "pSlowscan"), 0.5f);
	*/

	//glUniform1i(glGetUniformLocation(shaderBlit->shaderProg, "tv_tube_vignette"), 1);



	
		/*
	iLoc = glGetUniformLocation(shaderBlit->shaderProg, "fFlashAmount");
	glUniform1f(iLoc, 0.0);

	iLoc = glGetUniformLocation(shaderBlit->shaderProg, "fReflectionSize");
	glUniform1f(iLoc, 0.0);

	iLoc = glGetUniformLocation(shaderBlit->shaderProg, "fDeformValue");
	glUniform1f(iLoc,  (float)sync_get_val(screenDeform, ROCKET_ROW));

	iLoc = glGetUniformLocation(shaderBlit->shaderProg, "fScreenSize1");
	glUniform1f(iLoc, (float)sync_get_val(screenSize1, ROCKET_ROW));

	iLoc = glGetUniformLocation(shaderBlit->shaderProg, "fScreenSize2");
	glUniform1f(iLoc, (float)sync_get_val(screenSize2, ROCKET_ROW));

	iLoc = glGetUniformLocation(shaderBlit->shaderProg, "fScreenSize3");
	glUniform1f(iLoc, (float)sync_get_val(screenSize3, ROCKET_ROW));
	*/
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

	ROCKET_ROW = rocket_get_row();
	// get global sync vars
	SYNC_CAM_RADIUS = (float)sync_get_val(camRadius, ROCKET_ROW);
	SCENE_NUM = global::GetState()->sceneNum = (float)sync_get_val(sceneNum, ROCKET_ROW);

#if DEBUG
	// reload shader if updated
	shaderBlit->refresh();
#endif
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	UpdateRocket();

	const double rocketRow = rocket_get_row();

	glMatrixMode( GL_PROJECTION ); 
	glLoadIdentity();	
	gluPerspective(60.0, (double)RES_X/(double)RES_Y,  fzn, fzf);
	
	//glLoadMatrixf( projectionmatrix );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glClearColor(0, 0, 0, 0);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	switch ((int)SCENE_NUM)
	{
	case 0:
		{			
			// disk
			scene_text->draw(itime);
			break;
		}
	case 1:
		{
			//fnuque logo
			scene_fnuque->draw(itime);
			break;
		}
	case 2:
		{
			// water
			effect_terrain->draw(itime);
			break;
		}
	case 3:
		{
			// ice tunnel
			scene_tunnel->draw(itime);
			break;
		}
	case 4:
		{
			//greets
			scene_greets->draw(itime);
			break;
		}
	case 5:
		{
			//outtro
			scene_outtro->draw(itime);
			break;
		}

	default:
		{
			break;
		}
	}	

#if ENABLE_PP
	DrawFbo(itime);
#endif
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
