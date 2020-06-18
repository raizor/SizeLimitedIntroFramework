#include "tv.h"
#include "framework/utils8k.h"

#pragma warning(disable: 4102)

tv::tv() : effect()
{
#ifdef DEBUG
	prog = new shader(
		"shaders\\include\\tv.vs.glsl",
		NULL,
		"shaders\\include\\tv.fs.glsl"
		);
#else
	prog = new shader(
		&tv_vs_glsl,
		&tv_gs_glsl,
		&tv_fs_glsl,
		);
#endif
}

void tv::Render(GLuint colorBufferTextureId, float time, float flashAmount, float reflectionSize, float deformSize, float screenSize1, float screenSize2, float screenSize3, float viewportSize[2] )
{
	effect::Render();

	prog->SetActive(true);

	GLuint iLoc = glGetUniformLocation(prog->shaderProg, "input0");
	glUniform1i(iLoc, 0);

	iLoc = glGetUniformLocation(prog->shaderProg, "viewportSize");
	glUniform2fv(iLoc, 1, viewportSize);
	
	iLoc = glGetUniformLocation(prog->shaderProg, "fTime");
	glUniform1f(iLoc, time);

	iLoc = glGetUniformLocation(prog->shaderProg, "fFlashAmount");
	glUniform1f(iLoc, flashAmount);

	iLoc = glGetUniformLocation(prog->shaderProg, "fReflectionSize");
	glUniform1f(iLoc, reflectionSize);

	iLoc = glGetUniformLocation(prog->shaderProg, "fDeformValue");
	glUniform1f(iLoc, deformSize);

	iLoc = glGetUniformLocation(prog->shaderProg, "fScreenSize1");
	glUniform1f(iLoc, screenSize1);

	iLoc = glGetUniformLocation(prog->shaderProg, "fScreenSize2");
	glUniform1f(iLoc, screenSize2);

	iLoc = glGetUniformLocation(prog->shaderProg, "fScreenSize3");
	glUniform1f(iLoc, screenSize3);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBufferTextureId);
	
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 1);
		glVertex2f(0, 0);

		glTexCoord2f(0, 0);
		glVertex2f(0, YRES);

		glTexCoord2f(1, 0);
		glVertex2f(XRES, YRES);

		glTexCoord2f(1, 1);
		glVertex2f(XRES, 0);
	}		
	glEnd();
	
	prog->SetActive(false);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	effect::EndRender();
}