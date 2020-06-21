#pragma once
#include "..\..\config.h"
#include "framework\utils8k.h"
#include "framework\gl\gl8k.h"
#include "framework\math\matrix.h"
#include "text3d.h"

class shader;
class noise;
class ZFbo;

class effect_showscene
{
public:
	unsigned int textureNoise;
	unsigned int textureColor;
	unsigned int textureNoise3d;
	unsigned int textureNoise3dFix;

	//GLUquadric* quadric;

	//noise* perlinNoise;
	shader* shaderTerrain;
	shader* shaderTerrainPlain;
	shader* shaderSphere;
	shader* shaderTerrainLogo;
	shader* shaderTerrainSnowman;
	text3d* txt3d;

	shader* shaderTest;
	shader* shaderPass;
	void draw(float itime);
	void reload();
	void drawTerrain(float itime);
	void drawCube(float time);
	void drawCullCube(ZVector size, ZVector pos);
	void drawCullSphere(float size, ZVector pos);
	void makeTextures(float time);
	void drawCullObjects(float time);
	effect_showscene();
	//void makeNoise3d();
	~effect_showscene(void);


};

