#pragma once
#include "..\..\config.h"
#include "framework\gl\gl8k.h"
#include "framework\gl\shader.h"
#include <d3dx9mesh.h>

struct VertexFormatTextMeshDx // DX buffer format
{
	D3DXVECTOR3 vertex;
	D3DXVECTOR3 normal;
};

class textObj
{
public:
	char* txt;
	GLuint* indexBufferID;
	GLuint* vertexBufferID;
	bool* isNewline;
	float stringWidth;
	float stringHeight;
	float stringDepth;
};

class text3d
{
public:

	HANDLE hFont;
	char fontName[255];
	char* texts[100];
	shader* shaderText;
	textObj* textObjects[100];

	bool* isValidChar;
	bool Updated;

	GLuint* numIndices;
	GLuint* numVertices;
	D3DXVECTOR3* charDimensions;

	LPGLYPHMETRICSFLOAT* glyphMetrics;
	LPD3DXBUFFER* bufferAdjacency;

	GLuint** indexData;
	VertexFormatTextMeshDx** vertexData;

	text3d(void);
	~text3d(void);
	void CreateGlyphMeshes();
	textObj* CreateTextObject(char* txt);
	void DrawTextObject(textObj* obj);

private:
	GLYPHMETRICSFLOAT m_gmf[256];	// Storage For Information About Our Outline Font Characters
	GLuint	m_base;				// Base Display List For The Font Set
};

