#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <math.h>
#include "terrain.h"
#include "framework/gl/gl8k.h"
#include "framework/gl/shader.h"
#include <GL/glu.h>

#include "../quatCam.h"

#include "clinkster.h"
//#include "shaders/shader_code.h"
#include "config.h"
#include "../rocket/rocketControl.h"
#include "music/syncData.h"
#include "framework/gl/fbo.h"
#include "framework\math\matrix.h"

#ifndef SYNC_PLAYER	
	#include "../rocket/sync/device.h"
	#include <stdio.h>
#endif

#include "framework/utils8k.h"

#include "../../testStuff/stb_image.h"
#include "../../testStuff/noise26_png.h"


#include "../quatCam.h"

#define fzn  0.05f
#define fzf  200.0f

#define DEG_PER_RAD 0.0174532925f

//ZVector eye, lookAt, cubePos;

ZMatrix matrixProjection;
ZMatrix matrixProjectionInverse;
ZMatrix matrixModelView;
ZMatrix matrixModelViewInverse;

extern ZVector eye, look, cube;

extern ZFbo* fbo;

ZMatrix *mvMat;

quatCam* cam;

ZFbo* fboPicker = 0;

static float cubeVerts[] = { 

	1.0f, -1.0f, -1.0f,    0.0f,-1.0f, 0.0f,
	1.0f, -1.0f,  1.0f,    0.0f,-1.0f, 0.0f,
	-1.0f, -1.0f,  1.0f,    0.0f,-1.0f, 0.0f,
	-1.0f, -1.0f, -1.0f,    0.0f,-1.0f, 0.0f,

	-1.0f,  1.0f, -1.0f,    0.0f, 1.0f, 0.0f,
	-1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 0.0f,
	1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 0.0f,
	1.0f,  1.0f, -1.0f,    0.0f, 1.0f, 0.0f,

	-1.0f, -1.0f, -1.0f,   -1.0f, 0.0f, 0.0f,
	-1.0f, -1.0f,  1.0f,   -1.0f, 0.0f, 0.0f,
	-1.0f,  1.0f,  1.0f,   -1.0f, 0.0f, 0.0f,
	-1.0f,  1.0f, -1.0f,   -1.0f, 0.0f, 0.0f,

	1.0f,  1.0f, -1.0f,    1.0f, 0.0f, 0.0f,
	1.0f,  1.0f,  1.0f,    1.0f, 0.0f, 0.0f,
	1.0f, -1.0f,  1.0f,    1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, -1.0f,    1.0f, 0.0f, 0.0f,

	-1.0f, -1.0f, -1.0f,    0.0f, 0.0f,-1.0f,
	-1.0f,  1.0f, -1.0f,    0.0f, 0.0f,-1.0f,
	1.0f,  1.0f, -1.0f,    0.0f, 0.0f,-1.0f,
	1.0f, -1.0f, -1.0f,    0.0f, 0.0f,-1.0f,

	1.0f, -1.0f,  1.0f,    0.0f, 0.0f, 1.0f,
	1.0f,  1.0f,  1.0f,    0.0f, 0.0f, 1.0f,
	-1.0f,  1.0f,  1.0f,    0.0f, 0.0f, 1.0f,
	-1.0f, -1.0f,  1.0f,    0.0f, 0.0f, 1.0f };

static const int cubeIndices[] = { 
	0, 1, 2, 3,
	4, 5, 6, 7,
	8, 9,10,11,
	12,13,14,15,
	16,17,18,19,
	20,21,22,23 };

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

void terrain::makeNoise3d()
{
	ZRandom* random = new ZRandom(0);
	int seed = 0;

	shader* noiseShader = new shader("shaders\\include\\blit.vert",
		NULL,
		"shaders\\include\\Noise3DLinear.frag"
		);

	
	for (int i = 0; i < 1; i++)
	{
		// Large texture
		glGenTextures(1, &textureNoise3dFix);
		glBindTexture(GL_TEXTURE_3D, textureNoise3dFix);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB8, 256, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

		/*
		// Small texture
		unsigned char *texels = new unsigned char[16 * 16 * 16];
		for (int i = 0; i < 16 * 16 * 16; i++) texels[i] = random->randomInt() & 255;
		GLuint smallTex;
		glGenTextures(1, &smallTex);
		glBindTexture(GL_TEXTURE_3D, smallTex);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB8, 16, 16, 16, 0, GL_RED, GL_UNSIGNED_BYTE, texels);
		delete [] texels;
		*/
		
		// Layer fbo
		GLuint layerFbo;
		glGenFramebuffersEXT(1, &layerFbo);
		glBindFramebufferEXT(GL_FRAMEBUFFER, layerFbo);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureNoise);

		noiseShader->SetActive(true);
		
		// Generate large noise texture
		for (int i = 0; i < 256; i++)
		{
			glFramebufferTextureLayerEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureNoise3dFix, 0, i);
			glUniform1i(glGetUniformLocation(noiseShader->shaderProg, "t"), 0);
			glUniform1i(glGetUniformLocation(noiseShader->shaderProg, "textureLayer"), i);
			glRecti(-1, -1, 1, 1);
		}
				
		// Cleanup
		noiseShader->SetActive(false);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_3D, 0);
		glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
	}
}

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

