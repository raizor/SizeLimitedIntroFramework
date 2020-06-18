#include "textures.h"

textures::textures()
{
}


textures::~textures()
{
}

void textures::makeNoise3d()
{
	ZRandom* random = new ZRandom(0);
	int seed = 0;

	shader* noiseShader = new shader("shaders\\include\\blit.vert",
		NULL,
		"shaders\\include\\Noise3DLinear.frag",
		""
		);

	
	for (int i = 0; i < 1; i++)
	{
		// Large texture
		glGenTextures(1, &textureNoise3dFix);
		glBindTexture(GL_TEXTURE_3D, textureNoise3dFix);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB8, 256, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

		/*
		// Small texture
		unsigned char *texels = new unsigned char[16 * 16 * 16];
		for (int i = 0; i < 16 * 16 * 16; i++) texels[i] = random->randomInt() & 255;
		GLuint smallTex;
		glGenTextures(1, &smallTex);
		glBindTexture(GL_TEXTURE_3D, smallTex);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB8, 16, 16, 16, 0, GL_RED, GL_UNSIGNED_BYTE, texels);
		delete [] texels;
		*/
		
		// Layer fbo
		GLuint layerFbo;
		glGenFramebuffersEXT(1, &layerFbo);
		glBindFramebufferEXT(GL_FRAMEBUFFER, layerFbo);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureNoise);

		noiseShader->SetActive(true);
		
		// Generate large noise texture
		for (int i = 0; i < 256; i++)
		{
			glFramebufferTextureLayerEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureNoise3dFix, 0, i);
			glUniform1i(glGetUniformLocation(noiseShader->shaderProg, "t"), 0);
			glUniform1i(glGetUniformLocation(noiseShader->shaderProg, "textureLayer"), i);
			glRecti(-1, -1, 1, 1);
		}
				
		// Cleanup
		noiseShader->SetActive(false);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_3D, 0);
		glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
	}
}

