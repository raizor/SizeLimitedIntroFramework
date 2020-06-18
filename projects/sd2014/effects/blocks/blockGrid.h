#pragma once
#include "..\..\config.h"
#include "framework\utils8k.h"
#include "framework\gl\gl8k.h"
#include "framework\math\matrix.h"
#include "image.h"
#include "introTypes.h"
#include "Noise3D.h"
#include "framework/gl/fbo.h"
#include "framework\math\matrix.h"

class blockGrid
{
public:
	int zSize;
	int zRes;
	GLint texSize;

	image* img;
	
		
	GLuint indexBufferID;
	
	ZFboDesc fboDesc;
	ZFbo* fboVel[2];
	ZFbo* fboPos[2];
	
	vertex* vertices;
	GLuint vertexBufferID;

	int numCubes;
	cubeProps* properties;
	GLuint propsBufferID;
	GLuint textureNoiseId;
	bool recalc;
	int frameNum = 0;

	//ZPointGridVertex *vertices = new ZPointGridVertex[size*size];
	//ZPointGridVertexIndex *indices = new ZPointGridVertexIndex[size*size];
	
	blockGrid(shader* initProg, GLuint textureNoiseId, int size, int res, const char* filename, const char* filenameHeight);
	void blockGrid::draw(shader* prog, shader* posProg, shader* velProg, float time);
	void blockGrid::makeCube();
};