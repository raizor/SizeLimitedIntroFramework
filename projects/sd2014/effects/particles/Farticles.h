#ifndef __FARTICLES_H__
#define __FARTICLES_H__

#include "..\..\config.h"
#include "framework\utils8k.h"
#include "framework\gl\gl8k.h"
#include "framework/gl/shader.h"
#include "framework\math\matrix.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "framework\gl\fbo.h"

class Farticles
{
public:
	enum RenderStyle
	{
		Solid,
		Additive,
	};

	static void Init();
	static ZFbo* GetFboHandle();
	static void Run(int style, glm::mat4& proj, glm::mat4& view);

private:
	Farticles();
	~Farticles();

	static Farticles *get();

	ZFbo *fbo;
	ZFbo *fbo2;



	GLint *vertParticlePosInit, *fragParticlePosInit;
	GLint *progParticlePosInit;
	
	GLint *vertParticleVelPass, *fragParticleVelPass;
	GLint *progParticleVelPass;

	GLint *vertParticlePosPass, *fragParticlePosPass;
	GLint *progParticlePosPass;

	GLint *vertParticleRender, *fragParticleRender;
	GLint *progParticleRender;

	GLint *vertParticleRenderAdditive, *fragParticleRenderAdditive;
	GLint *progParticleRenderAdditive;

	GLuint particleRenderBuffer;

	ZFbo *positionLifeFbos[2];
	ZFbo *velocityFbos[2];
	int bufferIndex;

	//Timer globalTimer;
	double lastTime;
};

#endif