float fmix(float x, float y, float a)
{
	 return x * ( 1.0f - a ) + y * a;
}

terrain::terrain(void)
{
	for(int i=0; i<8*6*4; i++)
	{
		cubeVerts[i] *= 0.25f;
	}

	cam = new quatCam();
	
#ifdef DEBUG
	shaderTerrain = new shader(
		"shaders\\include\\terrain.vert",
		NULL,
		"shaders\\include\\terrain.frag"
		);

	shaderTest = new shader(
		"shaders\\include\\colorTest.vert",
		NULL,
		"shaders\\include\\colorTest.frag"
		);

	shaderPass = new shader(
		"shaders\\include\\passthru.vert",
		NULL,
		"shaders\\include\\passthru.frag"
		);
#else
	/*
	shaderTerrain = new shader(
		&spinner_vs_glsl,
		&spinner_gs_glsl,
		&spinner_fs_glsl,
		);*/
#endif

	int width, height, icomps;
	GLenum eFormat;

	GLbyte *pixels = gltLoadPNG(tex16_png, tex16_png_size, &width, &height, &icomps, &eFormat);	
	ZRandom* zr = new ZRandom(0);
			
	int adr;
	int adr2;
	
	pixel_rgb* pix = new pixel_rgb[width*height];
	
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

	float* zpix = new float[256*256*256];

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

	// create 3d blend texture
	for(int z=0; z<256; z++)
	{
		for (int y=0;y<256;y++)
		{
			for (int x=0;x<256;x++)
			{
				adr=x+256*y;
				int adrz = adr + (256*256*z);

				float mix = (z+1) / 256.0f;
				//mix += 0.01f;
				
				/*
				int px = ifloorf(x);
				int py = ifloorf(y);
				int pz = ifloorf(z);

				float fx;
				modf(x, &fx);

				float fy;
				modf(y, &fy);

				float fz;
				modf(z, &fz);

				fx = fx * fx *(3.0f-2.0f*fx);
				fy = fy * fy *(3.0f-2.0f*fy);
				fz = fz * fz *(3.0f-2.0f*fz);

				float uvX = (x + 37.0f * z) + fx;
				float uvY = (y + 17.0f * z) + fy;

				float val1 = texture3D(iChannel3d, vec3((uv+ 0.5)/256.0, f.z), -100.0).x;
				*/

				pixel_rgb p = pix[adr];

				//zpix[adrz] = fmix(pix[adr].r, pix[adr].g, mix);

				zpix[adrz] = (pix[adr].r * mix) + (pix[adr].g * (1.0f - mix));
			}
		}
	}
	
	glGenTextures(1, &textureNoise);
	glBindTexture(GL_TEXTURE_2D, textureNoise);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, pix );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering

	pixels = gltLoadPNG(tex01_png, tex01_png_size, &width, &height, &icomps, &eFormat);
	glGenTextures(1, &textureColor);
	glBindTexture(GL_TEXTURE_2D, textureColor);
	glTexImage2D(GL_TEXTURE_2D, 0, icomps, width, height, 0, eFormat, GL_UNSIGNED_BYTE, pixels );
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

	path1 = new ZSplinePath();

	path1->AddPoint(new ZVector(-1.0f, -1.9f, -0.0f));

	path1->AddPoint(new ZVector(-0.5f, -0.0f, -0.5f));

	path1->AddPoint(new ZVector(-0.02f, -2.2f, -1.0f));

	path1->AddPoint(new ZVector(-0.30f, -0.5f, -1.5f));

	path1->GenerateSpline(30);

	int xx = 1;

	mvMat = new ZMatrix();

	fboPicker = new ZFbo(ZFboDesc(RES_X, RES_Y, 0, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 1, false, GL_DEPTH_COMPONENT32, GL_FLOAT, false, GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST));
}

void terrain::reload()
{
	shaderTerrain->reload();
}

