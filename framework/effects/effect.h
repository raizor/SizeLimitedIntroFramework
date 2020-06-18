#pragma once

#include <windows.h>
#include "../gl/gl8k.h"
#include "../gl/shader.h"

class effect
{

public:
	effect();
	virtual void Render();
	virtual void EndRender();
};