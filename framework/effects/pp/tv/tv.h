#pragma once

#include <windows.h>
#include "framework/gl/gl8k.h"
#include "framework/effects/effect.h"

class tv : effect
{
	public:
		shader* prog;
		tv();
		void Render(GLuint colorBufferTextureId, float time, float flashAmount, float reflectionSize, float deformSize, float screenSize1, float screenSize2, float screenSize3, float viewportSize[2] );
};