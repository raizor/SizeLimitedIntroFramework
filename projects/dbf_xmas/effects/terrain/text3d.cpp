#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <math.h>
#include "text3d.h"
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
#include "../rocket/sync/device.h"
#include <stdio.h>
#endif

#include "intro.h"

#include "framework/utils8k.h"

#include "../../direct3d.h"

extern double rocketRow;
extern ZVector sunPos;

#define FVF_TEXT (D3DFVF_XYZ | D3DFVF_NORMAL)
typedef unsigned int VertexID;
#define NEWLINE_CHAR '^'
#define MAX_STRING_LEN 1000

float xOffset = 0.0f;

text3d::text3d(void)
{
	numVertices = new GLuint[256]; // index buffer positions for chars
	numIndices = new GLuint[256]; // index buffer positions for chars
	glyphMetrics = new LPGLYPHMETRICSFLOAT[256]; // glyph metrics...
	bufferAdjacency = new LPD3DXBUFFER[256]; // adjacency buffers...
	indexData = new GLuint*[256];
	vertexData = new VertexFormatTextMeshDx*[256];
	isValidChar = new bool[256];
	charDimensions= new D3DXVECTOR3[256];

	DWORD installedFonts = 0;
	int fontDataSize = 0;
	//hFont = AddFontMemResourceEx((void*)conflictivo_ttf, conflictivo_ttf_size, 0, &installedFonts);
	//strcpy(fontName, "CONFLICTIVO");

	// generate
	m_base = glGenLists(256);							// Storage For 256 Characters

	int fontHeight = 18;

#ifdef DEBUG
	shaderText = new shader("shaders\\include\\txt.vert",
		NULL,
		"shaders\\include\\txt.frag",
		""
		);
#else
	shaderText = new shader(
		(char*)txt_vert,
		NULL,
		(char*)txt_frag,
		""
		);
#endif


	HFONT font = CreateFontA( fontHeight,							// Height Of Font
		0,								// Width Of Font
		0,								// Angle Of Escapement
		0,								// Orientation Angle
		1,						// Font Weight
		0,							// Italic
		FALSE,							// Underline
		FALSE,							// Strikeout
		ANSI_CHARSET,					// Character Set Identifier
		OUT_DEFAULT_PRECIS,				// Output Precision
		CLIP_DEFAULT_PRECIS,			// Clipping Precision
		ANTIALIASED_QUALITY,			// Output Quality
		//						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
		VARIABLE_PITCH | FF_ROMAN,
		"Trebuchet MS Bold");			// Font Name
	
	// Selects The Font We Created
	
	SelectObject(RES_SETTINGS.hDC, font);
	CreateGlyphMeshes();

	int ti = 0;

	texts[ti++] = "Merry Christmas DBF!^A little Xmas intro^to help celebrate^this wonderful time of year";
	texts[ti++] = "^big shout outs to the forum members^both old and new^...";

	texts[ti++] = ""
		"shockwave, hellfire, rbz, jim^"
		"padman, combatking, kirl, jace^"
		"celtic, Moroboshisan, blacksheep8bit,^"
		"pixel outlaw, rain storm, ninogenio^"
		"hotshot, pot noodle, gore motel,";

	texts[ti++] = "^"
		"xetick, baudsurfer, canopy, zawran^"
		"benny, clyde, optimus, knurz^"
		"bobfresh, spathi, neriakx, nikin";

	texts[ti++] = "ran short of time again so,^lots of stuff still to do^hopefully a 'final' release^in the new year...";
	
	texts[ti++] = "^"
		"well, that's it for now.^"
		"listen to the music...^"
		"as the snow drifts on and on...";
	
	texts[ti++] = "^"
		"enjoy your Christmas!^"
		"and have a wonderful New Year^"
		"lots of love, Eventide";

	for(int i=0; i<ti; i++)
	{
		textObjects[i] = CreateTextObject(texts[i]);
	}
	int xx = 1;
}

