#pragma once
#include "framework\utils8k.h"
#include "framework\gl\gl8k.h"
#include "framework\math\matrix.h"

class shader;
//class noise;
class ZFbo;

class particles
{
public:
	int size;
	GLuint vertexBufferID;
	GLuint indexBufferID;
	shader* shaderParticles;
	void draw(float itime, ZMatrix* modelviewMatrix, ZMatrix* projectionMatrix, ZVector eye, ZVector rot);
	void reload();
	particles();	
	~particles(void);


};

