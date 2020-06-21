#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <math.h>
#include "effect_snowscene.h"
#include "framework/gl/gl8k.h"
#include "framework/gl/shader.h"
#include <GL/glu.h>

#include "clinkster.h"
#include "../../shader_code_intro.h"
#include "config.h"
#include "rocket/rocketControl.h"
#include "../../music/syncData.h"
#include "framework/gl/fbo.h"
#include "framework\math\matrix.h"

#ifndef SYNC_PLAYER	
	#include "../rocket/sync/device.h"
	#include <stdio.h>
#endif

#include "framework/utils8k.h"

#include "particles.h"

#include "global.h"

/*
 
#define fzn  1.0f
#define fzf  1000.0f
*/
#define DEG_PER_RAD 0.0174532925f

//ZVector eye, lookAt, cubePos;

extern ZVector cube;

extern double rocketRow;

float sunPos[3];

ZMatrix *mvMat;

//quatCam* cam;

ZFbo* fboPicker = 0;
ZFbo* fboTexture = 0;

particles* parts;


struct pixel_rgba
{
	float r, g, b, a;
};

struct pixel_rgb
{
	float r, g, b;
};

static short opc1 = 0x043f ;     // floor

int ifloorf( const float x )
{
	int res;
	short tmp;

	_asm fstcw   word  ptr [tmp]
	_asm fld     dword ptr [x]
	_asm fldcw   word  ptr [opc1]
	_asm fistp   dword ptr [res]
	_asm fldcw   word  ptr [tmp]

	return res;
}
//
//void terrain::makeNoise3d()
//{
//	ZRandom* random = new ZRandom(0);
//	int seed = 0;
//
//	shader* noiseShader = new shader("shaders\\include\\blit.vert",
//		NULL,
//		"shaders\\include\\Noise3DLinear.frag",
//		""
//		);
//
//	
//	for (int i = 0; i < 1; i++)
//	{
//		// Large texture
//		glGenTextures(1, &textureNoise3dFix);
//		glBindTexture(GL_TEXTURE_3D, textureNoise3dFix);
//		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
//		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB8, 256, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
//
//		/*
//		// Small texture
//		unsigned char *texels = new unsigned char[16 * 16 * 16];
//		for (int i = 0; i < 16 * 16 * 16; i++) texels[i] = random->randomInt() & 255;
//		GLuint smallTex;
//		glGenTextures(1, &smallTex);
//		glBindTexture(GL_TEXTURE_3D, smallTex);
//		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
//		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB8, 16, 16, 16, 0, GL_RED, GL_UNSIGNED_BYTE, texels);
//		delete [] texels;
//		*/
//		
//		// Layer fbo
//		GLuint layerFbo;
//		glGenFramebuffersEXT(1, &layerFbo);
//		glBindFramebufferEXT(GL_FRAMEBUFFER, layerFbo);
//		
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, textureNoise);
//
//		noiseShader->SetActive(true);
//		
//		// Generate large noise texture
//		for (int i = 0; i < 256; i++)
//		{
//			glFramebufferTextureLayerEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureNoise3dFix, 0, i);
//			glUniform1i(glGetUniformLocation(noiseShader->shaderProg, "t"), 0);
//			glUniform1i(glGetUniformLocation(noiseShader->shaderProg, "textureLayer"), i);
//			glRecti(-1, -1, 1, 1);
//		}
//				
//		// Cleanup
//		noiseShader->SetActive(false);
//		glBindTexture(GL_TEXTURE_2D, 0);
//		glBindTexture(GL_TEXTURE_3D, 0);
//		glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
//	}
//}

/*
////////////////////////////////////////////////////////////////////
// Allocate memory and load targa bits. Returns pointer to new buffer,
// height, and width of texture, and the OpenGL format of data.
// Call free() on buffer when finished!
// This only works on pretty vanilla targas... 8, 24, or 32 bit color
// only, no palettes, no RLE encoding.
GLbyte *gltLoadPNG(const unsigned char* data, unsigned int dataSize, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat)
{	
	//stbi_load_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp)';
	int req_comp = 4;
	GLbyte* pixels = (GLbyte*)stbi_load_from_memory((unsigned char*)data, dataSize, iWidth, iHeight, iComponents, req_comp);

	*eFormat = GL_RGBA;
	*iComponents = GL_RGBA8;

	return pixels;
}
*/

float fmix(float x, float y, float a)
{
	 return x * ( 1.0f - a ) + y * a;
}

