#pragma once

#include <stdint.h> 

// TODO: For GNU
//#define PACK_STRUCT( __Declaration__ ) __Declaration__ __attribute__((__packed__))
// For MSVC
#define PACK_STRUCT( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )

enum
{
	kOrderCounterClockwise,
	kOrderClockwise,
};

enum
{
	kContourTypeOuter,
	kContourTypeInner,
};

enum
{
	kPointTypeOn = 1,
	kPointTypeOffConic = 2,
	kPointTypeOffCubic = 4,
};

typedef unsigned short refonter_count;
typedef unsigned short refonter_coord;
typedef unsigned char  refonter_point_info;
typedef unsigned char  refonter_char_info;
typedef unsigned char  refonter_contour_info;
typedef unsigned short refonter_font_info;
typedef char           refonter_char_type;

PACK_STRUCT(
struct refonter_point
{
	refonter_coord      x;
	refonter_coord      y;
	refonter_point_info flags;
});

PACK_STRUCT(
struct refonter_contour
{
	refonter_contour_info flags;
	refonter_count        num_points;
	refonter_point*       points;
});

PACK_STRUCT(
struct refonter_char
{
	refonter_char_type id;
	refonter_char_info flags;
	refonter_coord     width;
	refonter_count     num_contours;
	refonter_contour*  contours;
});

PACK_STRUCT(
struct refonter_font
{
	refonter_font_info  flags;
	refonter_count      num_chars;
	refonter_char*      chars;
});

refonter_font* refonter_init_blob(unsigned char* blob);

#include "refonter_vertex.h"

double refonter_bezier(double t, double start, double control1, double control2, double end);
refonter_vec3 refonter_bezier(double t, const refonter_vec3& start, const refonter_vec3& control1, const refonter_vec3& control2, const refonter_vec3& end);
refonter_vec3 refonter_quadratic_control_to_cubic(const refonter_vec3& p0, const refonter_vec3& p1);

#include "refonter_tesselator.h"


/*
class Font
{
public:
Font(unsigned char* data)
: data(data)
{
font = refonter_init_blob(data);
tesselation = refonter_tesselate(font);

for (uint32_t i=0; i<font->num_chars; i++)
{
GLuint id;
glGenBuffers(1, &id);
vertexBufferIDs.push_back(id);

glBindBuffer(GL_ARRAY_BUFFER, id);
glBufferData(GL_ARRAY_BUFFER, tesselation[i].num_triangle_vertices*sizeof(refonter_vertex), tesselation[i].triangles, GL_STATIC_DRAW);
glBindBuffer(GL_ARRAY_BUFFER, 0);
}

}

unsigned char* data;
refonter_font* font;
refonter_tesselation_object* tesselation;

void DrawLetter(uint32_t i)
{
glBindBuffer(GL_ARRAY_BUFFER, vertexBufferIDs[i]);

glEnableVertexAttribArray(0);
glEnableVertexAttribArray(1);
glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(refonter_vertex), (GLvoid*)0);
glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, sizeof(refonter_vertex), (GLvoid*)(3*sizeof(double)));

//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);

//glDrawElements(GL_TRIANGLES, tesselation->, GL_UNSIGNED_INT, 0);
glDrawArrays(GL_TRIANGLES, 0, tesselation[i].num_triangle_vertices);

glDisableVertexAttribArray(0);
glDisableVertexAttribArray(1);
glBindBuffer(GL_ARRAY_BUFFER,         0);

//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Write(const char* str, ZRenderer& renderer, ZMaterial& mat)
{
while (*str)
{
char c = *str;

for (uint32_t i=0; i<font->num_chars; i++)
{
if (font->chars[i].id==c)
{
//glCallList(tesselation[i].glID);
DrawLetter(i);

renderer.mm() = renderer.mm().Translate(ZVector(float(font->chars[i].width)/64.f, 0.f, 0.f));
mat.program->SetCommonLocations(renderer);
}
}

str++;
}
}
private:

zvector<GLuint> vertexBufferIDs;
};
*/