void text3d::DrawTextObject(textObj* obj)
{
	float spacingX = 0.1f;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//glTranslatef(-3, 0, -5.0f);

	float offX = (float)sync_get_val(textPosX, rocketRow);
	float offY = (float)sync_get_val(textPosY, rocketRow);
	float offZ = (float)sync_get_val(textPosZ, rocketRow);

	float vec3[3];
	vec3[0] = offX;
	vec3[1] = offY;
	vec3[2] = offZ;
	int iLoc = glGetUniformLocation(shaderText->shaderProg, "offset");

	int lineIndex = 0;
	float lineWidth[10];

	lineWidth[lineIndex] = 0;

	int slen = (int)strlen(obj->txt);
	for(int i=0; i<slen; i++)
	{		
		char charCode = obj->txt[i];

		if (charCode != NEWLINE_CHAR)
		{
			float offsetX = charDimensions[charCode].x+spacingX;	
			if (i<slen-1)
				lineWidth[lineIndex] += offsetX;
		}else{
			lineIndex++;
			lineWidth[lineIndex] = 0;
		}
	}
	float lineSize = 10.0f;

	lineIndex = 0;
	for(int i=0; i<(int)strlen(obj->txt); i++)
	{
		char charCode = obj->txt[i];
		
		if (charCode != NEWLINE_CHAR)
		{
			//if (uniformCharIndex)
			//	glUniform1i(uniformCharIndex->location, fv);

			//fv++;

			if (charCode != 32)
			{
				glBindBuffer(GL_ARRAY_BUFFER, obj->vertexBufferID[charCode]);
				glEnableVertexAttribArray(0);
				glEnableVertexAttribArray(1);
				//glEnableVertexAttribArray(2);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormatTextMeshDx), (GLvoid*)offsetof(VertexFormatTextMeshDx, vertex)); // pos 
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormatTextMeshDx), (GLvoid*)offsetof(VertexFormatTextMeshDx, normal)); // normal
				//glVertexAttribPointer(2, 4, GL_BYTE, GL_FALSE, sizeof(VertexFormatTextMesh), (GLvoid*)offsetof(VertexFormatTextMesh, charNum)); // normal

				int ni = numIndices[charCode];
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->indexBufferID[charCode]);

				float offset[3] = {(vec3[0] + charDimensions[charCode].x*0.5f) - (lineWidth[lineIndex]*0.5f), vec3[1] + charDimensions[charCode].y*0.5f, vec3[2]};
				glUniform3fv(iLoc, 1, offset);
				glDrawElements(GL_TRIANGLES, ni, GL_UNSIGNED_INT, 0);
				//g_pRenderState->PopMatrix();

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				glDisableVertexAttribArray(0);
				glDisableVertexAttribArray(1);
				//glDisableVertexAttribArray(2);
			}

			// todo: fix (hack for sundown)
			if (charCode == ' ')
				charCode = 'E';	

			float offsetX = charDimensions[charCode].x+spacingX;						
			vec3[0] += offsetX;
			//glTranslatef(offsetX, 0.0, 0.0);
			//xOffset += glyphMetrics[charCode][0].gmfCellIncX+spacingX;
		}else{
			// new line, reset X offset
			//g_pRenderState->Translate(-xOffset, 0.0, 0.0);
			vec3[0] = offX;
			vec3[1] -= 1.2f;
			lineIndex++;
			//glTranslatef()
			//g_pRenderState->Translate(0.0, -GetFloatParameterValue(3), 0.0);						
		}
	}

	glPopMatrix();
}

textObj* text3d::CreateTextObject(char* txt)
{
	textObj* obj = new textObj();

	obj->txt = txt;

	obj->stringWidth = 0.0f;
	obj->stringHeight = 0.0f;
	obj->stringDepth = 0.2f;

	int centerX = 0;

	obj->indexBufferID = new GLuint[MAX_STRING_LEN];
	obj->vertexBufferID = new GLuint[MAX_STRING_LEN];

	for (int i=0; i<MAX_STRING_LEN; i++)
	{
		obj->indexBufferID[i] = 0;
		obj->vertexBufferID[i] = 0;
	}

	for(unsigned int i=0; i<strlen(txt); i++)
	{
		char charCode = txt[i];

		if (isValidChar[charCode])
		{		
			if (obj->vertexBufferID[charCode]==0)
				glGenBuffers(1, &obj->vertexBufferID[charCode]);

			if (obj->indexBufferID[charCode]==0)
				glGenBuffers(1, &obj->indexBufferID[charCode]);

			glBindBuffer(GL_ARRAY_BUFFER, obj->vertexBufferID[charCode]);

			glBufferData(GL_ARRAY_BUFFER, numVertices[charCode]*sizeof(VertexFormatTextMeshDx), vertexData[charCode], GL_STATIC_DRAW);		

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// upload indices
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->indexBufferID[charCode]);	

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices[charCode]*sizeof(VertexID), indexData[charCode], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}

	return obj;
}

