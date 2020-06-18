#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include "fbo.h"

ZFboDesc::ZFboDesc()
{
}

ZFboDesc::ZFboDesc(GLint width, GLint height, GLint depth, int colorBufferInternalFormat, int colorBufferFormat, GLint colorBufferType, int numColorBuffers, bool hasDepthBuffer, GLint depthBufferFormat, GLint depthBufferType, bool enableMipmaps, GLenum textureType, GLint minfilter, GLint magfilter, void* pixels)
{
	this->pixelData = pixels;
	this->minFilter = minfilter;
	this->magFilter = magfilter;
	this->wrapModeX = GL_REPEAT;
	this->wrapModeY = GL_REPEAT;
	this->width = width;
	this->height = height;
	this->depth = depth;
	this->colorBufferInternalFormat = colorBufferInternalFormat;
	this->colorBufferFormat = colorBufferFormat;
	this->colorBufferType = colorBufferType;
	this->numColorBuffers = numColorBuffers;
	this->hasDepthBuffer = hasDepthBuffer;
	this->depthBufferType = depthBufferType;
	this->enableMipmaps = enableMipmaps;
	this->textureType = textureType;	
	this->depthBufferFormat = depthBufferFormat;
}


ZFbo::ZFbo(ZFboDesc desc)
{
	this->description = desc;
	// create n RGBA color textures
	glGenTextures(desc.numColorBuffers, colorBufferID);

	for (int i = 0; i <desc.numColorBuffers; ++i) 
	{
		glBindTexture(desc.textureType, colorBufferID[i]);
		glTexImage2D(desc.textureType, 0, desc.colorBufferFormat, desc.width, desc.height, 0, desc.colorBufferInternalFormat, desc.colorBufferType, desc.pixelData);
		glTexParameteri(desc.textureType, GL_TEXTURE_MIN_FILTER, description.minFilter);
		glTexParameteri(desc.textureType, GL_TEXTURE_MIN_FILTER, description.magFilter);
		glTexParameteri(desc.textureType, GL_TEXTURE_WRAP_S, description.wrapModeX);
		glTexParameteri(desc.textureType, GL_TEXTURE_WRAP_T, description.wrapModeY);
	}
	
	// create the frame buffer object
	glGenFramebuffersEXT(1, &id);
	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER, id); 

	//glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, depthBufferID, 0);

	// attach colors
	for (int i = 0; i < desc.numColorBuffers; ++i) 
	{
		glFramebufferTextureEXT(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0_EXT + i , colorBufferID[i], 0);
	}

	if (desc.hasDepthBuffer)
	{
		glGenTextures(1, &depthBufferID);
		glBindTexture(GL_TEXTURE_2D, depthBufferID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, desc.width, desc.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
		glFramebufferTextureEXT(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthBufferID, 0);
		glBindTexture(desc.textureType, depthBufferID);
	}
	
	
#ifdef DEBUG
	//-------------------------
	//Does the GPU support current FBO configuration?
	if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT)
		DebugBreak();
#endif

	
	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER, 0);
}
