#pragma once
#include "global.h"

class fontBitmap
{
public:
	fontBitmap();
	~fontBitmap();
	GLuint textureId;
	GLbyte* pixels;

	color4f getBlockColor(int x, int y);
private:
	GLbyte* gltLoadPNG(int resourceId, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat);
	
};

class fontBitmapText
{
public:
	char* strText;
	fontBitmap* font;
	int numPixels;
	vec2* pixels;

	fontBitmapText(fontBitmap* font, char* text);
	~fontBitmapText();	
	
	void activate();
	void deactivate();
	GLuint getCullingBuffer();

private: 
	void drawCullingObjects();
	void drawCullCube(ZVector size, ZVector pos);

};
