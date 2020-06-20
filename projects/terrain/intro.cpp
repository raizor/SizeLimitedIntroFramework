//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008                                        //
//--------------------------------------------------------------------------//

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <math.h>
#include "framework/gl/gl8k.h"
#include "framework/gl/shader.h"
#include <GL/glu.h>

#include "intro.h"
#include "clinkster.h"
#include "shaders/shader_code.h"
#include "config.h"
#include "../rocket/rocketControl.h"
#include "music/syncData.h"
#include "framework/debug/watchfile.h"
#include "framework/gl/fbo.h"
#include "framework/effects/pp/ssao/ssao.h"
#include "effects\logo\logo.h"
#include "effects\terrain\terrain.h"

#ifndef SYNC_PLAYER	
	#include "../rocket/sync/device.h"
	#include <stdio.h>
#endif

#include "framework/utils8k.h"

GLuint vertexBufferPointGridID = 0;
GLuint vertexBufferQuadID = 0;

struct ZPointGridVertex
{
	float x, y, z, w;
	float fx, fy;
	float dummy[2]; // 32 bytes
};

typedef unsigned int ZPointGridVertexIndex;

ZFbo* fbo;

logo* effectLogo;
terrain* effectTerrain;

//---------------------------------------------------------------------

#define fzn  0.05f
#define fzf  200.0f

static const float projectionmatrix[16] = {
    1.0f, 0.00f,  0.0f,                    0.0f,
    0.0f, 1.25f,  0.0f,                    0.0f,
    0.0f, 0.00f, -(fzf+fzn)/(fzf-fzn),    -1.0f,
    0.0f, 0.00f, -2.0f*fzf*fzn/(fzf-fzn),  0.0f };

ssao* ppSsao;

shader* shaderSpinner;
shader* shaderBlit;

//----------------------------------------
// Initialize intro - set stuff up here
//----------------------------------------
void intro_init( void )
{
	#define GRID_SIZE 256

	glViewport(0, 0, RES_X, RES_Y);
	
	InitializeFramework(RES_X, RES_Y, 0);
	InitGlExtensions();

#ifdef DEBUG
	shaderSpinner = new shader(
		"shaders\\include\\spinner.vs.glsl",
		"shaders\\include\\spinner.gs.glsl",
		"shaders\\include\\spinner.fs.glsl"
		);

	shaderBlit = new shader(
		"shaders\\include\\blit.vert",
		NULL,
		"shaders\\include\\blit.frag"
		);
#else
	shaderSpinner = new shader(
		&spinner_vs_glsl,
		&spinner_gs_glsl,
		&spinner_fs_glsl,
		);
#endif
	
	ppSsao = new ssao();
	
#ifdef DEBUG
	syncDataInit();
#endif
		
	// create the point grid and upload to GPU
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

	fbo = new ZFbo(ZFboDesc(RES_X, RES_Y, 0, GL_RGBA, GL_RGBA32F_ARB, GL_FLOAT, 3, true, GL_DEPTH_COMPONENT32, GL_FLOAT, false, GL_TEXTURE_2D));

	effectLogo = new logo();
	effectTerrain = new terrain();
}

#ifdef DEBUG

	void ReloadShaders()
	{
		shaderSpinner->reload();
		ppSsao->prog->reload();
		effectTerrain->reload();
	}

#endif

	// vars
float camPos[3] = {0.0f, 0.0f, 0.0f};
float lightPos[3] = {-1.5f, 1.5f, 15.0f};
float objColor[4] = {0.2f, 0.29f, 0.39f, 1.0f};

void DrawFbo()
{

	glClearColor(0, 0, 0, 0);	
	glClear( GL_COLOR_BUFFER_BIT );

	glClearColor(0, 0, 0, 0);	
	glClear( GL_DEPTH_BUFFER_BIT );

	shaderBlit->SetActive(true);

	int iLoc = glGetUniformLocation(shaderBlit->shaderProg, "t");
	glUniform1i(iLoc, 0);

	// reset the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// ortho view
	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//glOrtho( 0, XRES , YRES , 0, 0, 1 );             // Select Ortho Mode (ie.640x480)
	glColor4f(1,1,1,1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo->colorBufferID[0]);
	//glBindTexture(GL_TEXTURE_2D, fbo->depthBufferID);
	
	// draw fbo to screen
	glRects(-1, -1, 1, 1);

	glBindTexture(GL_TEXTURE_2D, 0);

	//glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	shaderBlit->SetActive(false);
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

#if DEBUG
	// reload shader if updated
	shaderSpinner->refresh();
#endif

	UpdateRocket();

	const double rocketRow = rocket_get_row();

	glMatrixMode( GL_PROJECTION ); 
	glLoadIdentity();
	gluPerspective(60.0, (double)XRES/(double)YRES,  0.05f, 200.0f);
	
	//glLoadMatrixf( projectionmatrix );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
		
	effectTerrain->draw(itime);

	//effectTerrain->test(itime);


	DrawFbo();
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