void terrain::drawPath(float time)
{
	//glMatrixMode(GL_MODELVIEW);	
	//glLoadIdentity();
	glColor4f(1,1,1,1);
	/*
	for(int i=1; i<path1->pointsAllocated-1; i++)
	{
		glBegin(GL_LINES);
		glVertex3f(path1->vertices[i-1].x, path1->vertices[i-1].y, path1->vertices[i-1].z);
		glVertex3f(path1->vertices[i].x, path1->vertices[i].y, path1->vertices[i].z);
		glEnd();
	}*/

	GLint defaultFBO;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBO);

	glBindFramebufferEXT(GL_FRAMEBUFFER, fboPicker->id);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	path1->Draw(true, true, true, true);
	/*
	glColor4f(1,1,1,1);
	glBegin(GL_QUADS);
		glVertex3f(-1, -1, 0);
		glVertex3f(-1,  1, 0);
		glVertex3f( 1,  1, 0);
		glVertex3f( 1, -1, 0);
	glEnd();*/
	glPopMatrix();
	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
	path1->Draw(true, true, true, false);
	
}

void terrain::drawCube(float time)
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

			glTranslatef(1.0, 1.0, 0.0);

			glDrawElements( GL_QUADS, 6*4, GL_UNSIGNED_INT, cubeIndices );
			//shaderPass->SetActive(false);
		glPopMatrix();
	
		glDisableClientState( GL_NORMAL_ARRAY );
		glDisableClientState( GL_VERTEX_ARRAY );
	glPopMatrix();
	/**
	for(int x = 0; x<11; x++)
	{
		for(int y= 0; y<11; y++)
		{
			for(int z= 0; z<11; z++)
			{
				if (z == 5)
				{
					glColor4f(1,0,1,1);
				}else{
					glColor4f(1,1,1,1);
				}
				glPushMatrix();				
				glTranslatef((1.2f*x)-(5*1.2f), (1.2f * y)-(5*1.2f), (1.2f * z)-(5*1.2f));
				glEnableClientState( GL_VERTEX_ARRAY );
				glEnableClientState( GL_NORMAL_ARRAY );
				glVertexPointer( 3, GL_FLOAT, 6*sizeof(float), cubeVerts+0);
				glNormalPointer(    GL_FLOAT, 6*sizeof(float), cubeVerts+3);

				glPushMatrix();
				glScalef(0.1f, 0.1f, 0.1f);
				glDrawElements( GL_QUADS, 6*4, GL_UNSIGNED_INT, cubeIndices );
				glPopMatrix();
				
				glDisableClientState( GL_NORMAL_ARRAY );
				glDisableClientState( GL_VERTEX_ARRAY );
				glPopMatrix();
			}
		}
	}
	*/
	//glEnable( GL_BLEND );
	//glDisable( GL_DEPTH_TEST );	
	//glDisable( GL_CULL_FACE );
	glDisable( GL_LIGHTING );
	glDisable( GL_LIGHT0 );
	//glDisable( GL_NORMALIZE );
	
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

void terrain::drawTerrain(float itime)
{	
	shaderTerrain->refresh();
	shaderTerrain->SetActive(true);

	double fovy_rad;
	double aspect_ratio;
	double clip_min;
	double clip_max;

	gl_get_camera_parameter_from_perspective_matrix(fovy_rad, aspect_ratio, clip_min, clip_max);

	double focalLength = 1.0f / tan(fovy_rad / 2);


	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, perlinNoise->fboNoise->colorBufferID[0]);
	glBindTexture(GL_TEXTURE_2D, textureNoise);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureColor);
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureColor);

	/*
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_3D, textureNoise3d);
	*/

	float vec3[3];
	vec3[0] = XRES;
	vec3[1] = YRES;
	vec3[2] = 0;

	GLint iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "iResolution");
	glUniform3fv(iLoc, 1, vec3);

	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "iChannelResolution");
	glUniform3fv(iLoc, 4, vec3);

	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "iGlobalTime");
	glUniform1f(iLoc, itime);

	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "iGlobalTime");
	glUniform1f(iLoc, itime);
		
	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "iChannel0");
	glUniform1i(iLoc, 0);

	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "iChannel1");
	glUniform1i(iLoc, 1);

	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "iChannel2");
	glUniform1i(iLoc, 2);

	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "iChannel3d");
	glUniform1i(iLoc, 3);

	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "znear");
	glUniform1f(iLoc, fzn);

	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "zfar");
	glUniform1f(iLoc, fzf);

	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "pm");
	glUniformMatrix4fv(iLoc, 1, false, matrixProjection.m[0]);

	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "ipm");
	glUniformMatrix4fv(iLoc, 1, false, matrixProjectionInverse.m[0]);	

	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "mvm");
	glUniformMatrix4fv(iLoc, 1, false, matrixModelView.m[0]);	

	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "imvm");
	glUniformMatrix4fv(iLoc, 1, false, matrixModelViewInverse.m[0]);
				
	ZVector camPos = eye.Transform(matrixModelViewInverse);
	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "cameraPosition");
	glUniform3fv(iLoc, 1, &camPos.x);

	ZVector lookd = look.Minus(eye).Normalize();
	iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "cameraLookDirection");
	glUniform3fv(iLoc, 1, &lookd.x);
	
	glDisable(GL_BLEND);
	//glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);
	glRectf(-1, -1, 1, 1);
	glDepthFunc(GL_LESS);
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	
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

