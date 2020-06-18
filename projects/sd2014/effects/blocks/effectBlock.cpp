#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <math.h>
#include "effectBlock.h"
#include "framework/gl/gl8k.h"
#include "framework/gl/shader.h"
#include <GL/glu.h>


#include "clinkster.h"
#include "../../shader_code_intro.h"
#include "config.h"
#include "rocket/rocketControl.h"
#include "music/syncData.h"
#include "framework/gl/fbo.h"
#include "framework\math\matrix.h"

#ifndef SYNC_PLAYER	
#include "rocket/sync/device.h"
#include <stdio.h>
#endif

#include "framework/utils8k.h"

#include "blockGrid.h"

/*
instance attributes:
pos, color, rot
*/

effectBlock::effectBlock()
{
	GLenum eFormat;
	GLint iComponents;

	eFormat = GL_RGBA;
	iComponents = GL_RGBA8;

	image* img = new image("assets/tex16.png", "");
	glGenTextures(1, &textureNoiseId);
	glBindTexture(GL_TEXTURE_2D, textureNoiseId);
	glTexImage2D(GL_TEXTURE_2D, 0, img->comp, img->width, img->height, 0, eFormat, GL_UNSIGNED_BYTE, img->pixels);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Linear Filtering

#ifdef DEBUG

	shaderVoxels = new shader(
		"shaders\\include\\voxels.vert",
		NULL,/*"shaders\\include\\voxels.geom",*/
		"shaders\\include\\voxels.frag",
		"",
		"vox"
		);

	shaderNoise = new shader(
		"shaders\\include\\noise.vert",
		NULL,/*"shaders\\include\\voxels.geom",*/
		"shaders\\include\\noise.frag",
		"",
		"noise"
		);

	shaderInitVox = new shader(
		"shaders\\include\\initVox.vert",
		NULL,/*"shaders\\include\\voxels.geom",*/
		"shaders\\include\\initVox.frag",
		"",
		"voxInit"
		);

	shaderVoxPos = new shader(
		"shaders\\include\\voxelsPos.vert",
		NULL,/*"shaders\\include\\voxels.geom",*/
		"shaders\\include\\voxelsPos.frag",
		"",
		"voxPos"
		);

	shaderVoxVel = new shader(
		"shaders\\include\\voxelsVel.vert",
		NULL,/*"shaders\\include\\voxels.geom",*/
		"shaders\\include\\voxelsVel.frag",
		"",
		"voxVel"
		);

	

#else

	shaderVoxels = new shader(
		(char*)terrainTest_vert,
		NULL,
		(char*)terrainTest_frag,
		""
		);

	shaderTerrainLogo = new shader(
		(char*)terrainTest_vert,
		NULL,
		(char*)terrainTest_frag,
		"#define LOGO\n"
		);

	shaderTerrainSnowman = new shader(
		(char*)terrainTest_vert,
		NULL,
		(char*)terrainTest_frag,
		"#define SNOWMAN\n"
		);
#endif

	//glGenBuffers(1, &vertexBufferCubeID);
	//glGenBuffers(1, &indexBufferCubeID);

	//grid = new blockGrid(shaderInitVox, textureNoiseId, 1, 1,
	//	"assets\\coin_color.png",
	//	"assets\\coin_height.png");
	parts = new particles(shaderInitVox);
	scene = new blockScene();
	//

}

int texNum = 0;

void effectBlock::testNoise(float time)
{
	shaderNoise->refresh();

	// reset the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// ortho view
	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//glOrtho( 0, XRES , YRES , 0, 0, 1 );             // Select Ortho Mode (ie.640x480)
	glColor4f(1, 1, 1, 1);

	shaderNoise->SetActive(true);

	GLuint texHandle = Noise3D::GetHandle(texNum);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, texHandle);

	int iLoc = glGetUniformLocation(shaderNoise->shaderProg, "texOldPos");
	glUniform1i(iLoc, 0);

	iLoc = glGetUniformLocation(shaderNoise->shaderProg, "texNoise");
	glUniform1i(iLoc, 1);

	// draw fbo to screen
	glRects(-1, -1, 1, 1);
	shaderNoise->SetActive(false);

	glBindTexture(GL_TEXTURE_3D, 0);
}

void effectBlock::draw(float time)
{

	//glRects(-1, -1, 1, 1); 

	//GLuint texHandle = Noise3D::GetHandle(texNum);

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_3D, texHandle);

	//grid->draw(shaderVoxels, shaderVoxPos, shaderVoxVel, time);
	//testNoise(time);

	// render the scene
	scene->draw(time);
	parts->draw(shaderVoxels, shaderVoxPos, shaderVoxVel, scene->fboScene, time);
	// particalize!
}