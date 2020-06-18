#pragma once
#include "..\..\config.h"
#include "framework\utils8k.h"
#include "framework\gl\gl8k.h"
#include "framework/gl/shader.h"
#include "framework\math\matrix.h"
#include "blockGrid.h"
#include "Noise3D.h"
#include "blockScene.h"
#include "particles.h"

class shader;

class effectBlock
{
public:

	GLuint vertexBufferCubeID;
	GLuint indexBufferCubeID;

	blockGrid* grid;
	blockScene* scene;
	particles* parts;
	
	shader* shaderVoxels;
	shader* shaderNoise;
	shader* shaderInitVox;
	shader* shaderVoxPos;
	shader* shaderVoxVel;

	effectBlock();
	GLuint textureNoiseId;

	void draw(float time);
	void effectBlock::testNoise(float time);
};