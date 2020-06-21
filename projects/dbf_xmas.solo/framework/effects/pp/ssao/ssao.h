#pragma once

#include <windows.h>
#include "framework/gl/gl8k.h"
#include "framework/effects/effect.h"

class ssao : effect
{
	public:
		shader* prog;
		ssao();
		void Render(GLuint colorBufferTextureId, GLuint depthBufferTextureId, float zNear, float zFar);
};