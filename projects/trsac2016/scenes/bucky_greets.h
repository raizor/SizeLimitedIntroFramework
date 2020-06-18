#pragma once
#include "global.h"

class shader;
class noise;
class ZFbo;

class effect_greets
{
public:
	UINT32 textureNoise;
	UINT32 textureColor;
	UINT32 textureNoise3d;
	UINT32 textureNoise3dFix;
	shader* shaderMain;

	void draw(float itime);
	void draw_effect(float itime);
	void reload();

	effect_greets(UINT32 texNoise, UINT32 texColor, UINT32 texNoise3d, UINT32 texNoise3dFix);
	~effect_greets();

};