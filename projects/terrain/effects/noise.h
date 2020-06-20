#pragma once
#include "..\config.h"
#include "refonter\refonter.h"



class ZFbo;
class shader;

class noise
{
public:
	GLuint *textures;
	ZFbo* fboNoise;
	shader* prog;
	noise(void);
	void draw(float time);
	~noise(void);
};