void text3d::CreateGlyphMeshes()
{
	HRESULT res;
	/*
	for(int i=0; i<256; i++)
	{
		if (glyphMetrics[i] != NULL)
			delete [] glyphMetrics[i];

		if (bufferAdjacency[i] != NULL)
			bufferAdjacency[i]->Release();
	}
	*/
	bool createdBuffers = false;
		/*
	// delete temp buffers
	for(int i=0; i<256; i++)
	{
		if (indexData[i] != NULL)
		{
			delete indexData[i];
			delete vertexData[i];
			indexData[i] = NULL;
			vertexData[i] = NULL;
		}
	}
	*/
	for(int i=0; i<256; i++)
	{
		int numAllocatedVertices;
		int numAllocatedVertexIndices;

		indexData[i] = 0;
		vertexData[i] = 0;

		LPD3DXMESH dxMesh = NULL;

		glyphMetrics[i] = new GLYPHMETRICSFLOAT[1];
		
		char charVal[2];
		charVal[1] = NULL;

		/*
		if (i<33 || i> 126)
		{
			isValidChar[i] = false;
			continue;
		}*/

		charVal[0] = i;

		float depth = 0.4f;

		D3DXCreateTextA(d3dDevice, RES_SETTINGS.hDC, charVal, 0.001f, depth, &dxMesh, &bufferAdjacency[i], glyphMetrics[i]);

		numVertices[i] = 0;
		numIndices[i] = 0;

		isValidChar[i] = dxMesh != NULL;

		if (isValidChar[i])
		{
			// create output mesh with raw points (FVF)
			LPD3DXMESH tmesh;
			dxMesh->CloneMeshFVF(D3DXMESH_MANAGED | D3DXMESH_32BIT, FVF_TEXT, d3dDevice, &tmesh); // no normals
			dxMesh->Release();
			dxMesh=tmesh;
			
		#ifdef DEBUG
			//ZASSERT( !FAILED( res ) );
		#endif

			numAllocatedVertices = dxMesh->GetNumVertices();
			numAllocatedVertexIndices = dxMesh->GetNumFaces()*3;
	
			numIndices[i] = numAllocatedVertexIndices;
			numVertices[i] = numAllocatedVertices;

			LPDIRECT3DVERTEXBUFFER9 vb;
			LPDIRECT3DINDEXBUFFER9 ib;
			VertexFormatTextMeshDx* dxMeshBuffer;
			unsigned int* dataManIndices;
				
			dxMesh->GetVertexBuffer(&vb);
			dxMesh->GetIndexBuffer(&ib);

			DWORD fvf = dxMesh->GetFVF();
 			DWORD ops = dxMesh->GetOptions();

			bool is32 = (ops || D3DXMESH_32BIT) == D3DXMESH_32BIT;

			// indices
		
			res = ib->Lock(0, 0, (void**)&dataManIndices, 0);
			
			// upload index data
						
			indexData[i] = new VertexID[numAllocatedVertexIndices];
			memcpy(indexData[i], dataManIndices, sizeof(VertexID) * numAllocatedVertexIndices);
						
			ib->Unlock();
			ib->Release();
	
			// vertices

			res = vb->Lock(0, 0, (void**)&dxMeshBuffer, 0);

			float minVals [] = {10000.0f, 10000.0f, 10000.0f};
			float maxVals [] = {-10000.0f, -10000.0f, -10000.0f};

			for(int j=0; j<numAllocatedVertices; j++)
			{
				// min
				if (dxMeshBuffer[j].vertex.x < minVals[0])
					minVals[0] = dxMeshBuffer[j].vertex.x;

				if (dxMeshBuffer[j].vertex.y < minVals[1])
					minVals[1] = dxMeshBuffer[j].vertex.y;

				if (dxMeshBuffer[j].vertex.z < minVals[2])
					minVals[2] = dxMeshBuffer[j].vertex.z;

				// max
				if (dxMeshBuffer[j].vertex.x > maxVals[0])
					maxVals[0] = dxMeshBuffer[j].vertex.x;

				if (dxMeshBuffer[j].vertex.y > maxVals[1])
					maxVals[1] = dxMeshBuffer[j].vertex.y;

				if (dxMeshBuffer[j].vertex.z > maxVals[2])
					maxVals[2] = dxMeshBuffer[j].vertex.z;

			}				

			float charWidth = maxVals[0] - minVals[0];
			float charHeight = maxVals[1] - minVals[1];
			float charDepth = maxVals[2] - minVals[2];

			charDimensions[i] = D3DXVECTOR3(charWidth, charHeight, charDepth);
			
			// fix up the vertices
			for(int j=0; j<numAllocatedVertices; j++)
			{
				dxMeshBuffer[j].vertex.x-=charWidth*0.5f;
				dxMeshBuffer[j].vertex.y-=charHeight*0.5f;				
				dxMeshBuffer[j].vertex.z+=charDepth*0.5f;
			}

			vertexData[i] = new VertexFormatTextMeshDx[numAllocatedVertices];
			memcpy(vertexData[i], dxMeshBuffer, sizeof(VertexFormatTextMeshDx) * numAllocatedVertices);
			vb->Unlock();
			vb->Release();
			
			// free the DX mesh, we don't need it anymore
			dxMesh->Release();		
		}		
	}
}


text3d::~text3d(void)
{
}
