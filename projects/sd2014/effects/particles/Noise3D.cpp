#include "Noise3D.h"

const int Noise3D::numTextures = 2;

void Noise3D::Init()
{
	get();
}

GLuint Noise3D::GetHandle(int texNum)
{
	return get()->textures[texNum];
}

Noise3D::Noise3D()
{
	Noise3D *n = get();
	n->textures = new GLuint[numTextures];
	int seed = 0;
	for (int i = 0; i < numTextures; i++)
	{
		// Large texture
		GLuint tex;
		glGenTextures(1, &tex);
		n->textures[i] = tex;
		glBindTexture(GL_TEXTURE_3D, tex);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB8, 256, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

		// Small texture
		unsigned char *texels = new unsigned char[16 * 16 * 16];
		for (int i = 0; i < 16 * 16 * 16; i++) texels[i] = msys_rand(&seed) & 255;
		GLuint smallTex;
		glGenTextures(1, &smallTex);
		glBindTexture(GL_TEXTURE_3D, smallTex);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB8, 16, 16, 16, 0, GL_RED, GL_UNSIGNED_BYTE, texels);
		delete[] texels;

		// Layer fbo
		GLuint layerFbo;
		glGenFramebuffersEXT(1, &layerFbo);
		glBindFramebufferEXT(GL_FRAMEBUFFER, layerFbo);

		shader* shaderNoise = new shader(
			"shaders\\include\\Blit.vert",
			NULL,/*"shaders\\include\\voxels.geom",*/
			"shaders\\include\\Noise3DLinear.frag",
			""
			);

		shaderNoise->SetActive(true);

		// Generate large noise texture
		for (int i = 0; i < 256; i++)
		{
			glFramebufferTextureLayerEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex, 0, i);
			glUniform1i(glGetUniformLocation(shaderNoise->shaderProg, "t"), 0);
			glUniform1i(glGetUniformLocation(shaderNoise->shaderProg, "a"), i);
			glRecti(-1, -1, 1, 1);
		}

		// Cleanup
		shaderNoise->SetActive(false);
		glBindTexture(GL_TEXTURE_3D, 0);
		glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
	}
}

Noise3D::~Noise3D()
{
	for (int i = 0; i < numTextures; i++) glDeleteTextures(1, &(get()->textures[i]));
	delete[] textures;
}

Noise3D *Noise3D::get()
{
	static Noise3D n;
	return &n;
}
