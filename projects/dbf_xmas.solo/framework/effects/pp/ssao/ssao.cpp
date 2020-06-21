#include "ssao.h"
#include "framework/utils8k.h"
#include "global.h"
#include "framework/gl/shaders/shader_code_framework.h"

#pragma warning(disable: 4102)

ssao::ssao() : effect()
{
#ifdef DEBUG
	prog = new shader
	(
		"shaders\\include\\ssao.vs.glsl",
		NULL,
		"shaders\\include\\ssao.fs.glsl",
		"", 
		"SSAO"
	);
#else
	prog = new shader(
		ssao_vs_glsl,
		"",
		ssao_fs_glsl,
		"",
		"SSAO"
	);
#endif
}

void ssao::Render(GLuint colorBufferTextureId, GLuint depthBufferTextureId, float zNear, float zFar)
{
	effect::Render();

	prog->SetActive(true);

	GLuint iLoc = glGetUniformLocation(prog->shaderProg, "bgl_RenderedTexture");
	glUniform1i(iLoc, 0);

	iLoc = glGetUniformLocation(prog->shaderProg, "bgl_DepthTexture");
	glUniform1i(iLoc, 1);
	
	iLoc = glGetUniformLocation(prog->shaderProg, "bgl_RenderedTextureWidth");
	glUniform1f(iLoc, RES_X);

	iLoc = glGetUniformLocation(prog->shaderProg, "bgl_RenderedTextureHeight");
	glUniform1f(iLoc, RES_Y);

	iLoc = glGetUniformLocation(prog->shaderProg, "zNear");
	glUniform1f(iLoc, zNear);

	iLoc = glGetUniformLocation(prog->shaderProg, "zFar");
	glUniform1f(iLoc, zFar);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBufferTextureId);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthBufferTextureId);

	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 1);
		glVertex2f(0, 0);

		glTexCoord2f(0, 0);
		glVertex2f(0, RES_Y);

		glTexCoord2f(1, 0);
		glVertex2f(RES_X, RES_Y);

		glTexCoord2f(1, 1);
		glVertex2f(RES_X, 0);
	}		
	glEnd();
	
	prog->SetActive(false);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	effect::EndRender();
}