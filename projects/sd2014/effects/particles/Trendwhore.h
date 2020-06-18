#ifndef __EFFECTS_TRENDWHORE_H__
#define __EFFECTS_TRENDWHORE_H__

#include "..\..\config.h"
#include "framework\utils8k.h"
#include "framework\gl\gl8k.h"
#include "framework/gl/shader.h"
#include "framework\math\matrix.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include "framework/gl/fbo.h"
//#include "../Effect.h"

class Trendwhore
{
public:
	Trendwhore();
	~Trendwhore();
    void Trendwhore::draw_cube( void );
	void Run(double time);

private:
	class Loop
	{
	public:
		class Cube
		{
		public:
			float Angle;
			float RadiusOffset;
			float HeightOffset;
			glm::vec3 Scale;
		};

		glm::vec3 Pos;
		glm::vec3 Rot;
		glm::vec3 Scale;
		float Radius;
		float RotSpeed;

		Cube* Cubes[300];
	};

	shader *prog;

	Loop* loops[3];
};

#endif
