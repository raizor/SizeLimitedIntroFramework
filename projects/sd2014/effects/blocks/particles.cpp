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

#include "particles.h"

extern int RES_X, RES_Y;
extern float timeStep;

particles::particles(shader* initProg)
{
	glGenBuffers(1, &vertexBufferID); // cube verts (quads)
	glGenBuffers(1, &indexBufferID); // cube verts (quads)
	glGenBuffers(1, &propsBufferID); // cube verts (quads)

	bufferWidth = RES_X / 2;
	bufferHeight = RES_Y / 2;

	// todo: add rotation and color			
	makeCube();

	// cube per pixel on 1/2 size buffer
	numCubes = bufferWidth * bufferHeight;

	properties = new cubeProps[numCubes];

	for (int i = 0; i < numCubes; i++)
	{
		properties[i] = 
		{ 
			v3{ 0.01f * i, 0.01f * i, 0.01f * i },
			v4{ 0, 0, 0, 0 },
			i
		};
	}

	// init fbos

	fboDesc.numColorBuffers = 1;
	fboDesc.colorBufferFormat = GL_RGBA32F;
	fboDesc.colorBufferInternalFormat = GL_RGBA;
	fboDesc.colorBufferType = GL_UNSIGNED_BYTE;

	fboDesc.height = bufferHeight;
	fboDesc.width = bufferWidth;
	fboDesc.depth = 0;

	fboDesc.hasDepthBuffer = false;
	fboDesc.depthBufferFormat = GL_DEPTH_COMPONENT16;

	fboDesc.enableMipmaps = false;
	fboDesc.magFilter = GL_LINEAR;
	fboDesc.minFilter = GL_LINEAR;
	fboDesc.wrapModeX = GL_CLAMP_TO_EDGE;
	fboDesc.wrapModeY = GL_CLAMP_TO_EDGE;
	fboDesc.textureType = GL_TEXTURE_2D;

	for (int i = 0; i < 2; i++)
	{
		GLuint noiseTex = Noise3D::GetHandle();

		// pos fbos
		fboPos[i] = new ZFbo(fboDesc);
		glBindFramebufferEXT(GL_FRAMEBUFFER, fboPos[i]->id);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_3D, noiseTex);
		initProg->SetActive(true);
		GLint loc = glGetUniformLocation(initProg->shaderProg, "NoiseTex");
		glUniform1i(loc, 0);
		glRecti(-1, -1, 1, 1);
		glBindTexture(GL_TEXTURE_3D, 0);
		initProg->SetActive(false);
		glBindFramebufferEXT(GL_FRAMEBUFFER, 0);

		// velocity fbos
		fboVel[i] = new ZFbo(fboDesc);
		glBindFramebufferEXT(GL_FRAMEBUFFER, fboVel[i]->id);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindFramebufferEXT(GL_FRAMEBUFFER, 0);

	}

	// fill cube buffers

	// positions
	glBindBuffer(GL_ARRAY_BUFFER, propsBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeProps)* numCubes, properties, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// cube verts
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)* 24, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	u32* buf = new u32[24];
	for (int i = 0; i < 24; i++)
	{
		buf[i] = i;
	}

	if (vertexBufferID == 0)
		glGenBuffers(1, &indexBufferID);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32)* 24, buf, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void particles::populateVertex4(vertex* v, float w, float h, float d, int idx, int x, int y, int z)
{
	v->pos = { w, h, d };
	//v->gridPos = { 0, 0, 0 };
}

void particles::populateNormal3(vertex* v, float x, float y, float z)
{
	v->norm = { x, y, z };
}

