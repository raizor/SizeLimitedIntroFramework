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

void populateVertex4(vertex* v, float w, float h, float d, int idx, int x, int y, int z)
{
	v->pos = { w, h, d };
	//v->gridPos = { 0, 0, 0 };
}

void populateNormal3(vertex* v, float x, float y, float z)
{
	v->norm = { x, y, z };
}

void blockGrid::makeCube()
{
	vertices = new vertex[24];
	vertex* verts = vertices;
	//vertices+=numCubes * 24;
	int index = 0;
	float cubeSize = 1.0f;
	
	int x=0, y =0, z = 0;

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

blockGrid::blockGrid(shader* initProg, GLuint textureNoiseId, int size, int res, const char* filenameColor, const char* filenameHeight)
{
	numCubes = 0;
	zRes = res;
	zSize = size;
	img = new image(filenameColor, filenameHeight);
	this->textureNoiseId = textureNoiseId;

	recalc = false;

	glGenBuffers(1, &vertexBufferID); // cube verts (quads)
	glGenBuffers(1, &propsBufferID); // positions of cubes

	// todo: add rotation and color			
	makeCube();
	
	int ix = 0;
	
	//properties = new cubeProps[img->width*img->height*(img->depthFront+img->depthBack)];
	properties = new cubeProps[img->width*img->height*(img->depthFront+img->depthBack)];
	float midX = (float)img->width;
	float midY = (float)img->height;
	float z = 0;
	for (int py = 0; py < img->height; py++)
	{
		for (int px = 0; px < img->width; px++)
		{
			pixel* p = (pixel*)&img->pixels[((img->width * py) + px)];
			pixel* pDepth = (pixel*)&img->pixelsDepth[((img->width * py) + px)];
			float offY = 2.0f * py;
			float offX = 2.0f * px;

			if (p->a > 0)
			{
				// draw
				properties[numCubes] = { v3{ (float)-midX + offX, (float)-midY + offY, 0 }, p->col() };
				numCubes++;

				float imageDepth = (float)pDepth->r / 255.0f;
				//float depthFront = 

				// handle rear extrusion
				float depthBack = imageDepth * img->depthBack;								
				for (int i = 0; i < img->depthBack; i++)
				{
					if (i <= depthBack)
					{
						properties[numCubes] = { v3{ (float)-midX + offX, (float)-midY + offY, (float)-i*2 }, p->col() };
						numCubes++;
					}
				}

				// handle front extrusion
				float depthFront = imageDepth * img->depthFront;
				for (int i = 0; i < img->depthFront; i++)
				{
					if (i <= depthFront)
					{
						properties[numCubes] = { v3{ (float)-midX + offX, (float)-midY + offY, (float)i*2 }, p->col() };
						numCubes++;
					}
				}
			}
		}
	}

	// positions
	glBindBuffer(GL_ARRAY_BUFFER, propsBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeProps)* numCubes, properties, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// cube verts
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*24, vertices, GL_STATIC_DRAW);
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

	// width/height of the fbo lookup textures
	texSize = (int)(sqrt((double)numCubes))+1;

	v4* pixels = new v4[texSize*texSize];

	for (int i = 0; i < numCubes; i++)
	{
		v4* pix = &pixels[i];
		/*
		pix->x = properties[i].pos.x;
		pix->y = properties[i].pos.y;
		pix->z = properties[i].pos.z;
		pix->w = 1;
		*/
		//pix->x = 0.5;
		//pix->z = 0.75;
	}

	fboDesc.numColorBuffers = 1;
	fboDesc.colorBufferFormat = GL_RGBA32F;
	fboDesc.colorBufferInternalFormat = GL_RGBA;
	fboDesc.colorBufferType = GL_UNSIGNED_BYTE;

	fboDesc.height = texSize;
	fboDesc.width = texSize;
	fboDesc.depth = 0;

	fboDesc.hasDepthBuffer = false;
	fboDesc.depthBufferFormat = GL_DEPTH_COMPONENT16;

	fboDesc.enableMipmaps = false;	
	fboDesc.magFilter = GL_LINEAR;
	fboDesc.minFilter = GL_LINEAR;
	fboDesc.wrapModeX = GL_CLAMP_TO_EDGE;
	fboDesc.wrapModeY = GL_CLAMP_TO_EDGE;
	fboDesc.textureType = GL_TEXTURE_2D;
	fboDesc.pixelData = pixels;
	
	for (int i = 0; i < 2; i++)
	{
		fboPos[i] = new ZFbo(fboDesc);
		glBindFramebufferEXT(GL_FRAMEBUFFER, fboPos[i]->id);

		GLuint noiseTex = Noise3D::GetHandle();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_3D, noiseTex);
		initProg->SetActive(true);
		GLint loc = glGetUniformLocation(initProg->shaderProg, "NoiseTex");
		glUniform1i(loc, 0);
		glRecti(-1, -1, 1, 1);
		glBindTexture(GL_TEXTURE_3D, 0);
		initProg->SetActive(true);

	}

	for (int i = 0; i < 2; i++)
	{
		fboVel[i] = new ZFbo(fboDesc);
		glBindFramebufferEXT(GL_FRAMEBUFFER, fboVel[i]->id);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
	}

	// init the fbos
	initProg->SetActive(true);



	initProg->SetActive(false);
	

	int xx = 1;
}

