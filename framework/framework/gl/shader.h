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
	const char *strVs, *strFs, *strGs;
	GLint shaderProg;
	shader( const char* vs, const char* gs, const char* fs, const char* defines, const char* shaderName);
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

