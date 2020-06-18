#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <math.h>
#include "effect_glenz.h"
#include "framework/gl/gl8k.h"
#include "framework/gl/shader.h"
#include <GL/glu.h>

#include "clinkster/player/clinkster.h"
#include "../shader_code_intro.h"
#include "rocket/rocketControl.h"
#include "../music/syncData.h"
#include "framework/gl/fbo.h"
#include "framework\math\matrix.h"

#ifndef SYNC_PLAYER	
#include "../rocket/sync/device.h"
#include <stdio.h>
#endif

#include "framework/utils8k.h"
#include "effect_glenz.h"
#include "global.h"


#define fzn  1.0f
#define fzf  1000.0f

#define DEG_PER_RAD 0.0174532925f

//ZVector eye, lookAt, cubePos;

ZMatrix matrixProjection = ZMatrix();
ZMatrix matrixProjectionInverse = ZMatrix();
ZMatrix matrixModelView = ZMatrix();
ZMatrix matrixModelViewInverse = ZMatrix();
float normalMatrix[9];

extern float objectMixVal;
extern float camRadiusVal;

extern ZFbo *fbo, *fboCulling;

extern double rocketRow;

float sunPos[3];

ZMatrix *mvMat;

//quatCam* cam;

ZFbo* fboPicker = 0;
ZFbo* fboTexture = 0;

static float cubeVerts[] = {

	1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
	-1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
	-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f,

	-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,

	-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
	-1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
	-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
	-1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f,

	1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,

	-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
	-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
	1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
	1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f,

	1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f };

static const int cubeIndices[] = {
	0, 1, 2, 3,
	4, 5, 6, 7,
	8, 9, 10, 11,
	12, 13, 14, 15,
	16, 17, 18, 19,
	20, 21, 22, 23 };

struct pixel_rgba
{
	float r, g, b, a;
};

struct pixel_rgb
{
	float r, g, b;
};

static short opc1 = 0x043f;     // floor

int ifloorf(const float x)
{
	int res;
	short tmp;

	_asm fstcw   word  ptr[tmp]
		_asm fld     dword ptr[x]
		_asm fldcw   word  ptr[opc1]
		_asm fistp   dword ptr[res]
		_asm fldcw   word  ptr[tmp]

		return res;
}

float fmix(float x, float y, float a)
{
	return x * (1.0f - a) + y * a;
}

effect_glenz::effect_glenz(void)
{

#ifdef DEBUG

	shaderWater = new shader(
		"shaders\\include\\water.vert",
		NULL,
		"shaders\\include\\water.frag",
		""
		);


#else

	shaderWater = new shader(
		(char*)terrainTest_vert,
		NULL,
		(char*)terrainTest_frag,
		""
		);

#endif

	mvMat = new ZMatrix();

	fboPicker = new ZFbo(ZFboDesc(RES_X, RES_Y, 0, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 1, false, GL_DEPTH_COMPONENT32, GL_FLOAT, false, GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST));
	fboTexture = new ZFbo(ZFboDesc(RES_X, RES_Y, 0, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 1, false, GL_DEPTH_COMPONENT32, GL_FLOAT, false, GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST));
	
	shaderWater->SetActive(true);
	glRectf(-1, -1 / AR, 1, 1 / AR);
	shaderWater->SetActive(false);

}

#ifdef DEBUG
void effect_glenz::reload()
{
	shaderWater->reload();
}
#endif

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
	fovy_rad = 2.0f * atan(1.0f / bb);

	GLdouble const kk = (cc - 1.0f) / (cc + 1.0f);
	clip_min = (dd * (1.0f - kk)) / (2.0f * kk);
	clip_max = kk * clip_min;
}

void effect_glenz::getSyncVars()
{
	POS_EYE.x = (float)sync_get_val(syncEyeX, rocketRow);
	POS_EYE.y = (float)sync_get_val(syncEyeY, rocketRow);
	POS_EYE.z = (float)sync_get_val(syncEyeZ, rocketRow);

	POS_CUBE.x = (float)sync_get_val(cubeX, rocketRow);
	POS_CUBE.y = (float)sync_get_val(cubeY, rocketRow);
	POS_CUBE.z = (float)sync_get_val(cubeZ, rocketRow);

	POS_OBJ.x = (float)sync_get_val(objectX, rocketRow);
	POS_OBJ.y = (float)sync_get_val(objectY, rocketRow);
	POS_OBJ.z = (float)sync_get_val(objectZ, rocketRow);

	objectMixVal = (float)sync_get_val(objectMix, rocketRow);

	camRadiusVal = (float)sync_get_val(camRadius, rocketRow);

	//sunPos
	sunPos[0] = (float)sync_get_val(sunPosX, rocketRow);
	sunPos[1] = (float)sync_get_val(sunPosY, rocketRow);
	sunPos[2] = (float)sync_get_val(sunPosZ, rocketRow);

	float roll = (float)sync_get_val(rotRoll, rocketRow);
	float pitch = (float)sync_get_val(rotPitch, rocketRow);
	float heading = (float)sync_get_val(rotHeading, rocketRow);

	glTranslatef(0, 0, camRadiusVal);
	glRotatef(pitch, 1.0f, 0.0f, 0.0f);
	glRotatef(heading, 0.0f, 1.0f, 0.0f);
	glRotatef(roll, 0.0f, 0.0f, 1.0f);
	glTranslatef(POS_EYE.x, POS_EYE.y, POS_EYE.z);


}

