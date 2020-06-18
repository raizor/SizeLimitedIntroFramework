#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <math.h>
#include "framework/gl/gl8k.h"
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
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

#include "image.h"

#include "introTypes.h"

#include "stb_image.h"

imageBuffer image::loadFile(const char* filename)
{
	imageBuffer ib;
	ib.fileLen = 0;
	FILE *file;

	//Open file
	file = fopen(filename, "rb");
	if (!file)
	{
		fprintf(stderr, "Unable to open file %s", filename);
		return ib;
	}

	//Get file length
	fseek(file, 0, SEEK_END);
	ib.fileLen = ftell(file);
	fseek(file, 0, SEEK_SET);

	//Allocate memory
	ib.pixelBuffer = (char *)malloc(ib.fileLen + 1);
	if (!ib.pixelBuffer)
	{
		fprintf(stderr, "Memory error!");
		fclose(file);
		return ib;
	}

	//Read file contents into buffer
	fread(ib.pixelBuffer, ib.fileLen, 1, file);
	fclose(file);
	ib.pixelBuffer = ib.pixelBuffer;
	ib.fileLen = ib.fileLen;
	return ib;
}

image::image(const char* filenameColor, const char* filenameHeight)
{
	imageBuffer bufferColor = loadFile(filenameColor);
		
	int x, y;
	reqComp = 4;
	pixel* pixelData = (pixel*)stbi_load_from_memory((u8*)bufferColor.pixelBuffer, bufferColor.fileLen, &x, &y, &comp, reqComp);

	this->width = x;
	this->height = y;

	// flip Y
	pixel* pix = new pixel[width*height];	
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			pixel* src = &pixelData[x + (y*width)];
			pixel* dest = &pix[x + ((height - 1 - y)*width)];
			*dest = *src;
		}
	}
	free(pixelData);
	//free(pix);
	this->pixels = pix;
	
	if (strlen(filenameHeight) != 0)
	{
		imageBuffer bufferHeight = loadFile(filenameHeight);
		pixelData = (pixel*)stbi_load_from_memory((u8*)bufferHeight.pixelBuffer, bufferHeight.fileLen, &x, &y, &comp, reqComp);

		// flip Y
		pixel* dPix = new pixel[width*height];
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				pixel* src = &pixelData[x + (y*width)];
				pixel* dest = &dPix[x + ((height - 1 - y)*width)];
				*dest = *src;
				if (dest->r > maxDepth)
				{
					maxDepth = dest->r;
				}
			}
		}
		free(pixelData);
		//free(pix);
		this->pixelsDepth = dPix;
		this->depthFront = 2;
		this->depthBack =2;
	}
}
