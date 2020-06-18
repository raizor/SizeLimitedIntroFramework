#pragma once
#include "global.h"

class textures
{
public:
	UINT32 textureNoise3dFix;
	UINT32 textureNoise;

	textures();
	void makeNoise3d();
	~textures();
};