void effect_glenz::draw(float itime)
{
	getSyncVars();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	// reset the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float matrix[16];

	glGetFloatv(GL_PROJECTION_MATRIX, matrix);
	MAT_PROJ = ZMatrix::CreateIdentity();
	memcpy(MAT_PROJ.m, matrix, sizeof(float)* 16);

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
	for (int i = 0; i<3; i++)
	{
		for (int j = 0; j<3; j++)
		{
			MAT_NORM.m[i * 3][j] = n.m[i][j];
		}
	}

	GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT };

	glClearColor(0, 0, 0, 0);

	// terrain 
	// bind FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo->id);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// terrain
#ifdef DEBUG
	//shaderTerrainPlain->refresh();
	//shaderTerrainLogo->refresh();
	shaderWater->refresh();
#endif
	shaderWater->SetActive(true);

	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, perlinNoise->fboNoise->colorBufferID[0]);
	glBindTexture(GL_TEXTURE_2D, textureNoise);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, fboTexture->colorBufferID[0]);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, fboCulling->colorBufferID[0]);

	float vec3[3];
	vec3[0] = (float)RES_X;
	vec3[1] = (float)RES_Y;
	vec3[2] = 0;
	
	vec3[0] = (float)sync_get_val(skyCol1_R, rocketRow);
	vec3[1] = (float)sync_get_val(skyCol1_G, rocketRow);
	vec3[2] = (float)sync_get_val(skyCol1_B, rocketRow);
	iLoc = glGetUniformLocation(shaderWater->shaderProg, "sky1");
	glUniform3fv(iLoc, 1, vec3);

	vec3[0] = (float)sync_get_val(skyCol2_R, rocketRow);
	vec3[1] = (float)sync_get_val(skyCol2_G, rocketRow);
	vec3[2] = (float)sync_get_val(skyCol2_B, rocketRow);
	iLoc = glGetUniformLocation(shaderWater->shaderProg, "sky2");
	glUniform3fv(iLoc, 1, vec3);

	iLoc = glGetUniformLocation(shaderWater->shaderProg, "lig");
	glUniform3fv(iLoc, 1, sunPos);

	iLoc = glGetUniformLocation(shaderWater->shaderProg, "cube");
	glUniform3fv(iLoc, 1, POS_CUBE.v);

	iLoc = glGetUniformLocation(shaderWater->shaderProg, "cube");
	glUniform3fv(iLoc, 1, POS_CUBE.v);

	iLoc = glGetUniformLocation(shaderWater->shaderProg, "objectPos");
	glUniform3fv(iLoc, 1, POS_OBJ.v);

	iLoc = glGetUniformLocation(shaderWater->shaderProg, "objectMix");
	glUniform1f(iLoc, objectMixVal);

	iLoc = glGetUniformLocation(shaderWater->shaderProg, "time");
	glUniform1f(iLoc, itime);

	iLoc = glGetUniformLocation(shaderWater->shaderProg, "iChannel0");
	glUniform1i(iLoc, 0);

	iLoc = glGetUniformLocation(shaderWater->shaderProg, "iChannel1");
	glUniform1i(iLoc, 1);

	iLoc = glGetUniformLocation(shaderWater->shaderProg, "mask");
	glUniform1i(iLoc, 2);

#define MATRIX_PROJECTION global:GetState()->matrixProjection
	//ZMatrix
	float v2[2];
	v2[0] = (float)RES_X;
	v2[1] = (float)RES_Y;
	iLoc = glGetUniformLocation(shaderWater->shaderProg, "g_resolution");
	glUniform2fv(iLoc, 1, v2);

	iLoc = glGetUniformLocation(shaderWater->shaderProg, "znear");
	glUniform1f(iLoc, fzn);

	iLoc = glGetUniformLocation(shaderWater->shaderProg, "zfar");
	glUniform1f(iLoc, fzf);

	iLoc = glGetUniformLocation(shaderWater->shaderProg, "pm");
	glUniformMatrix4fv(iLoc, 1, false, MAT_PROJ.m[0]);

	iLoc = glGetUniformLocation(shaderWater->shaderProg, "ipm");
	glUniformMatrix4fv(iLoc, 1, false, MAT_PROJ_INV.m[0]);

	iLoc = glGetUniformLocation(shaderWater->shaderProg, "mvm");
	glUniformMatrix4fv(iLoc, 1, false, MAT_MV.m[0]);

	iLoc = glGetUniformLocation(shaderWater->shaderProg, "imvm");
	glUniformMatrix4fv(iLoc, 1, false, MAT_MV_INV.m[0]);

	ZVector camPos = POS_EYE.Transform(MAT_MV_INV);
	iLoc = glGetUniformLocation(shaderWater->shaderProg, "cameraPosition");
	glUniform3fv(iLoc, 1, &camPos.x);

	ZVector lookd = POS_LOOK.Minus(POS_EYE).Normalize();
	iLoc = glGetUniformLocation(shaderWater->shaderProg, "cameraLookDirection");
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

	shaderWater->SetActive(false);

	// unbind FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

effect_glenz::~effect_glenz(void)
{
}