void particles::makeCube()
{
	vertices = new vertex[24];
	vertex* verts = vertices;
	//vertices+=numCubes * 24;
	int index = 0;
	float cubeSize = 1.0f;

	int x = 0, y = 0, z = 0;

	// target vertex pos
	// front 
	populateVertex4(&verts[index++], -cubeSize, -cubeSize, +cubeSize, numCubes, x, y, z);
	populateVertex4(&verts[index++], -cubeSize, +cubeSize, +cubeSize, numCubes, x, y, z);
	populateVertex4(&verts[index++], +cubeSize, +cubeSize, +cubeSize, numCubes, x, y, z);
	populateVertex4(&verts[index++], +cubeSize, -cubeSize, +cubeSize, numCubes, x, y, z);

	// back 
	populateVertex4(&verts[index++], -cubeSize, -cubeSize, -cubeSize, numCubes, x, y, z);
	populateVertex4(&verts[index++], -cubeSize, +cubeSize, -cubeSize, numCubes, x, y, z);
	populateVertex4(&verts[index++], +cubeSize, +cubeSize, -cubeSize, numCubes, x, y, z);
	populateVertex4(&verts[index++], +cubeSize, -cubeSize, -cubeSize, numCubes, x, y, z);

	// top
	populateVertex4(&verts[index++], -cubeSize, -cubeSize, +cubeSize, numCubes, x, y, z);
	populateVertex4(&verts[index++], -cubeSize, -cubeSize, -cubeSize, numCubes, x, y, z);
	populateVertex4(&verts[index++], +cubeSize, -cubeSize, -cubeSize, numCubes, x, y, z);
	populateVertex4(&verts[index++], +cubeSize, -cubeSize, +cubeSize, numCubes, x, y, z);

	// bottom
	populateVertex4(&verts[index++], -cubeSize, +cubeSize, +cubeSize, numCubes, x, y, z);
	populateVertex4(&verts[index++], -cubeSize, +cubeSize, -cubeSize, numCubes, x, y, z);
	populateVertex4(&verts[index++], +cubeSize, +cubeSize, -cubeSize, numCubes, x, y, z);
	populateVertex4(&verts[index++], +cubeSize, +cubeSize, +cubeSize, numCubes, x, y, z);

	// left
	populateVertex4(&verts[index++], -cubeSize, -cubeSize, -cubeSize, numCubes, x, y, z);
	populateVertex4(&verts[index++], -cubeSize, +cubeSize, -cubeSize, numCubes, x, y, z);
	populateVertex4(&verts[index++], -cubeSize, +cubeSize, +cubeSize, numCubes, x, y, z);
	populateVertex4(&verts[index++], -cubeSize, -cubeSize, +cubeSize, numCubes, x, y, z);

	// right
	populateVertex4(&verts[index++], +cubeSize, -cubeSize, -cubeSize, numCubes, x, y, z);
	populateVertex4(&verts[index++], +cubeSize, +cubeSize, -cubeSize, numCubes, x, y, z);
	populateVertex4(&verts[index++], +cubeSize, +cubeSize, +cubeSize, numCubes, x, y, z);
	populateVertex4(&verts[index++], +cubeSize, -cubeSize, +cubeSize, numCubes, x, y, z);

	// normals
	index = 0;
	// front
	populateNormal3(&verts[index++], 0.0f, 0.0f, 1.0f);
	populateNormal3(&verts[index++], 0.0f, 0.0f, 1.0f);
	populateNormal3(&verts[index++], 0.0f, 0.0f, 1.0f);
	populateNormal3(&verts[index++], 0.0f, 0.0f, 1.0f);

	// back
	populateNormal3(&verts[index++], 0.0f, 0.0f, -1.0f);
	populateNormal3(&verts[index++], 0.0f, 0.0f, -1.0f);
	populateNormal3(&verts[index++], 0.0f, 0.0f, -1.0f);
	populateNormal3(&verts[index++], 0.0f, 0.0f, -1.0f);

	// top
	populateNormal3(&verts[index++], 0.0f, -1.0f, 0.0f);
	populateNormal3(&verts[index++], 0.0f, -1.0f, 0.0f);
	populateNormal3(&verts[index++], 0.0f, -1.0f, 0.0f);
	populateNormal3(&verts[index++], 0.0f, -1.0f, 0.0f);

	// bottom
	populateNormal3(&verts[index++], 0.0f, 1.0f, 0.0f);
	populateNormal3(&verts[index++], 0.0f, 1.0f, 0.0f);
	populateNormal3(&verts[index++], 0.0f, 1.0f, 0.0f);
	populateNormal3(&verts[index++], 0.0f, 1.0f, 0.0f);


	// left
	populateNormal3(&verts[index++], -1.0f, 0.0f, 0.0f);
	populateNormal3(&verts[index++], -1.0f, 0.0f, 0.0f);
	populateNormal3(&verts[index++], -1.0f, 0.0f, 0.0f);
	populateNormal3(&verts[index++], -1.0f, 0.0f, 0.0f);

	// right
	populateNormal3(&verts[index++], 1.0f, 0.0f, 0.0f);
	populateNormal3(&verts[index++], 1.0f, 0.0f, 0.0f);
	populateNormal3(&verts[index++], 1.0f, 0.0f, 0.0f);
	populateNormal3(&verts[index++], 1.0f, 0.0f, 0.0f);
}

