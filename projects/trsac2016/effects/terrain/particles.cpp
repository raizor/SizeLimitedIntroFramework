#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <math.h>
#include "particles.h"
#include "framework/gl/gl8k.h"
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include "framework/gl/shader.h"
#include <GL/glu.h>

#include "clinkster/player/clinkster.h"
#include "../../shader_code_intro.h"
#include "config.h"
#include "rocket/rocketControl.h"
#include "music/syncData.h"
#include "framework/gl/fbo.h"
#include "framework\math\matrix.h"

#ifndef SYNC_PLAYER	
#include "../rocket/sync/device.h"
#include <stdio.h>
#endif

#include "framework/utils8k.h"
#include "global.h"

particles::particles(void)
{
	size = 256;

	#ifdef DEBUG
	shaderParticles = new shader(
		"shaders\\include\\particles.vert",
		"shaders\\include\\particles.geom",
		"shaders\\include\\particles.frag",
		"");

#else
	shaderParticles = new shader(
		(char*)particles_vert,
		(char*)particles_geom,
		(char*)particles_frag,
		""
		);
#endif

	// Alloc data
	ZPointGridVertex *vertices = new ZPointGridVertex[size*size];
	ZPointGridVertexIndex *indices = new ZPointGridVertexIndex[size*size];

	// Generate vertices
	for (int y=0; y<size; y++)
	{
		for (int x=0; x<size; x++)
		{
			float fx = (float)x/(float)size;
			float fy = (float)y/(float)size;

			int i = y*size + x;

			vertices[i].x = fx;
			vertices[i].y = fy;
			vertices[i].z = 0.f;
			vertices[i].w = 1.f;
			vertices[i].fx = fx;
			vertices[i].fy = fy;

			indices[i] = i;
		}
	}
	
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, size*size*sizeof(ZPointGridVertex), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Update vertex index data
	glGenBuffers(1, &indexBufferID);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size*size*sizeof(ZPointGridVertexIndex), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Release data
	delete[] vertices;
	delete[] indices;
}


void particles::draw(float time, ZMatrix* modelviewMatrix, ZMatrix* projectionMatrix, ZVector eye, ZVector rot)
{
#ifdef DEBUG
	shaderParticles->refresh();
#endif
	shaderParticles->SetActive(true);

	float vec3[3];
	vec3[0] = eye.x;
	vec3[1] = eye.y;
	vec3[2] = eye.z;

	int iLoc = glGetUniformLocation(shaderParticles->shaderProg, "projectionMatrix");
	glUniformMatrix4fv(iLoc, 1, false, projectionMatrix->m[0]);

	iLoc = glGetUniformLocation(shaderParticles->shaderProg, "modelViewMatrix");
	glUniformMatrix4fv(iLoc, 1, false, modelviewMatrix->m[0]);	

	iLoc = glGetUniformLocation(shaderParticles->shaderProg, "eye");
	glUniform3fv(iLoc, 1, vec3);

	iLoc = glGetUniformLocation(shaderParticles->shaderProg, "rot");
	glUniform3fv(iLoc, 1, &rot.x);

	iLoc = glGetUniformLocation(shaderParticles->shaderProg, "fTime");
	glUniform1f(iLoc, time);

	iLoc = glGetUniformLocation(shaderParticles->shaderProg, "aspectRatio");
	glUniform1f(iLoc, AR);

	float sz [] = {(float)RES_X, (float)RES_Y};
	iLoc = glGetUniformLocation(shaderParticles->shaderProg, "viewportSize");
	glUniform2fv(iLoc, 1, sz);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(ZPointGridVertex), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ZPointGridVertex), (GLvoid*)(4*sizeof(float)));
	//glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(ZMeshVertex2), (GLvoid*)(7*sizeof(float)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);

	glDrawElements(GL_POINTS, size*size, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	shaderParticles->SetActive(false);
}


particles::~particles(void)
{
}
