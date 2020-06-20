#pragma once

#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

static const unsigned int kMaxTesselatorVertices = 8*1024;

struct refonter_tesselation_object
{
	refonter_vertex storage[kMaxTesselatorVertices];
	refonter_vertex triangles[kMaxTesselatorVertices];

	unsigned int num_storage;
	unsigned int num_triangle_vertices;

	uint32_t glID;

	GLUtesselator* glu_tess_obj;

	refonter_vertex history[2];
	uint32_t type;
	uint32_t vcount;
};

refonter_tesselation_object* refonter_tesselate(refonter_font* p_font);