effect_showscene::effect_showscene(void)
{
	for(int i=0; i<8*6*4; i++)
	{
		//cubeVerts[i] *= 0.25f;
	}

	//quadric = gluNewQuadric(); 

	txt3d = new text3d();

	//cam = new quatCam();

	//cam->eyePos.z = 2;

#ifdef DEBUG

	shaderTerrainPlain = new shader(
		"shaders\\include\\terrainTest.vert",
		NULL,
		"shaders\\include\\terrainTest.frag",
		"", "TER_LOGO");
	
	shaderTerrainLogo = new shader(
		"shaders\\include\\terrainTest.vert",
		NULL,
		"shaders\\include\\terrainTest.frag",
		"#define LOGO\n",
		"TER_PLAIN"
		);
		
	
	shaderTerrainSnowman = new shader(
		"shaders\\include\\terrainTest.vert",
		NULL,
		"shaders\\include\\terrainTest.frag",
		"#define SNOWMAN\n",
		"TER_SNOWMAN");
		
#else
	
	shaderTerrainPlain = new shader(
		(char*)terrainTest_vert,
		NULL,
		(char*)terrainTest_frag,
		"",
		""
		);

	shaderTerrainLogo = new shader(
		(char*)terrainTest_vert,
		NULL,
		(char*)terrainTest_frag,
		"#define LOGO\n",
		""
		);

	shaderTerrainSnowman = new shader(
		(char*)terrainTest_vert,
		NULL,
		(char*)terrainTest_frag,
		"#define SNOWMAN\n",
		""
		);
#endif

	int width = 256;
	int height = 256;
	ZRandom* zr = new ZRandom(0);
			
	int adr;
	int adr2;
	
	pixel_rgb* pix = new pixel_rgb[256*256];
	
	// gen
	for (int y=0;y<256;y++)
	{
		for (int x=0;x<256;x++)
		{
			adr=x+256*y;
			pix[adr].r=zr->randomNormalizedFloat();			
			pix[adr].g=0;
			pix[adr].b=zr->randomNormalizedFloat();
		}
	}
	
	// offset green channel
	for (int y=0;y<256;y++)
	{
		for (int x=0;x<256;x++)
		{
			adr=x+256*y;
			adr2=((x+37)&255)+256*((y+17)&255);
			pix[adr2].g=pix[adr].r;
		}
	}
	
	glGenTextures(1, &textureNoise);
	glBindTexture(GL_TEXTURE_2D, textureNoise);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, pix );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering
		
	//pixels = gltLoadPNG(tex01_png, tex01_png_size, &width, &height, &icomps, &eFormat);
	glGenTextures(1, &textureColor);

	glBindTexture(GL_TEXTURE_2D, textureColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, pix );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	
	/*
	glGenTextures(1, &textureNoise3d);
	glBindTexture(GL_TEXTURE_3D, textureNoise3d);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB8, 256, 256, 256, 0, GL_RED, GL_FLOAT, zpix);
	glBindTexture(GL_TEXTURE_3D, 0);
	*/
	//noise3d = new noise();

	//makeNoise3d();
	/*
	path1 = new ZSplinePath();

	path1->AddPoint(new ZVector(-1.0f, -1.9f, -0.0f));

	path1->AddPoint(new ZVector(-0.5f, -0.0f, -0.5f));

	path1->AddPoint(new ZVector(-0.02f, -2.2f, -1.0f));

	path1->AddPoint(new ZVector(-0.30f, -0.5f, -1.5f));

	path1->GenerateSpline(30);
	*/

	mvMat = new ZMatrix();

	fboPicker = new ZFbo(ZFboDesc((int)RES_X, (int)RES_Y, 0, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 1, false, GL_DEPTH_COMPONENT32, GL_FLOAT, false, GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST));
	fboTexture = new ZFbo(ZFboDesc((int)RES_X, (int)RES_Y, 0, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 1, false, GL_DEPTH_COMPONENT32, GL_FLOAT, false, GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST));

	parts = new particles();

	shaderTerrainPlain->SetActive(true);
	glRectf(-1, -1 / AR, 1, 1 / AR);
	shaderTerrainPlain->SetActive(false);

	shaderTerrainLogo->SetActive(true);
	glRectf(-1, -1 / AR, 1, 1 / AR);
	shaderTerrainLogo->SetActive(false);

	shaderTerrainSnowman->SetActive(true);
	glRectf(-1, -1 / AR, 1, 1 / AR);
	shaderTerrainSnowman->SetActive(false);

}

