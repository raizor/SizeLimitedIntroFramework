//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008                                        //
//--------------------------------------------------------------------------//

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "framework/gl/gl8k.h"

#include "intro.h"
#include "clinkster/player/clinkster.h"
#include "../shaders/shader_code.h"
#include "config.h"
#include "rocket/rocketControl.h"

#ifndef SYNC_PLAYER	
	#include "rocket/sync/device.h"
	#include <stdio.h>
#endif

GLuint vertexBufferID = 0;
GLuint indexBufferID = 0;

struct ZPointGridVertex
{
	float x, y, z, w;
	float fx, fy;
	float dummy[2]; // 32 bytes
};

typedef unsigned int ZPointGridVertexIndex;

//---------------------------------------------------------------------

#define fzn  0.005f
#define fzf  1000.0f

static const float projectionmatrix[16] = {
    1.0f, 0.00f,  0.0f,                    0.0f,
    0.0f, 1.25f,  0.0f,                    0.0f,
    0.0f, 0.00f, -(fzf+fzn)/(fzf-fzn),    -1.0f,
    0.0f, 0.00f, -2.0f*fzf*fzn/(fzf-fzn),  0.0f };

//----------------------------------------
// Initialize intro - set stuff up here
//----------------------------------------
void intro_init( void )
{
	#define GRID_SIZE 256

	// init GL extensions (defined in gl8k.h)
	for( int i=0; i<GLEXT_FUNCTION_COUNT; i++ )
	{
		myglfunc[i] = wglGetProcAddress( strs[i] );	
		if( !myglfunc[i] )
			return;
	}

	int pid = oglCreateProgram();                           
	const int vshaderTunnel = oglCreateShader( GL_VERTEX_SHADER ); 
	const int gshaderTunnel = oglCreateShader( GL_GEOMETRY_SHADER );
	const int fshaderTunnel = oglCreateShader( GL_FRAGMENT_SHADER );	
	
	/*
	oglShaderSource( vshaderTunnel, 1, &tunnel_vs_glsl, 0 );
	oglShaderSource( gshaderTunnel, 1, &tunnel_gs_glsl, 0 );
	oglShaderSource( fshaderTunnel, 1, &tunnel_fs_glsl, 0 );
	*/

	oglShaderSource( vshaderTunnel, 1, &spinner_vs_glsl, 0 );
	oglShaderSource( gshaderTunnel, 1, &spinner_gs_glsl, 0 );
	oglShaderSource( fshaderTunnel, 1, &spinner_fs_glsl, 0 );

	oglCompileShader( vshaderTunnel );
	oglCompileShader( gshaderTunnel );
	oglCompileShader( fshaderTunnel );

	oglAttachShader( pid, vshaderTunnel );
	oglAttachShader( pid, gshaderTunnel );
	oglAttachShader( pid, fshaderTunnel );
		
	oglLinkProgram( pid );

	syncDataInit();

	// create the point grid and upload to GPU
	ZPointGridVertex *vertices = new ZPointGridVertex[GRID_SIZE*GRID_SIZE];
	ZPointGridVertexIndex *indices = new ZPointGridVertexIndex[GRID_SIZE*GRID_SIZE];

	// Generate vertices
	for (int y=0; y<GRID_SIZE; y++)
	{
		for (int x=0; x<GRID_SIZE; x++)
		{
			float fx = (float)x/(float)GRID_SIZE;
			float fy = (float)y/(float)GRID_SIZE;

			int i = y*GRID_SIZE + x;

			vertices[i].x = fx;
			vertices[i].y = fy;
			vertices[i].z = 0.f;
			vertices[i].w = 1.f;
			vertices[i].fx = fx;
			vertices[i].fy = fy;

			indices[i] = i;
		}
	}

	// Update vertex data
	if (vertexBufferID==0)
		oglGenBuffers(1, &vertexBufferID);

	oglBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	oglBufferData(GL_ARRAY_BUFFER, GRID_SIZE*GRID_SIZE*sizeof(ZPointGridVertex), vertices, GL_STATIC_DRAW);
	oglBindBuffer(GL_ARRAY_BUFFER, 0);

	// Update vertex index data
	if (indexBufferID==0)
		oglGenBuffers(1, &indexBufferID);

	oglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	oglBufferData(GL_ELEMENT_ARRAY_BUFFER, GRID_SIZE*GRID_SIZE*sizeof(ZPointGridVertexIndex), indices, GL_STATIC_DRAW);
	oglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Release data
	//delete[] vertices;
	//delete[] indices;
}

//----------------------------------------
// Main Intro loop - update and draw
//----------------------------------------
void intro_do( float itime )
{
    const float ftime = itime;

#ifdef DEBUG
	// render music to cache file if needed
#endif

	UpdateRocket();

    // render
	glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
	glDisable( GL_BLEND );
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    glEnable( GL_NORMALIZE );

    glClearColor( 0.2f, 0.3f, 0.2f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_PROJECTION ); 
	glLoadMatrixf( projectionmatrix );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

	// Camera
    //glTranslatef(0.0f,0.0f, (float)sync_get_val(syncTest, (float)rocket_get_row()));
	glTranslatef(0.0f,0.0f, -10.0f);

	//oglUseProgram([pi)

	oglBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	oglEnableVertexAttribArray(0);
	oglEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);
	oglVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(ZPointGridVertex), (GLvoid*)0);
	oglVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ZPointGridVertex), (GLvoid*)(4*sizeof(float)));
	//glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(ZMeshVertex2), (GLvoid*)(7*sizeof(float)));

	oglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);

	glDrawElements(GL_POINTS, GRID_SIZE*GRID_SIZE, GL_UNSIGNED_INT, 0);

	oglBindBuffer(GL_ARRAY_BUFFER, 0);
	oglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