void terrain::draw(float itime)
{
	glEnable( GL_DEPTH_TEST );	

	// reset the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	const double rocketRow = rocket_get_row();
		
	eye.x = (float)sync_get_val(syncEyeX, rocketRow);
	eye.y = (float)sync_get_val(syncEyeY, rocketRow);
	eye.z = (float)sync_get_val(syncEyeZ, rocketRow);
	
	cube.x = (float)sync_get_val(cubeX, rocketRow);
	cube.y = (float)sync_get_val(cubeY, rocketRow);
	cube.z = (float)sync_get_val(cubeZ, rocketRow);
	
	look = eye;
	
	float roll = (float)sync_get_val(rotRoll, rocketRow);
	float pitch = (float)sync_get_val(rotPitch, rocketRow);
	float heading = (float)sync_get_val(rotHeading, rocketRow);
	ZMatrix rot = ZMatrix::CreateIdentity().RotateY(DEG_PER_RAD * roll).RotateX(DEG_PER_RAD * pitch);

	ZVector lookDir(0,0,-1);
	look = eye.Plus(lookDir.Transform(rot).Normalize());
	
	// Look at
	// .. compute real up from pseudo up
	//ZVector center(0.f, 0.f, 0.f);
	ZVector forward = look.Minus(eye).Normalize();
	ZVector pseudoup(0.f, 1.f, 0.f);
	

	//pseudoup = pseudoup.Transform(rot).Normalize();

	ZVector right = forward.Cross(pseudoup);
	ZVector up = right.Cross(forward);


	cam->eyePos.x = eye.x;
	cam->eyePos.y = eye.y;
	cam->eyePos.z = eye.z;

	cam->lookPos = cam->eyePos;
	cam->lookPos.z -=1.0;

	cam->Look();
	
	/**
	glRotatef((float)sync_get_val(rotTheta, rocketRow), 1.0, 0.0, 0.0);
	glRotatef((float)sync_get_val(rotPhi, rocketRow), 0.0, 1.0, 0.0);
	glTranslatef(eye.x, eye.y, eye.z);
	*/

	//glLoadMatrixf(matrixInverseModelView.m);
	
	//gluLookAt(eye.x, eye.y, eye.z, look.x, look.y, look.z, up.x, up.y, up.z);

	//glRotatef(pitch, 1.0f,0.0f,0.0f); // pitch
	//glRotatef(heading, 0.0f,1.0f,0.0f); // heading
	//glRotatef(roll, 0.0f,0.0f,1.0f); // roll
	//glRotatef(yaw,0.0f,1.0f,0.0f);
	//glRotatef(pitch,1.0f,0.0f,0.0f);
	//glTranslatef(-eye.x,-eye.y,-eye.z);

	float matrix[16];

	glGetFloatv(GL_PROJECTION_MATRIX, matrix);
	matrixProjection = ZMatrix::CreateIdentity();	
	memcpy(matrixProjection.m, matrix, sizeof(float)*16);

	matrixProjectionInverse = ZMatrix::CreateIdentity();
	matrixProjectionInverse = matrixProjection.Inverse();

	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	int iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "modelviewMatrix");
	glUniformMatrix4fv(iLoc, 1, false, matrix);	

	matrixModelView = ZMatrix::CreateIdentity();
	matrixModelViewInverse = ZMatrix::CreateIdentity();
	memcpy(matrixModelView.m, matrix, sizeof(float)*16);
	matrixModelViewInverse = matrixModelView.Inverse();

		
	GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT };
	
	// FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo->id);

	glDrawBuffers(3, &buffers[0]);
	
	glClearColor(0, 0, 0, 0);
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
	
	// terrain
	//drawTerrain(itime);

	//glLoadIdentity();

	//drawPath(itime);
	
	// cube
	drawCube(itime);

	// unbind FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	glDrawBuffers(1, &buffers[0]);

}

terrain::~terrain(void)
{
}