#ifdef DEBUG
void effect_showscene::reload()
{
	//shaderTerrainPlain->reload();
	shaderTerrain->reload();
	//shaderTerrainSnowman->reload();
	parts->shaderParticles->reload();
}
#endif

#ifdef DEBUG

//void terrain::drawPath(float time)
//{
//	//glMatrixMode(GL_MODELVIEW);	
//	//glLoadIdentity();
//	glColor4f(1,1,1,1);
//	/*
//	for(int i=1; i<path1->pointsAllocated-1; i++)
//	{
//		glBegin(GL_LINES);
//		glVertex3f(path1->vertices[i-1].x, path1->vertices[i-1].y, path1->vertices[i-1].z);
//		glVertex3f(path1->vertices[i].x, path1->vertices[i].y, path1->vertices[i].z);
//		glEnd();
//	}*/
//
//	GLint defaultFBO;
//	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBO);
//
//	glBindFramebufferEXT(GL_FRAMEBUFFER, fboPicker->id);
//	glClearColor(0, 0, 0, 0);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glPushMatrix();
//	path1->Draw(true, true, true, true);
//	/*
//	glColor4f(1,1,1,1);
//	glBegin(GL_QUADS);
//		glVertex3f(-1, -1, 0);
//		glVertex3f(-1,  1, 0);
//		glVertex3f( 1,  1, 0);
//		glVertex3f( 1, -1, 0);
//	glEnd();*/
//	glPopMatrix();
//	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
//	path1->Draw(true, true, true, false);
//	
//}
#endif


void effect_showscene::drawCube(float time)
{	
	//glDisable( GL_BLEND );
	//glEnable( GL_DEPTH_TEST );	
	//glEnable( GL_CULL_FACE );
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );
	glEnable( GL_COLOR_MATERIAL );
	//glEnable( GL_NORMALIZE );
	
	//glTranslatef(0, 0.6f, -3.0f-time);
	//glRotatef(-time*10.0f, 0.5f, 0.3f, 0.2f);

	glPushMatrix();				
		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_NORMAL_ARRAY );
		glVertexPointer( 3, GL_FLOAT, 6*sizeof(float), cubeVerts+0);
		glNormalPointer(    GL_FLOAT, 6*sizeof(float), cubeVerts+3);
	
		glPushMatrix();
			//glScalef(0.25f, 0.25f, 0.25f);
			glTranslatef(cube.x, cube.y, cube.z);
			//shaderPass->SetActive(true);
			glDrawElements( GL_QUADS, 6*4, GL_UNSIGNED_INT, cubeIndices );

			//glTranslatef(1.0, 0.0, 0.0);

			//glDrawElements( GL_QUADS, 6*4, GL_UNSIGNED_INT, cubeIndices );
			//shaderPass->SetActive(false);
		glPopMatrix();
	
		glDisableClientState( GL_NORMAL_ARRAY );
		glDisableClientState( GL_VERTEX_ARRAY );
	glPopMatrix();
	glDisable( GL_LIGHTING );
	glDisable( GL_LIGHT0 );
	
}

/// get perspective camera parameters
/// from the OpenGL projection matrix
///
/// \param[out] fovy_rad     field of view in radian
/// \param[out] aspect_ratio aspect ratio
/// \param[out] clip_min     clipping min distance
/// \param[out] clip_max     clipping max distance
void gl_get_camera_parameter_from_perspective_matrix(
	double & fovy_rad,
	double & aspect_ratio,
	double & clip_min,
	double & clip_max)
{
	GLdouble mat[16];
	glGetDoublev(GL_PROJECTION_MATRIX, mat);

	GLdouble const aa = mat[0];
	GLdouble const bb = mat[5];
	GLdouble const cc = mat[10];
	GLdouble const dd = mat[14];

	aspect_ratio = bb / aa;
	fovy_rad     = 2.0f * atan(1.0f / bb);

	GLdouble const kk = (cc - 1.0f) / (cc + 1.0f);
	clip_min = (dd * (1.0f - kk)) / (2.0f * kk);
	clip_max = kk * clip_min;
}