void particles::draw(shader* renderProg, shader* posProg, shader* velProg, ZFbo* screenFbo, float time)
{
	renderProg->refresh();
	posProg->refresh();
	velProg->refresh();

	float timeStepVal = timeStep + 0.0001f;

	int lastFrameNum = !frameNum;

	GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT };


	// **********************
	// update velocity
	// **********************

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboVel[frameNum]->id);
	glDrawBuffers(1, &buffers[0]);

	GLuint texHandle = Noise3D::GetHandle(1);
	GLuint texHandle2 = Noise3D::GetHandle();


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fboPos[lastFrameNum]->colorBufferID[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, fboVel[lastFrameNum]->colorBufferID[0]);

	int iLoc;

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_3D, texHandle);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_3D, texHandle2);

	velProg->SetActive(true);

	iLoc = glGetUniformLocation(velProg->shaderProg, "texOldPos");
	glUniform1i(iLoc, 0);

	iLoc = glGetUniformLocation(velProg->shaderProg, "texOldVel");
	glUniform1i(iLoc, 1);

	iLoc = glGetUniformLocation(velProg->shaderProg, "texNoise");
	glUniform1i(iLoc, 2);

	iLoc = glGetUniformLocation(velProg->shaderProg, "texNoise2");
	glUniform1i(iLoc, 3);

	iLoc = glGetUniformLocation(velProg->shaderProg, "time");
	glUniform1f(iLoc, time);

	iLoc = glGetUniformLocation(velProg->shaderProg, "timeStep");
	glUniform1f(iLoc, timeStepVal);

	iLoc = glGetUniformLocation(velProg->shaderProg, "accSpeed");
	glUniform1f(iLoc, 4);

	iLoc = glGetUniformLocation(velProg->shaderProg, "damping");
	glUniform1f(iLoc, 0.99000001f);

	glColor4f(1, 1, 1, 1);
	glRects(-1, -1, 1, 1);

	velProg->SetActive(false);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	// **********************
	// update particle pos
	// **********************

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboPos[frameNum]->id);
	glDrawBuffers(1, &buffers[0]);

	glColor4f(1, 1, 1, 1);

	posProg->SetActive(true);

	glActiveTexture(GL_TEXTURE0); // old pos
	glBindTexture(GL_TEXTURE_2D, fboPos[lastFrameNum]->colorBufferID[0]);
	iLoc = glGetUniformLocation(posProg->shaderProg, "texOldPos");
	glUniform1i(iLoc, 0);

	glActiveTexture(GL_TEXTURE1); // vel
	glBindTexture(GL_TEXTURE_2D, fboVel[frameNum]->colorBufferID[0]);
	iLoc = glGetUniformLocation(posProg->shaderProg, "texVel");
	glUniform1i(iLoc, 1);

	glActiveTexture(GL_TEXTURE2); // pos (screen world-space tex)
	glBindTexture(GL_TEXTURE_2D, screenFbo->colorBufferID[0]);
	iLoc = glGetUniformLocation(posProg->shaderProg, "texPos");
	glUniform1i(iLoc, 2);

	glActiveTexture(GL_TEXTURE3); // noise
	glBindTexture(GL_TEXTURE_3D, Noise3D::GetHandle());
	iLoc = glGetUniformLocation(posProg->shaderProg, "texNoise");
	glUniform1i(iLoc, 3);

	iLoc = glGetUniformLocation(posProg->shaderProg, "texPos");
	glUniform1i(iLoc, 2);

	iLoc = glGetUniformLocation(posProg->shaderProg, "time");
	glUniform1f(iLoc, time);

	iLoc = glGetUniformLocation(posProg->shaderProg, "timeStep");
	glUniform1f(iLoc, timeStepVal);

	iLoc = glGetUniformLocation(posProg->shaderProg, "velSpeed");
	glUniform1f(iLoc, 0.050000001f);

	glColor4f(1, 1, 1, 1);

	glRects(-1, -1, 1, 1);

	posProg->SetActive(false);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	//glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	//glCullFace();

	renderProg->refresh();

	glColor4f(1, 1, 1, 1);
	glTranslatef(0, 0, -350);
	glRotatef(time * 4, 0.8f, 1, 0);

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

	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(float)* 3)); // normal
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(v3), (GLvoid*)(sizeof(v4)*2)); // grid ref

	renderProg->SetActive(true);

	iLoc = glGetUniformLocation(renderProg->shaderProg, "matModelView");
	glUniformMatrix4fv(iLoc, 1, false, matrixModelView.v);

	iLoc = glGetUniformLocation(renderProg->shaderProg, "matProjection");
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

	//iLoc = glGetUniformLocation(prog->shaderProg, "matNormal");
	//glUniformMatrix3fv(prog->shaderProg, 1, false, m);

	glActiveTexture(GL_TEXTURE0); // noise
	glBindTexture(GL_TEXTURE_2D, fboPos[frameNum]->colorBufferID[0]);
	iLoc = glGetUniformLocation(renderProg->shaderProg, "texNoise");
	glUniform1i(iLoc, 0);

	glActiveTexture(GL_TEXTURE0); // pos
	glBindTexture(GL_TEXTURE_3D, Noise3D::GetHandle());
	iLoc = glGetUniformLocation(renderProg->shaderProg, "texPos");
	glUniform1i(iLoc, 1);

	iLoc = glGetUniformLocation(renderProg->shaderProg, "time");
	glUniform1f(iLoc, time);

	// shared cube vertices (for instances)
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(0)); // cube verts
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(v3))); // cube verts
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// positions for cubes instances
	glBindBuffer(GL_ARRAY_BUFFER, propsBufferID);

	// pos
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(cubeProps), (void*)(0));
	glVertexAttribDivisor(2, 1); // APPLY... each vertex: 0, each instance: 1, each second instance: 2 etc 

	// col
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(cubeProps), (void*)(sizeof(v3)));
	glVertexAttribDivisor(3, 1); // APPLY... each vertex: 0, each instance: 1, each second instance: 2 etc 

	// id
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(cubeProps), (void*)(sizeof(v3)+sizeof(v4)));
	glVertexAttribDivisor(4, 1); // APPLY... each vertex: 0, each instance: 1, each second instance: 2 etc 

	// cornerVec
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(cubeProps), (void*)(sizeof(v3)+sizeof(v4)+sizeof(unsigned int)));
	glVertexAttribDivisor(5, 1); // APPLY... each vertex: 0, each instance: 1, each second instance: 2 etc 

	// bind buffers for drawing
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	// draw
	glDrawArraysInstanced(GL_QUADS, 0, 24, numCubes);

	renderProg->SetActive(false);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);

	frameNum = !frameNum;
}

particles::~particles()
{
}
