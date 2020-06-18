#include "Trendwhore.h"
#include "Farticles.h"

Trendwhore::Trendwhore()
{
	/*
	vert = new Shader("Data/Trendwhore.vert", GL_VERTEX_SHADER);
	frag = new Shader("Data/Trendwhore.frag", GL_FRAGMENT_SHADER);
	prog = new Program(vert, frag);
	*/
	prog = new shader("effects/particles/trendwhore.vert", NULL, "effects/particles/trendwhore.frag", "", "TH");

	//Random::Seed(6);
	int seed = 6;

	for (int i = 0; i < 3; i++)
	{
		Loop* l = new Loop;

		msys_frand(&seed);

        l->Pos = glm::vec3(msys_randFloat(&seed, -3.0f, 3.0f), msys_randFloat(&seed,-3.0f, 3.0f), msys_randFloat(&seed,-3.0f, 3.0f));
		l->Rot = glm::vec3(msys_randFloat(&seed, -50.0f, 50.0f), msys_randFloat(&seed,0.0f, 360.0f), msys_randFloat(&seed,-50.0f, 50.0f));
		l->Scale = glm::vec3(msys_randFloat(&seed, .8f, 1.2f), 1.0f, msys_randFloat(&seed,.8f, 1.2f));
		l->Radius = msys_randFloat(&seed,5.0f, 7.0f);
		l->RotSpeed = msys_randFloat(&seed,4.0f, 30.0f);
		for (int j = 0; j < 300; j++)
		{
			Loop::Cube* c = new Loop::Cube();

			c->Angle = msys_randFloat(&seed, 0.0f, 360.0f);
			c->RadiusOffset = msys_randFloat(&seed, -.6f, .6f);
			c->HeightOffset = msys_randFloat(&seed, -.6f, .6f);
			c->Scale = glm::vec3(1.1f, .2f, .2f);

			l->Cubes[j] = c;
		}



		loops[i] = l;
	}
}

Trendwhore::~Trendwhore()
{
	/*
	delete prog;
	*/
}


static const float cubeVerts[] = { 

	1.0f, -1.0f, -1.0f,    0.0f,-1.0f, 0.0f,
	1.0f, -1.0f,  1.0f,    0.0f,-1.0f, 0.0f,
	-1.0f, -1.0f,  1.0f,    0.0f,-1.0f, 0.0f,
	-1.0f, -1.0f, -1.0f,    0.0f,-1.0f, 0.0f,

	-1.0f,  1.0f, -1.0f,    0.0f, 1.0f, 0.0f,
	-1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 0.0f,
	1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 0.0f,
	1.0f,  1.0f, -1.0f,    0.0f, 1.0f, 0.0f,

	-1.0f, -1.0f, -1.0f,   -1.0f, 0.0f, 0.0f,
	-1.0f, -1.0f,  1.0f,   -1.0f, 0.0f, 0.0f,
	-1.0f,  1.0f,  1.0f,   -1.0f, 0.0f, 0.0f,
	-1.0f,  1.0f, -1.0f,   -1.0f, 0.0f, 0.0f,

	1.0f,  1.0f, -1.0f,    1.0f, 0.0f, 0.0f,
	1.0f,  1.0f,  1.0f,    1.0f, 0.0f, 0.0f,
	1.0f, -1.0f,  1.0f,    1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, -1.0f,    1.0f, 0.0f, 0.0f,

	-1.0f, -1.0f, -1.0f,    0.0f, 0.0f,-1.0f,
	-1.0f,  1.0f, -1.0f,    0.0f, 0.0f,-1.0f,
	1.0f,  1.0f, -1.0f,    0.0f, 0.0f,-1.0f,
	1.0f, -1.0f, -1.0f,    0.0f, 0.0f,-1.0f,

	1.0f, -1.0f,  1.0f,    0.0f, 0.0f, 1.0f,
	1.0f,  1.0f,  1.0f,    0.0f, 0.0f, 1.0f,
	-1.0f,  1.0f,  1.0f,    0.0f, 0.0f, 1.0f,
	-1.0f, -1.0f,  1.0f,    0.0f, 0.0f, 1.0f };

	static const int cubeIndices[] = { 
		0, 1, 2, 3,
		4, 5, 6, 7,
		8, 9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23 };