void effect_showscene::drawTerrain(float itime)
{	
#ifdef DEBUG
	//shaderTerrainPlain->refresh();
	//shaderTerrainLogo->refresh();
	shaderTerrain->refresh();
#endif
	shaderTerrain->SetActive(true);

	//double fovy_rad;
	//double aspect_ratio;
//	double clip_min;
	//double clip_max;

	//gl_get_camera_parameter_from_perspective_matrix(fovy_rad, aspect_ratio, clip_min, clip_max);

	//double focalLength = 1.0f / tan(fovy_rad / 2);


	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, perlinNoise->fboNoise->colorBufferID[0]);
	glBindTexture(GL_TEXTURE_2D, textureNoise);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, fboTexture->colorBufferID[0]);
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, FBO_CULLING->colorBufferID[0]);


	/*
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_3D, textureNoise3d);
	*/

	float vec3[3];
	vec3[0] = (float)RES_X;
	vec3[1] = (float)RES_Y;
	vec3[2] = 0;

	GLint iLoc;

	vec3[0] = (float)sync_get_val(skyCol1_R, rocketRow);
	vec3[1] = (float)sync_get_val(skyCol1_G, rocketRow);
	vec3[2] = (float)sync_get_val(skyCol1_B, rocketRow);
	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "sky1");
	glUniform3fv(iLoc, 1, vec3);

	vec3[0] = (float)sync_get_val(skyCol2_R, rocketRow);
	vec3[1] = (float)sync_get_val(skyCol2_G, rocketRow);
	vec3[2] = (float)sync_get_val(skyCol2_B, rocketRow);
	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "sky2");
	glUniform3fv(iLoc, 1, vec3);

	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "lig");	
	glUniform3fv(iLoc, 1, sunPos);
	
	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "cube");
	glUniform3fv(iLoc, 1, cube.v);

	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "cube");
	glUniform3fv(iLoc, 1, cube.v);

	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "objectPos");
	glUniform3fv(iLoc, 1, POS_OBJ.v);

	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "objectMix");
	glUniform1f(iLoc, SYNC_OBJ_MIX_1);

	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "time");
	glUniform1f(iLoc, itime);
			
	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "iChannel0");
	glUniform1i(iLoc, 0);

	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "iChannel1");
	glUniform1i(iLoc, 1);

	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "mask");
	glUniform1i(iLoc, 2);

#define MATRIX_PROJECTION global:GetState()->matrixProjection
	//ZMatrix
	float v2[2];
	v2[0] = (float)RES_X;
	v2[1] = (float)RES_Y;
	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "g_resolution");
	glUniform2fv(iLoc, 1, v2);
		
	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "znear");
	glUniform1f(iLoc, fzn);

	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "zfar");
	glUniform1f(iLoc, fzf);

	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "pm");
	glUniformMatrix4fv(iLoc, 1, false, MAT_PROJ.m[0]);

	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "ipm");
	glUniformMatrix4fv(iLoc, 1, false, MAT_PROJ_INV.m[0]);

	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "mvm");
	glUniformMatrix4fv(iLoc, 1, false, MAT_MV.m[0]);	

	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "imvm");
	glUniformMatrix4fv(iLoc, 1, false, MAT_MV_INV.m[0]);
				
	ZVector camPos = POS_EYE.Transform(MAT_MV_INV);
	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "cameraPosition");
	glUniform3fv(iLoc, 1, &camPos.x);

	ZVector lookd = POS_LOOK.Minus(POS_EYE).Normalize();
	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "cameraLookDirection");
	glUniform3fv(iLoc, 1, &lookd.x);
	
	//glDisable(GL_BLEND);
	//glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	glRectf(-1, -1 / AR, 1, 1 / AR);
	glDepthFunc(GL_LEQUAL);
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	
	//drawCube(itime);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_3D, 0);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
		
	shaderTerrain->SetActive(false);
}

void drawFluff()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glOrtho( 0, (int)RES_X , (int)RES_Y , 0, 0, 1 );             // Select Ortho Mode (ie.640x480)
	glBegin(GL_LINES);
		glVertex2i(0, BAR_SIZE-2);
		glVertex2i((int)RES_X, BAR_SIZE-2);

		glVertex2i(0, (int)RES_Y-BAR_SIZE+1);
		glVertex2i((int)RES_X, ((int)RES_Y)-BAR_SIZE+1);
	glEnd();

	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

static int nsides = 32;

static void output_vertex(int lat, int lon) {
	float v[3];
	double la, lo;

	la = 2.*M_PI*lat/nsides;
	lo = 2.*M_PI*lon/nsides;
	/* this is unoptimized */
	v[0] = (float)(cos(lo)*sin(la));
	v[1] = (float)(sin(lo)*sin(la));
	v[2] = (float)(cos(la));
	glNormal3fv(v);
	glVertex3fv(v);
}

