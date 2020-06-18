#include "noise.h"
#include "framework\utils8k.h"
#include "framework\gl\gl8k.h"
#include "framework\gl\shader.h"
#include "framework/gl/fbo.h"

#include "../testStuff/stb_image.h"
#include "../testStuff/noise26_png.h"

noise::noise(void)
{

	ZRandom* random = new ZRandom(0);
	int seed = 0;
	textures = new GLuint[2];


	shader* noiseShader = new shader("shaders\\include\\ssao.vs.glsl",
		NULL,
		"shaders\\include\\perlin2d.fs.glsl"
		);

	
	for (int i = 0; i < 1; i++)
	{
		// Large texture
		GLuint tex;
		glGenTextures(1, &tex);
		textures[i] = tex;
		glBindTexture(GL_TEXTURE_3D, tex);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F_ARB, 256, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

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

		noiseShader->SetActive(true);
		
		// Generate large noise texture
		for (int i = 0; i < 256; i++)
		{
			glFramebufferTextureLayerEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex, 0, i);
			glUniform1i(glGetUniformLocation(noiseShader->shaderProg, "t"), 0);
			glUniform1i(glGetUniformLocation(noiseShader->shaderProg, "textureLayer"), i);
			glRecti(-1, -1, 1, 1);
		}

		// Cleanup
		noiseShader->SetActive(false);
		glBindTexture(GL_TEXTURE_3D, 0);
		glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
	}
}

void noise::draw(float time)
{
	// reset the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// ortho view
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glOrtho( 0, RES_X , RES_Y , 0, 0, 1 );             // Select Ortho Mode (ie.640x480)

	glClearColor(0, 0, 0, 0);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fboNoise->colorBufferID[0]);

	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 1);
		glVertex2f(0, 0);

		glTexCoord2f(0, 0);
		glVertex2f(0, RES_X);

		glTexCoord2f(1, 0);
		glVertex2f(RES_X, RES_Y);

		glTexCoord2f(1, 1);
		glVertex2f(RES_X, 0);
	}		
	glEnd();

	glDisable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();
}


noise::~noise(void)
{
}