#pragma once
#include "global.h"

class shader;
class noise;
class ZFbo;

class effect_glenz
{
public:
	unsigned int textureNoise;
	unsigned int textureColor;
	unsigned int textureNoise3d;
	unsigned int textureNoise3dFix;
	
	shader* shaderWater;

	void draw(float itime);
	void reload();
	void getSyncVars();
	effect_glenz();	
	//void makeNoise3d();
	~effect_glenz(void);
};