void draw_tri_strip_sphere()
{
	int lat, lon;

	/* this is probably doing almost exactly the same thing as gluSphere */
	for (lat=0; lat<nsides/2; lat++) {
		glBegin(GL_TRIANGLE_STRIP);
		for (lon=0; lon<=nsides; lon++) {
			/* glColor3f(FRAND(), FRAND(), FRAND()); */
			output_vertex(lat, lon);
			output_vertex(lat+1, lon);
		}
		glEnd();
	}
}

void effect_showscene::drawCullSphere(float size, ZVector pos)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();	
	{
		glTranslatef(pos.x, pos.y, pos.z);
		glScalef(size, size, size);
		draw_tri_strip_sphere();
	}	
	glPopMatrix();
}

void effect_showscene::drawCullCube(ZVector size, ZVector pos)
{
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 6*sizeof(float), cubeVerts+0);
	glNormalPointer(    GL_FLOAT, 6*sizeof(float), cubeVerts+3);

	glPushMatrix();	
	{
		glTranslatef(pos.x, pos.y, pos.z);
		glScalef(size.x, size.y, size.z);
		glDrawElements( GL_QUADS, 6*4, GL_UNSIGNED_INT, cubeIndices );
	}	
	glPopMatrix();

	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
}

void effect_showscene::drawCullObjects(float time)
{
	glColor4f(1.0, 1.0, 1.0, 0.2f);
	glDisable(GL_DEPTH_TEST);

	if (shaderTerrain == shaderTerrainLogo)
	{
		ZVector size = ZVector(35.0f, 5.0f, 15.0f);
		ZVector pos = ZVector(-POS_OBJ.x + 22.0f, -POS_OBJ.y + 35.0f, -POS_OBJ.z);
		drawCullCube(size, pos);

		size = ZVector(5.0f, 20.0f, 5.0f);

		// d mast
		pos = ZVector(-POS_OBJ.x+10.0f, -POS_OBJ.y+47.0f, -POS_OBJ.z);
		drawCullCube(size, pos);

		// b mast
		pos = ZVector(-POS_OBJ.x+18.0f, -POS_OBJ.y+47.0f, -POS_OBJ.z);
		drawCullCube(size, pos);

		// f mast
		pos = ZVector(-POS_OBJ.x+48.0f, -POS_OBJ.y+47.0f, -POS_OBJ.z);
		drawCullCube(size, pos);

		// d sphere
		pos = ZVector(-POS_OBJ.x+0.0f, -POS_OBJ.y+40.0f, -POS_OBJ.z);
		drawCullSphere(15.0, pos);

		// b sphere
		pos = ZVector(-POS_OBJ.x+28.0f, -POS_OBJ.y+40.0f, -POS_OBJ.z);
		drawCullSphere(15.0, pos);

		// f sphere
		pos = ZVector(-POS_OBJ.x+52.0f, -POS_OBJ.y+50.0f, -POS_OBJ.z);
		drawCullSphere(15.0, pos);

	}else if (shaderTerrain == shaderTerrainSnowman)
	{
		ZVector pos = ZVector(-POS_OBJ.x+20.0f, -POS_OBJ.y+68.0f, -POS_OBJ.z+20);

		// head
		//drawCullCube(size, pos);
		drawCullSphere(14.0f, pos);

		// body
		pos = ZVector(-POS_OBJ.x+20.0f, -POS_OBJ.y+40.0f, -POS_OBJ.z+20);
		drawCullSphere(22.0f, pos);
	}

	glEnable(GL_DEPTH_TEST);
}