void Trendwhore::draw_cube( void ) {
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 6*sizeof(float), cubeVerts+0);
	glNormalPointer(    GL_FLOAT, 6*sizeof(float), cubeVerts+3);
	glDrawElements( GL_QUADS, 6*4, GL_UNSIGNED_INT, cubeIndices );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
}

void Trendwhore::Run(double time)
{
	//CheckGlError(true);
	GLuint iv = Farticles::GetFboHandle()->id;
	//CheckGlError(true);
	glBindFramebufferEXT(GL_FRAMEBUFFER, iv);

    GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT };
    glDrawBuffers(2, &buffers[0]);

	//CheckGlError(false);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//CheckGlError(false);
	glm::mat4 proj = glm::perspective(45.0f, 16.0f / 9.0f, 1.0f, 1000.0f);
	glm::mat4 view;
	//view = translate(view, glm::vec3(0.0f, 0.0f, Sync::GetTrackValue("Camera Distance")));
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));

	//switch ((int) Sync::GetTrackValue("Trendwhore Camera"))
	switch ((int) 0)
	{
	case 0:
		view = glm::rotate(view, (float) sin((float) time * .7f * .5f) * 20.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, sin((float) time * .47f * .5f) * 20.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		view = glm::rotate(view, (float) time * 6.0f * .5f, glm::vec3(0.0f, 1.0f, 0.0f));
		break;

	case 1:
		//view = TrackballCamera::GetView();
		break;
	}
	//CheckGlError(false);
	glEnable(GL_DEPTH_TEST);
	prog->SetActive(true);
    {
		glUniform3f(glGetUniformLocation(prog->shaderProg, "LightDir"), 1.0f, .5f, 1.0f);
        //CheckGlError(true); 

        for (int i = 0; i < 3; i++)
        {
            Loop *l = loops[i];

            glm::mat4 loopWorld;
            loopWorld = glm::rotate(loopWorld, l->Rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
            loopWorld = glm::rotate(loopWorld, l->Rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
            loopWorld = glm::rotate(loopWorld, l->Rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
            loopWorld = glm::translate(loopWorld, l->Pos);
            loopWorld = glm::scale(loopWorld, l->Scale);
            for (int j = 0; j < 30; j++)
            {
                Loop::Cube *c = l->Cubes[j];

                glm::mat4 world = loopWorld;
                world = glm::rotate(world, c->Angle + l->RotSpeed * (float) time, glm::vec3(0.0f, 1.0f, 0.0f));
                world = glm::translate(world, glm::vec3(0.0f, c->HeightOffset, l->Radius + c->RadiusOffset));
                glm::mat4 viewWorld = view * world;
                glm::mat4 projViewWorld = proj * viewWorld;

                GLint add = glGetUniformLocation(prog->shaderProg, "ProjViewWorld");
                glUniformMatrix4fv(add, 1, false, (float *) &projViewWorld[0]);

				add = glGetUniformLocation(prog->shaderProg, "World");
                glUniformMatrix4fv(add, 1, false, (float *) &world[0]);

				glUniform3f(glGetUniformLocation(prog->shaderProg, "Color"), 1.0f, 1.0f, 1.0f);
                //glColor4f(1,0,1,1);
                draw_cube();
                //glColor4f(1,1,1,1);
                glFlush();
            }
        }
        //CheckGlError(false);
    }
	prog->SetActive(false);
	glDisable(GL_DEPTH_TEST);
	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
	Farticles::Run(Farticles::Additive, proj, view);
}
