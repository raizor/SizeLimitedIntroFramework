#pragma once
#include "..\..\config.h"
#include "refonter\refonter.h"
#include "..\noise.h"
#include "..\paths\splinePath.h"


class shader;
class noise;
class ZFbo;

class terrain
{
public:
	unsigned int textureNoise;
	unsigned int textureColor;
	unsigned int textureNoise3d;
	unsigned int textureNoise3dFix;

	noise* noise3d;

	ZSplinePath* path1;
	
	//noise* perlinNoise;
	shader* shaderTerrain;
	shader* shaderTest;
	shader* shaderPass;
	void draw(float itime);
	void reload();
	void drawTerrain(float itime);
	void drawCube(float time);
	void drawPath(float time);
	terrain();	
	void makeNoise3d();
	~terrain(void);


};