void effect_showscene::draw(float itime)
{
	glEnable( GL_DEPTH_TEST );	
	glEnable( GL_NORMALIZE );	

	// reset the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// refresh text shader

#ifdef DEBUG
	txt3d->shaderText->refresh();
#endif
		
	POS_EYE.x = (float)sync_get_val(syncEyeX, rocketRow);
	POS_EYE.y = (float)sync_get_val(syncEyeY, rocketRow);
	POS_EYE.z = (float)sync_get_val(syncEyeZ, rocketRow);
	
	cube.x = (float)sync_get_val(cubeX, rocketRow);
	cube.y = (float)sync_get_val(cubeY, rocketRow);
	cube.z = (float)sync_get_val(cubeZ, rocketRow);

	POS_OBJ.x = (float)sync_get_val(objectX, rocketRow);
	POS_OBJ.y = (float)sync_get_val(objectY, rocketRow);
	POS_OBJ.z = (float)sync_get_val(objectZ, rocketRow);

	SYNC_OBJ_MIX_1 = (float)sync_get_val(objectMix, rocketRow);

	SYNC_CAM_RADIUS = (float)sync_get_val(camRadius, rocketRow);

	//sunPos
	sunPos[0] = (float)sync_get_val(sunPosX, rocketRow);
	sunPos[1] = (float)sync_get_val(sunPosY, rocketRow);
	sunPos[2] = (float)sync_get_val(sunPosZ, rocketRow);
	
	int objNum = (int)sync_get_val(objectNum, rocketRow);

	switch (objNum)
	{
		case 2:
			shaderTerrain = shaderTerrainSnowman;
			break;
		case 1:
			shaderTerrain = shaderTerrainLogo;
			break;
		default:
			shaderTerrain = shaderTerrainPlain;
	}
	
	float roll = (float)sync_get_val(rotRoll, rocketRow);
	float pitch = (float)sync_get_val(rotPitch, rocketRow);
	float heading = (float)sync_get_val(rotHeading, rocketRow);

	glTranslatef(0, 0, SYNC_CAM_RADIUS);
	glRotatef(pitch, 1.0f, 0.0f, 0.0f);
	glRotatef(heading, 0.0f, 1.0f, 0.0f);
	glRotatef(roll, 0.0f, 0.0f, 1.0f);
	glTranslatef(POS_EYE.x, POS_EYE.y, POS_EYE.z);

	float matrix[16];

	glGetFloatv(GL_PROJECTION_MATRIX, matrix);
	MAT_PROJ = ZMatrix::CreateIdentity();	
	memcpy(MAT_PROJ.m, matrix, sizeof(float)*16);

	MAT_PROJ_INV = ZMatrix::CreateIdentity();
	MAT_PROJ_INV = MAT_PROJ_INV.Inverse();

	int iLoc;

	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
//	int iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "modelviewMatrix");
//	glUniformMatrix4fv(iLoc, 1, false, matrix);	

	MAT_MV = ZMatrix::CreateIdentity();
	MAT_MV_INV = ZMatrix::CreateIdentity();
	memcpy(MAT_MV.m, matrix, sizeof(float)* 16);
	MAT_MV_INV = MAT_MV.Inverse();

	// normal matrix
	ZMatrix n = MAT_MV_INV.Transpose();
	for (int i=0; i<3; i++)
	{
		for (int j=0; j<3; j++)
		{
			MAT_NORM.m[i*3][j] = n.m[i][j];
		}
	}		

	GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT };
	
	glClearColor(0, 0, 0, 0);
	
	// create cull texture
	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboTexture->id);
	//glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
	//makeTextures(0);
	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	
	// create cull texture
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FBO_CULLING->id);
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
	glDrawBuffers(1, &buffers[0]);
	drawCullObjects(itime);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	
	// terrain 
	// bind FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FBO->id);
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );

	// terrain
	drawTerrain(itime);

	// snow particles
	parts->draw(itime, &MAT_MV, &MAT_PROJ, POS_EYE, ZVector(pitch, heading, roll));

	// debug
	//drawCullObjects(itime);
	
	txt3d->shaderText->SetActive(true);
		
	iLoc = glGetUniformLocation(txt3d->shaderText->shaderProg, "pm");
	glUniformMatrix4fv(iLoc, 1, false, MAT_PROJ.m[0]);
	
	iLoc = glGetUniformLocation(txt3d->shaderText->shaderProg, "mvm");
	glUniformMatrix4fv(iLoc, 1, false, MAT_MV.m[0]);	

	iLoc = glGetUniformLocation(txt3d->shaderText->shaderProg, "nm");
	glUniformMatrix3fv(iLoc, 1, false, MAT_NORM.m[0]);

	iLoc = glGetUniformLocation(txt3d->shaderText->shaderProg, "lig");	
	glUniform3fv(iLoc, 1, ZVector(-1.0, -1.0, -1.0).v);

	iLoc = glGetUniformLocation(txt3d->shaderText->shaderProg, "txtAmount");	
	glUniform1f(iLoc, (float)sync_get_val(textAmount, rocketRow));

	iLoc = glGetUniformLocation(txt3d->shaderText->shaderProg, "time");	
	glUniform1f(iLoc, itime);

	int txtNum = (int)sync_get_val(textNum, rocketRow);
	if (txtNum > -1)
	{
		txt3d->DrawTextObject(txt3d->textObjects[txtNum]);
	}
	
	txt3d->shaderText->SetActive(false);


	// edging lines etc
	drawFluff();
	
	// unbind FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

effect_showscene::~effect_showscene(void)
{
}
