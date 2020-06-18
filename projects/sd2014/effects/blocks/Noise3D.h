#pragma once
#include "..\..\config.h"
#include "framework\utils8k.h"
#include "framework\gl\gl8k.h"
#include "framework/gl/shader.h"
#include "framework\math\matrix.h"
#include "blockGrid.h"
#include "Noise3D.h"

class Noise3D
{
public:
	static void Init();
	static GLuint GetHandle(int texNum = 0);

	static const int numTextures;

	GLuint *textures;

private:
	Noise3D();
	~Noise3D();

	static Noise3D *get();

};

