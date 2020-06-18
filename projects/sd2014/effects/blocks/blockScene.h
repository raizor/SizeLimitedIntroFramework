#pragma once
#include "..\..\config.h"
#include "framework\utils8k.h"
#include "framework\gl\gl8k.h"
#include "framework\math\matrix.h"
#include "framework\gl\fbo.h"

class blockScene
{
public:
	ZFbo* fboScene;
	ZFboDesc fboDesc;
	int bufferWidth, bufferHeight;

	blockScene();
	~blockScene();

	shader* shaderRender;
	void blockScene::draw(float time);
	void blockScene::drawCube();
};

