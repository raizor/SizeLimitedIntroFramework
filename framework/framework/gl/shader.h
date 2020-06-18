#pragma once

#include <windows.h>
#include "gl8k.h"
#ifdef DEBUG
	#include "../debug/watchfile.h"
#endif

class shader
{
public:
	char definesString[5000];
	char *strVs, *strFs, *strGs;
	GLint shaderProg;
	shader( char* vs,  char* gs,  char* fs,  char* defines, char* shaderName = NULL);
	void SetActive(bool active);
	#ifdef DEBUG
		bool reload();
		void refresh();
	#endif

private:
	GLuint shaderVertex;
	GLuint shaderGeometry;
	GLuint shaderFragment;

#ifdef DEBUG
	bool haveGeomShader;
	const char* vsTxtPath;
	const char* gsTxtPath;
	const char* fsTxtPath;
	watchfile* watchFileVertex;
	watchfile* watchFileGeometry;
	watchfile* watchFileFragment;

#endif
};

