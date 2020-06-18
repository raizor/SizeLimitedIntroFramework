#include "fontBitmap.h"
#include "stb_image.h"
#include "resource.h"
#include "res/font_harmonica.h"

////////////////////////////////////////////////////////////////////
// Allocate memory and load targa bits. Returns pointer to new buffer,
// height, and width of texture, and the OpenGL format of data.
// Call free() on buffer when finished!
// This only works on pretty vanilla targas... 8, 24, or 32 bit color
// only, no palettes, no RLE encoding.
GLbyte* fontBitmap::gltLoadPNG(int resourceId, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat)
{
	//stbi_load_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp)';
	int req_comp = 4;
	pixels = (GLbyte*)stbi_load_from_memory((unsigned char*)res_font_harmonica_png, res_font_harmonica_png_size, iWidth, iHeight, iComponents, req_comp);
	return pixels;
}

fontBitmap::fontBitmap()
{
	// create font image
	//shader takes string of hex chars 
	GLint width, height, icomps;
	GLenum eFormat;
	GLbyte *pixels = gltLoadPNG(IDR_PNG_LOGO, &width, &height, &icomps, &eFormat);
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, icomps, width, height, 0, eFormat, GL_UNSIGNED_BYTE, pixels);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Linear Filtering
}

// get color of pixel
color4f fontBitmap::getBlockColor(int x, int y)
{
	color4f col;

	GLubyte cr = pixels[(x * 4) + (y * 256 * 4)];
	GLubyte cg = pixels[(x * 4) + (y * 256 * 4) + 1];
	GLubyte cb = pixels[(x * 4) + (y * 256 * 4) + 2];
	GLubyte ca = pixels[(x * 4) + (y * 256 * 4) + 3];

	col.r = cr / 255.0f;
	col.g = cg / 255.0f;
	col.b = cb / 255.0f;
	col.a = ca / 255.0f;

	return col;
}

fontBitmap::~fontBitmap()
{
}

// ***********************************************************************

void fontBitmapText::activate()
{
	
}

void fontBitmapText::deactivate() 
{
	
}

fontBitmapText::fontBitmapText(fontBitmap* font, char* text)
{
	this->strText = new char[strlen(text)+1];
	this->font = font;
	memcpy(strText, text, strlen(text) + 1);

	float offsetY = 0;
	float startY = 0;
	float offsetX = 0;

	int offset = 0;
	int charSize = 9;
	int charsPerLine = 16;
	int charOffx = 0;

	float cubeSize = 2.0f / 256.0f;

	pixels = new vec2[512];

	numPixels = 0;	

	for (int i = 0; i<(int)strlen(this->strText); i++)
	{
		char ch = this->strText[i];
		int line = (ch - 32) / charsPerLine;
		int lineChar = (ch - 32) % charsPerLine;

		int offPix = 0;

		if (ch == ' ')
		{
			charOffx += charSize;
			continue;
		}

		for (int x = 0; x<charSize; x++) // each col
		{
			bool hitPixel = false;

			for (int y = 0; y<charSize; y++) // each row
			{
				int yp = (line*charSize) + line + y + 1;
				int xp = (lineChar*charSize) + lineChar + x;
				color4f color = font->getBlockColor(xp, yp);
				if (color.a > 0)
				{
					for (int level = 0; level<1; level++)
					{
						pixels[numPixels].x = ((float)(offPix + charOffx + offsetX));
						pixels[numPixels++].y = 1.0f - (((float)y - startY + offsetY)) + (charSize/2);
						hitPixel = true;
					}
				}
			}

			if (hitPixel)
			{
				offPix++;
			}
		}
		charOffx += offPix + 1;
	}
}

void fontBitmapText::drawCullCube(ZVector size, ZVector pos)
{
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 6 * sizeof(float), cubeVerts + 0);
	glNormalPointer(GL_FLOAT, 6 * sizeof(float), cubeVerts + 3);

	glPushMatrix();
	{
		glTranslatef(pos.x, pos.y, pos.z);
		glScalef(size.x, size.y, size.z);
		glDrawElements(GL_QUADS, 6 * 4, GL_UNSIGNED_INT, cubeIndices);
	}
	glPopMatrix();

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void fontBitmapText::drawCullingObjects()
{
	for (int i = 0; i < numPixels; i++)
	{
		vec2 pixel = pixels[i];
		drawCullCube(ZVector(1.0f, 1.0f, 1.0f).Mul(10.0f), ZVector(pixel.x, pixel.y, 0.0f).Mul(20.0f));
	}
}

/*
render culling objects for the text pixels to an fbo and return the colorbuffer id 
todo: return a depth buffer too :)
*/
GLuint fontBitmapText::getCullingBuffer(){
	GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT };
	glClearColor(0, 0, 0, 0);

	glColor4f(1.0, 1.0, 1.0, 0.2f);
	glDisable(GL_DEPTH_TEST);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, global::GetState()->fboCulling->id);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glDrawBuffers(1, &buffers[0]);
	
	drawCullingObjects();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	return global::GetState()->fboCulling->colorBufferID[0];
}

fontBitmapText::~fontBitmapText()
{
	
}
