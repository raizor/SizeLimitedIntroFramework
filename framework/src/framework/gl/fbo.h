#pragma once

#include <windows.h>
#include "gl8k.h"

struct ZFboDesc
{
	int numColorBuffers;
	bool hasDepthBuffer;
	bool enableMipmaps;
	GLint colorBufferInternalFormat, colorBufferFormat, depthBufferFormat;
	GLint colorBufferType, depthBufferType;
	GLint width, height, depth;
	GLint minFilter, magFilter, wrapModeX, wrapModeY;
	GLenum textureType;
	void* pixelData = 0;

	public:
		ZFboDesc();
		ZFboDesc(GLint width, GLint height, GLint depth, int colorBufferInternalFormat, int colorBufferFormat, GLint colorBufferType, int numColorBuffers, bool hasDepthBuffer, GLint depthBufferFormat, GLint depthBufferType, bool enableMipmaps, GLenum textureType, GLint minfilter = GL_LINEAR, GLint magfilter = GL_LINEAR, void* pixels = NULL);
};

class ZFbo
{
	public:
		GLuint id;
		GLuint depthBufferID;
		GLuint colorBufferID[4];
		ZFboDesc description;

		ZFbo(ZFboDesc desc);
};