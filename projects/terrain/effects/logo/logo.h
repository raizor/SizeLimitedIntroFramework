#pragma once
#include "..\..\config.h"
#include "refonter\refonter.h"

class shader;

class logo
{
public:
	GLuint vertexBufferId;
	GLuint indexBufferId;
	shader* shaderText;
	refonter_tesselation_object* tesselatedFont;
	void draw(float itime);

	refonter_font* refonterFont;

	logo();	
	~logo(void);
	void makeLogo();


};

