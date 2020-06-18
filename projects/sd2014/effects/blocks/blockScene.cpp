#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <math.h>
#include "framework/gl/gl8k.h"
#include "framework/gl/shader.h"
#include <GL/glu.h>


#include "clinkster.h"
#include "../../shader_code_intro.h"
#include "config.h"
#include "rocket/rocketControl.h"
#include "music/syncData.h"
#include "framework/gl/fbo.h"
#include "framework\math\matrix.h"

#ifndef SYNC_PLAYER	
#include "rocket/sync/device.h"
#include <stdio.h>
#endif

#include "framework/utils8k.h"

#include "stb_image.h"

#include "blockGrid.h"
#include "block.h"

#include "blockScene.h"

static const float cubeVerts[] = {

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

blockScene::blockScene()
{
	bufferWidth = RES_X / 2;
	bufferHeight = RES_Y / 2;

#ifdef DEBUG

	shaderRender = new shader(
		"shaders\\include\\sceneRender.vert",
		NULL,
		"shaders\\include\\sceneRender.frag",
		"",
		"sceneRender"
		);
#else
		
#endif

	fboDesc.numColorBuffers = 3;
	fboDesc.colorBufferFormat = GL_RGBA32F;
	fboDesc.colorBufferInternalFormat = GL_RGBA;
	fboDesc.colorBufferType = GL_UNSIGNED_BYTE;

	fboDesc.height = bufferHeight;
	fboDesc.width = bufferWidth;
	fboDesc.depth = 0;

	fboDesc.hasDepthBuffer = true;
	fboDesc.depthBufferFormat = GL_DEPTH_COMPONENT16;

	fboDesc.enableMipmaps = false;
	fboDesc.magFilter = GL_LINEAR;
	fboDesc.minFilter = GL_LINEAR;
	fboDesc.wrapModeX = GL_CLAMP_TO_EDGE;
	fboDesc.wrapModeY = GL_CLAMP_TO_EDGE;
	fboDesc.textureType = GL_TEXTURE_2D;

	fboScene = new ZFbo(fboDesc);
}


blockScene::~blockScene()
{
}

void blockScene::drawCube()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 6 * sizeof(float), cubeVerts + 0);
	glNormalPointer(GL_FLOAT, 6 * sizeof(float), cubeVerts + 3);
	glDrawElements(GL_QUADS, 6 * 4, GL_UNSIGNED_INT, cubeIndices);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void blockScene::draw(float time)
{
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	glUseProgram(0);
	glTranslatef(0, 0, -5);
	glRotatef(time*10, 0.1f, 0.2f, 0.3f);
	
	shaderRender->SetActive(true);

	ZMatrix matrixProjection = ZMatrix();
	ZMatrix matrixProjectionInverse = ZMatrix();
	ZMatrix matrixModelView = ZMatrix();
	ZMatrix matrixModelViewInverse = ZMatrix();
	float normalMatrix[9];

	float matrix[16];

	glGetFloatv(GL_PROJECTION_MATRIX, matrix);
	matrixProjection = ZMatrix::CreateIdentity();
	memcpy(matrixProjection.m, matrix, sizeof(float)* 16);

	matrixProjectionInverse = ZMatrix::CreateIdentity();
	matrixProjectionInverse = matrixProjection.Inverse();

	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	//	int iLoc = glGetUniformLocation(shaderTerrain->shaderProg, "modelviewMatrix");
	//	glUniformMatrix4fv(iLoc, 1, false, matrix);	

	matrixModelView = ZMatrix::CreateIdentity();
	matrixModelViewInverse = ZMatrix::CreateIdentity();
	memcpy(matrixModelView.m, matrix, sizeof(float)* 16);
	matrixModelViewInverse = matrixModelView.Inverse();

	// normal matrix
	ZMatrix n = matrixModelViewInverse.Transpose();
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			normalMatrix[i * 3 + j] = n.m[i][j];
		}
	}

	GLuint iLoc = glGetUniformLocation(shaderRender->shaderProg, "Color");
	float v[3] = { 1, 1, 1 };
	glUniform3fv(iLoc, 1, v);

	iLoc = glGetUniformLocation(shaderRender->shaderProg, "matModelView");
	glUniformMatrix4fv(iLoc, 1, false, matrixModelView.v);

	iLoc = glGetUniformLocation(shaderRender->shaderProg, "matProjection");
	glUniformMatrix4fv(iLoc, 1, false, matrixProjection.v);

	// Compute normal matrix
	ZMatrix normal = matrixModelView.Inverse().Transpose();

	// Create 3x3 matrix
	float m[9];

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			m[i * 3 + j] = normal.m[i][j];
		}
	}

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, fboPos[frameNum]->colorBufferID[0]);

//	iLoc = glGetUniformLocation(shaderRender->shaderProg, "matNormal");
//	glUniformMatrix3fv(shaderRender->shaderProg, 1, false, m);

	GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT };
	glBindFramebufferEXT(GL_FRAMEBUFFER, fboScene->id);
	drawCube();
	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
	shaderRender->SetActive(false);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_COLOR_MATERIAL);
}