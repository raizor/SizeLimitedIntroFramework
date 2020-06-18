#pragma once
#include "framework\utils8k.h"
#include "framework\gl\gl8k.h"
#include "framework\math\matrix.h"

class shader;
class noise;
class ZFbo;

class rm
{
public:
	unsigned int textureNoise;
	unsigned int textureColor;
	unsigned int textureNoise3d;
	unsigned int textureNoise3dFix;

	//GLUquadric* quadric;

	//noise* perlinNoise;
	shader* shaderMain;
	void draw(float itime);
	void draw_effect(float itime);
	void reload();
	rm();
	//void makeNoise3d();
	~rm(void);


};

