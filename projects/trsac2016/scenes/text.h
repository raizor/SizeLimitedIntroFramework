#pragma once
#include "global.h"
#include "effects/font/fontBitmap.h"

class shader;
class noise;
class ZFbo;

class effect_text
{
public:
	UINT32 textureNoise;
	UINT32 textureColor;
	UINT32 textureNoise3d;
	UINT32 textureNoise3dFix;
	shader* shaderMain;

	fontBitmap* font;
	fontBitmapText* text;

	void draw(float itime);
	void draw_text(float itime);
	void draw_effect(float itime);
	void reload();
	
	effect_text(UINT32 texNoise, UINT32 texColor, UINT32 texNoise3d, UINT32 texNoise3dFix);
	~effect_text();

};