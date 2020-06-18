#pragma once
#include "..\..\config.h"
#include "framework\utils8k.h"
#include "framework\gl\gl8k.h"
#include "framework/gl/shader.h"
#include "framework\math\matrix.h"
#include "blockGrid.h"
#include "Noise3D.h"

class particles
{
public:
	int frameNum = 0;
	int bufferWidth, bufferHeight;
	int numCubes;
	cubeProps* properties;
	GLuint propsBufferID;

	ZFboDesc fboDesc;
	ZFbo* fboVel[2];
	ZFbo* fboPos[2];

	vertex* vertices;
	GLuint vertexBufferID;
	GLuint indexBufferID;

	particles(shader* initProg);
	void particles::draw(shader* prog, shader* posProg, shader* velProg, ZFbo* screenFbo, float time);
	void particles::makeCube();
	void particles::populateVertex4(vertex* v, float w, float h, float d, int idx, int x, int y, int z);
	void particles::populateNormal3(vertex* v, float x, float y, float z);
	~particles();
};

