#include "logo.h"
#include "font.h"
#include "framework\gl\gl8k.h"
#include "framework\gl\shader.h"
#include "refonter\refonter_tesselator.h"

logo::logo(void)
{
	refonterFont = refonter_init_blob(font);
	tesselatedFont = refonter_tesselate(refonterFont);

#ifdef DEBUG
	shaderText = new shader(
		"shaders\\include\\text.vs.glsl",
		NULL,
		"shaders\\include\\text.fs.glsl"
		);
#else
	shaderText = new shader(
		&spinner_vs_glsl,
		&spinner_gs_glsl,
		&spinner_fs_glsl,
		);
#endif

	
	
	makeLogo();
}

void logo::makeLogo()
{
	
	glGenBuffers(1, &vertexBufferId);
	glGenBuffers(1, &indexBufferId);

	char* txt = "e";
	int totalVerts = 0;

	for(unsigned int i=0; i<strlen(txt); i++)
	{
		char c = txt[i] - 0x61;
		totalVerts+=tesselatedFont[c].num_triangle_vertices;
	}

	refonter_vertex* verts = new refonter_vertex[totalVerts];
	

	int vertexIndex = 0;
	float offsetX = 0.0;

	refonter_vertex* vp = verts;

	for(unsigned int i=0; i<strlen(txt); i++)
	{
		char c = txt[i] - 0x61;
		memcpy(vp, tesselatedFont[c].triangles, sizeof(refonter_vertex) * tesselatedFont[c].num_triangle_vertices);
		for(unsigned int j=0; j<tesselatedFont[c].num_triangle_vertices; j++) 
		{
			//vp[j].pos.x += offsetX;
			//vp[j].pos.y += offsetX;
		}
		vertexIndex+=tesselatedFont[c].num_triangle_vertices;
		vp+=tesselatedFont[c].num_triangle_vertices * sizeof(refonter_vertex);
		offsetX+=1.1f;
	}
	
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, totalVerts*sizeof(refonter_vertex), verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void logo::draw(float itime)
{
	glTranslatef(0, 0, -itime);

	shaderText->SetActive(1);

	float matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	GLuint iLoc = glGetUniformLocation(shaderText->shaderProg, "modelViewMatrix");
	glUniformMatrix4fv(iLoc, 1, false, matrix);

	glGetFloatv(GL_PROJECTION_MATRIX, matrix);
	iLoc = glGetUniformLocation(shaderText->shaderProg, "projectionMatrix");
	glUniformMatrix4fv(iLoc, 1, false, matrix);

	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	iLoc = glGetUniformLocation(shaderText->shaderProg, "viewMatrix");
	glUniformMatrix4fv(iLoc, 1, false, matrix);


	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(refonter_vertex), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, sizeof(refonter_vertex), (GLvoid*)(3*sizeof(double)));

	glDrawArrays(GL_TRIANGLES, 0, tesselatedFont[0].num_triangle_vertices);
	//glDrawArrays(GL_LINES, 0, tesselatedFont[0].num_triangle_vertices);
	
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	shaderText->SetActive(0);
}

logo::~logo(void)
{
}
