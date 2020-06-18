#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <math.h>
#include "rm.h"
#include "framework/gl/gl8k.h"
#include "framework/gl/shader.h"
#include <GL/glu.h>

#include "clinkster/player/clinkster.h"
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

#include "global.h"

rm::rm()
{

#ifdef DEBUG
	shaderMain = new shader(
		"shaders\\include\\water.vert",
		NULL,
		"shaders\\include\\water.frag",
		"",
		"water"
		);
#else
	shaderMain = new shader(
		(char*)rm_vert,
		NULL,
		(char*)rm_frag,
		""
		);
#endif
}

#ifdef DEBUG
void rm::reload()
{
	//shaderTerrainPlain->reload();
	shaderMain->reload();
}
#endif

void rm::draw_effect(float itime)
{
#ifdef DEBUG
	shaderMain->refresh();
#endif
	shaderMain->SetActive(true);
	
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, perlinNoise->fboNoise->colorBufferID[0]);
	glBindTexture(GL_TEXTURE_2D, textureNoise);

	float vec3[3];
	vec3[0] = (float)RES_X;
	vec3[1] = (float)RES_Y;
	vec3[2] = 0;

	GLint iLoc;

	vec3[0] = (float)sync_get_val(skyCol1_R, ROCKET_ROW);
	vec3[1] = (float)sync_get_val(skyCol1_G, ROCKET_ROW);
	vec3[2] = (float)sync_get_val(skyCol1_B, ROCKET_ROW);
	iLoc = glGetUniformLocation(shaderMain->shaderProg, "sky1");
	glUniform3fv(iLoc, 1, vec3);

	vec3[0] = (float)sync_get_val(skyCol2_R, ROCKET_ROW);
	vec3[1] = (float)sync_get_val(skyCol2_G, ROCKET_ROW);
	vec3[2] = (float)sync_get_val(skyCol2_B, ROCKET_ROW);
	iLoc = glGetUniformLocation(shaderMain->shaderProg, "sky2");
	glUniform3fv(iLoc, 1, vec3);

	iLoc = glGetUniformLocation(shaderMain->shaderProg, "lig");
	glUniform3fv(iLoc, 1, SYNC_LIGHT_1_POS);

	iLoc = glGetUniformLocation(shaderMain->shaderProg, "cube");
	glUniform3fv(iLoc, 1, POS_CUBE.v);

	iLoc = glGetUniformLocation(shaderMain->shaderProg, "objectPos");
	glUniform3fv(iLoc, 1, POS_OBJ.v);

	iLoc = glGetUniformLocation(shaderMain->shaderProg, "objectMix");
	glUniform1f(iLoc, SYNC_OBJ_MIX_1);

	iLoc = glGetUniformLocation(shaderMain->shaderProg, "time");
	glUniform1f(iLoc, itime);

	iLoc = glGetUniformLocation(shaderMain->shaderProg, "iChannel0");
	glUniform1i(iLoc, 0);

	iLoc = glGetUniformLocation(shaderMain->shaderProg, "iChannel1");
	glUniform1i(iLoc, 1);

	iLoc = glGetUniformLocation(shaderMain->shaderProg, "mask");
	glUniform1i(iLoc, 2);

	#define MATRIX_PROJECTION global:GetState()->matrixProjection

	//ZMatrix
	float v2[2];
	v2[0] = (float)RES_X;
	v2[1] = (float)RES_Y;
	iLoc = glGetUniformLocation(shaderMain->shaderProg, "g_resolution");
	glUniform2fv(iLoc, 1, v2);

	iLoc = glGetUniformLocation(shaderMain->shaderProg, "znear");
	glUniform1f(iLoc, fzn);

	iLoc = glGetUniformLocation(shaderMain->shaderProg, "zfar");
	glUniform1f(iLoc, fzf);

	iLoc = glGetUniformLocation(shaderMain->shaderProg, "pm");
	glUniformMatrix4fv(iLoc, 1, false, MAT_PROJ.m[0]);

	iLoc = glGetUniformLocation(shaderMain->shaderProg, "ipm");
	glUniformMatrix4fv(iLoc, 1, false, MAT_PROJ_INV.m[0]);

	iLoc = glGetUniformLocation(shaderMain->shaderProg, "mvm");
	glUniformMatrix4fv(iLoc, 1, false, MAT_MV.m[0]);

	iLoc = glGetUniformLocation(shaderMain->shaderProg, "imvm");
	glUniformMatrix4fv(iLoc, 1, false, MAT_MV_INV.m[0]);

	ZVector camPos = POS_EYE.Transform(MAT_MV_INV);
	iLoc = glGetUniformLocation(shaderMain->shaderProg, "cameraPosition");
	glUniform3fv(iLoc, 1, &camPos.x);

	ZVector lookd = POS_LOOK.Minus(POS_EYE).Normalize();
	iLoc = glGetUniformLocation(shaderMain->shaderProg, "cameraLookDirection");
	glUniform3fv(iLoc, 1, &lookd.x);

	//glDisable(GL_BLEND);
	//glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glRectf(-1, -1, 1, 1);
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

	shaderMain->SetActive(false);
}

void rm::draw(float itime)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	// reset the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	POS_EYE.x = (float)sync_get_val(syncEyeX, ROCKET_ROW);
	POS_EYE.y = (float)sync_get_val(syncEyeY, ROCKET_ROW);
	POS_EYE.z = (float)sync_get_val(syncEyeZ, ROCKET_ROW);

	POS_CUBE.x = (float)sync_get_val(cubeX, ROCKET_ROW);
	POS_CUBE.y = (float)sync_get_val(cubeY, ROCKET_ROW);
	POS_CUBE.z = (float)sync_get_val(cubeZ, ROCKET_ROW);

	POS_OBJ.x = (float)sync_get_val(objectX, ROCKET_ROW);
	POS_OBJ.y = (float)sync_get_val(objectY, ROCKET_ROW);
	POS_OBJ.z = (float)sync_get_val(objectZ, ROCKET_ROW);
	
	SYNC_CAM_RADIUS = (float)sync_get_val(camRadius, ROCKET_ROW);

	int objNum = (int)sync_get_val(objectNum, ROCKET_ROW);


	float roll = (float)sync_get_val(rotRoll, ROCKET_ROW);
	float pitch = (float)sync_get_val(rotPitch, ROCKET_ROW);
	float heading = (float)sync_get_val(rotHeading, ROCKET_ROW);

	glTranslatef(0, 0, SYNC_CAM_RADIUS);
	glRotatef(pitch, 1.0f, 0.0f, 0.0f);
	glRotatef(heading, 0.0f, 1.0f, 0.0f);
	glRotatef(roll, 0.0f, 0.0f, 1.0f);
	glTranslatef(POS_EYE.x, POS_EYE.y, POS_EYE.z);

	float matrix[16];

	glGetFloatv(GL_PROJECTION_MATRIX, matrix);
	MAT_PROJ = ZMatrix::CreateIdentity();
	memcpy(MAT_PROJ.m, matrix, sizeof(float)* 16);

	MAT_PROJ_INV = ZMatrix::CreateIdentity();
	MAT_PROJ_INV = MAT_PROJ_INV.Inverse();
	
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

#if ENABLE_PP
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FBO->id);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
#endif

	// terrain
	draw_effect(itime);

#if ENABLE_PP
	// unbind FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
#endif
}

