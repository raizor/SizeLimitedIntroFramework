#pragma once
#include "..\..\config.h"
#include "framework\utils8k.h"
#include "framework\gl\gl8k.h"
#include "framework\math\matrix.h"

#include "introTypes.h"

struct imageBuffer
{
	char* pixelBuffer;
	unsigned long fileLen;
};

class image
{
public:
	int width;
	int height;
	int depthFront;
	int depthBack;
	int maxDepth;
	int comp, reqComp;
	pixel* pixels;
	pixel* pixelsDepth;
	
	image(const char* filename = 0, const char* filenameDepth = 0);
	imageBuffer image::loadFile(const char* filename);
	pixel* image::getPixels(char* buffer);
};