extern float timeStep;

void CheckGlError()
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		DebugBreak();
	}
}

extern HWND hWndMain;

void blockGrid::draw(shader* prog, shader* posProg, shader* velProg, float time)
{
	//MessageBoxA(hWndMain, NULL, NULL, 0);

	prog->refresh();
	posProg->refresh();
	velProg->refresh();

	int lastFrameNum = !frameNum;
	GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT };

	// **********************
	// update velocity
	// **********************

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboVel[frameNum]->id);
	glDrawBuffers(1, &buffers[0]);

	GLuint texHandle = Noise3D::GetHandle(frameNum);
	GLuint texHandle2 = Noise3D::GetHandle(lastFrameNum);


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
		glUniform1f(iLoc, timeStep+0.0001f);

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
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fboPos[lastFrameNum]->colorBufferID[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, fboVel[frameNum]->colorBufferID[0]);
	
	posProg->SetActive(true);
	
		iLoc = glGetUniformLocation(posProg->shaderProg, "texOldPos");
		glUniform1i(iLoc, 0);

		iLoc = glGetUniformLocation(posProg->shaderProg, "texVel");
		glUniform1i(iLoc, 1);

		iLoc = glGetUniformLocation(posProg->shaderProg, "time");
		glUniform1f(iLoc, time);

		iLoc = glGetUniformLocation(posProg->shaderProg, "timeStep");
		glUniform1f(iLoc, timeStep + 0.0001f);

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

	prog->refresh();

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
	for (int i = 0; i<3; i++)
	{
		for (int j = 0; j<3; j++)
		{
			normalMatrix[i * 3 + j] = n.m[i][j];
		}
	}
					
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(float)* 3)); // normal
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(v3), (GLvoid*)(sizeof(v4)*2)); // grid ref

	prog->SetActive(true);
	
	iLoc = glGetUniformLocation(prog->shaderProg, "matModelView");
	glUniformMatrix4fv(iLoc, 1, false, matrixModelView.v);

	iLoc = glGetUniformLocation(prog->shaderProg, "matProjection");
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

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fboPos[frameNum]->colorBufferID[0]);

	iLoc = glGetUniformLocation(prog->shaderProg, "matNormal");
	glUniformMatrix3fv(prog->shaderProg, 1, false, m);

	iLoc = glGetUniformLocation(prog->shaderProg, "texNoise");
	glUniform1i(iLoc, 0);

	iLoc = glGetUniformLocation(prog->shaderProg, "time");
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

	// bind buffers for drawing
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	// draw
	glDrawArraysInstanced(GL_QUADS, 0, 24, numCubes);
	
	prog->SetActive(false);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);

	frameNum = !frameNum;
}