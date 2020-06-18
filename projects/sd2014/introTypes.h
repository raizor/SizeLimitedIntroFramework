#pragma once

#define u8 unsigned char
#define s8 char
#define u16 unsigned short
#define s16 short
#define u32 unsigned int
#define s32 int

struct v2
{
	float x, y;
};

struct u16_3
{
	u16 x, y, z;
};

struct v3
{
	float x, y, z;
};

struct v4
{
	float x, y, z, w;
};

struct i4
{
	int x, y, z, w;
};

struct pixel
{
	u8 r, g, b, a;

	v4 col()
	{
		return v4{ (float)r / 255, (float)g / 255, (float)b / 255, (float)a / 255 };
	}
};

struct cubeProps
{
	v3 pos;
	v4 col;
	u32 id;
};

struct vertex
{
	v3 pos;
	v3 norm;
	//v3 gridPos;
	//v4 col;

	vertex::vertex()
	{
	}

	vertex::vertex(v3 pos, v3 norm, v3 gridPos, v4 col)
	{
		this->pos = pos;
		this->norm = norm;
		//this->gridPos = gridPos;
		//this->col = col;
	}
};