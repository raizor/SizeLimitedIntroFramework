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
#include "clinkster/player/clinkster.h"
#include "shader_code_intro.h"
#include "config.h"
#include "rocket/rocketControl.h"
#include "music/syncData.h"
#include "framework/debug/watchfile.h"
#include "framework/gl/fbo.h"
//#include "effects\terrain\terrain.h"
#include "effects\particles\Farticles.h"
#include "effects\particles\Trendwhore.h"
#include "effects\particles\Noise3D.h"

#ifndef SYNC_PLAYER	
	#include "rocket/sync/device.h"
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

ZFbo  *fbo, *fboCulling, *fboRender;

//terrain* effectTerrain;

int RES_X = 0;
int RES_Y = 0;
int DRAW_X = 0;
int DRAW_Y = 0;
int BAR_SIZE = 0;

float AR = 1.75f;

double rocketRow;

//---------------------------------------------------------------------

#define fzn  1.00f
#define fzf  1000.0f

static const float projectionmatrix[16] = {
    1.0f, 0.00f,  0.0f,                    0.0f,
    0.0f, 1.25f,  0.0f,                    0.0f,
    0.0f, 0.00f, -(fzf+fzn)/(fzf-fzn),    -1.0f,
    0.0f, 0.00f, -2.0f*fzf*fzn/(fzf-fzn),  0.0f };



shader* shaderBlit;
Trendwhore* trendwhore;


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
		"shaders\\include\\tv.vs.glsl",
		NULL,
		"shaders\\include\\tv.fs.glsl",
		""
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

	trendwhore = new Trendwhore();
	GLuint noiseTex = Noise3D::GetHandle();
		
	/*
	glGenBuffers(1, &vertexBufferPointGridID);
	glGenBuffers(1, &vertexBufferQuadID);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPointGridID);
	glBufferData(GL_ARRAY_BUFFER, GRID_SIZE*GRID_SIZE*sizeof(ZPointGridVertex), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	fbo = new ZFbo(ZFboDesc((int)(RES_X*1.0), (int)(RES_Y*1.0), 0, GL_RGBA, GL_RGBA32F_ARB, GL_FLOAT, 1, true, GL_DEPTH_COMPONENT32, GL_FLOAT, false, GL_TEXTURE_2D));
	fboCulling = new ZFbo(ZFboDesc((int)(RES_X*1.0), (int)(RES_Y*1.0), 0, GL_RGBA, GL_RGBA32F_ARB, GL_FLOAT, 1, true, GL_DEPTH_COMPONENT32, GL_FLOAT, false, GL_TEXTURE_2D));
	*/
	//effectTerrain = new terrain();
}

#ifdef DEBUG

	void ReloadShaders()
	{
		//shaderSpinner->reload();
		//ppSsao->prog->reload();
		//effectTerrain->reload();
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
	glBindTexture(GL_TEXTURE_2D, fbo->colorBufferID[0]);
	//glBindTexture(GL_TEXTURE_2D, fbo->depthBufferID);
	
	// draw fbo to screen
	glRects(-1, -1, 1, 1);

	glBindTexture(GL_TEXTURE_2D, 0);

	//glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	shaderBlit->SetActive(false);

	

	//effectTv->EndRender();
}

extern double lastTime;
extern float timeStep;

//----------------------------------------
// Main Intro loop - update and draw
//----------------------------------------
void intro_do(double itime)
{
	timeStep = (float)(itime - lastTime);

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
	
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	//glEnable(GL_BLEND);
	UpdateRocket();

	const double rocketRow = rocket_get_row();

	glMatrixMode( GL_PROJECTION ); 
	glLoadIdentity();	
	gluPerspective(60.0, (double)RES_X/(double)RES_Y,  fzn, fzf);
	
	//glLoadMatrixf( projectionmatrix );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

//	float  tt = (float)sync_get_val(syncTrans, rocketRow);
			
	//effectTerrain->draw(itime);

	//effectTerrain->test(itime);

	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo->id);
	glClearColor(0, 0, 0, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glColor4f(1, 1, 1, 1);
	glPushMatrix();
	//blockEffect->draw((float)itime);
	trendwhore->Run(itime);
	glPopMatrix();
	// unbind FBO
	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);


	//DrawFbo(itime);

	lastTime = itime;
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
