#include "Farticles.h"

#include "Noise3D.h"
//#include "PerkProc.h"
//#include "Bloom.h"

#define NUM_PARTS_X 800 
#define NUM_PARTS_Y 600

static shader* shaderVel;
static shader* shaderPos;
static shader* shaderPosInit;
static shader* shaderRender;
static shader* shaderRenderAdd;

int xx = 0;

void Farticles::Init()
{
	get();
}

ZFbo* Farticles::GetFboHandle()
{
	return get()->fbo;
}

Farticles::Farticles()
{
	ZFboDesc fboDesc;
	fboDesc.numColorBuffers = 2;
	fboDesc.colorBufferFormat = GL_RGBA16F;
	fboDesc.colorBufferInternalFormat = GL_RGBA;
	fboDesc.colorBufferType = GL_UNSIGNED_BYTE;

	fboDesc.width = 800;
	fboDesc.height = 600;	
	fboDesc.depth = 0;

	fboDesc.hasDepthBuffer = true;
	fboDesc.depthBufferFormat = GL_DEPTH_COMPONENT16;

	fboDesc.enableMipmaps = false;
	fboDesc.magFilter = GL_LINEAR;
	fboDesc.minFilter = GL_LINEAR;
	fboDesc.wrapModeX = GL_CLAMP_TO_EDGE;
	fboDesc.wrapModeY = GL_CLAMP_TO_EDGE;
	fboDesc.textureType = GL_TEXTURE_2D;


	//fbo = new Fbo(1280, 720, true, 2, GL_RGBA16F);
	fbo = new ZFbo(fboDesc);
	fboDesc.numColorBuffers = 1;
	//fb2 = globalFboManager->addFbo(info);

	/*
	vertParticlePosInit = new Shader("Data/ParticlePosInit.vert", GL_VERTEX_SHADER);
	fragParticlePosInit = new Shader("Data/ParticlePosInit.frag", GL_FRAGMENT_SHADER);
	progParticlePosInit = new Program(vertParticlePosInit, fragParticlePosInit);
	*/

	shaderPosInit = new shader("effects/particles/ParticlePosInit.vert", NULL, "effects/particles/ParticlePosInit.frag", "", "POS INIT");
	shaderPos = new shader("effects/particles/ParticlePosPass.vert", NULL, "effects/particles/ParticlePosPass.frag", "", "POS");
	shaderVel = new shader("effects/particles/ParticleVelPass.vert", NULL, "effects/particles/ParticleVelPass.frag", "", "VEL");
	shaderRender = new shader("effects/particles/ParticleRender.vert", NULL, "effects/particles/ParticleRender.frag", "", "RENDER");
	shaderRenderAdd = new shader("effects/particles/ParticleRender.vert", NULL, "effects/particles/ParticleRenderAdditive.frag", "", "ADD");
	
	//CheckGlError(true);
	glGenBuffers(1, &particleRenderBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, particleRenderBuffer);
	//CheckGlError(true);
	int renderBufferElements = NUM_PARTS_X * NUM_PARTS_Y * 4 * 2 * 2;
	int *renderBufferData = new int[renderBufferElements];
	for (int y = 0; y < NUM_PARTS_Y; y++)
	{
		for (int x = 0; x < NUM_PARTS_X; x++)
		{
			int bufferIndex = (y * NUM_PARTS_X + x) * 4 * 2 * 2;

			renderBufferData[bufferIndex +  0] = x;
			renderBufferData[bufferIndex +  1] = y;
			renderBufferData[bufferIndex +  2] = 0;
			renderBufferData[bufferIndex +  3] = 0;

			renderBufferData[bufferIndex +  4] = x;
			renderBufferData[bufferIndex +  5] = y;
			renderBufferData[bufferIndex +  6] = 1;
			renderBufferData[bufferIndex +  7] = 0;

			renderBufferData[bufferIndex +  8] = x;
			renderBufferData[bufferIndex +  9] = y;
			renderBufferData[bufferIndex + 10] = 1;
			renderBufferData[bufferIndex + 11] = 1;

			renderBufferData[bufferIndex + 12] = x;
			renderBufferData[bufferIndex + 13] = y;
			renderBufferData[bufferIndex + 14] = 0;
			renderBufferData[bufferIndex + 15] = 1;
		}
	}
	glBufferData(GL_ARRAY_BUFFER, renderBufferElements * sizeof(int), renderBufferData, GL_STATIC_DRAW);
	delete [] renderBufferData;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//CheckGlError(true);
	for (int i = 0; i < 2; i++)
	{
		fboDesc.hasDepthBuffer = false;
		fboDesc.numColorBuffers = 1;
		//positionLifeFbos[i] = new Fbo(1280, 720, false, 1, GL_RGBA16F);
		positionLifeFbos[i] = new ZFbo(fboDesc);
		//CheckGlError(true);
		glBindFramebufferEXT(GL_FRAMEBUFFER, positionLifeFbos[i]->id);
		//CheckGlError(true);

		shaderPosInit->SetActive(true);
		//CheckGlError(true);
		GLuint noiseTex = Noise3D::GetHandle();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_3D, noiseTex);
		GLint loc = glGetUniformLocation(shaderPosInit->shaderProg, "NoiseTex");
		glUniform1i(loc, 0);
		//CheckGlError(false);
		glRecti(-1, -1, 1, 1);
		//CheckGlError(true);
		/*
		glBegin(GL_QUADS);
		//glTexCoord3f(0, 0, 0);
		glVertex3f(-1, -1, -5);

		//glTexCoord3f(0, 1, 0);
		glVertex3f(-1,  1, -5);

		//glTexCoord3f(1, 1, 0);
		glVertex3f( 1,  1, -5);

		//glTexCoord3f(1, 0, 0);
		glVertex3f( 1, -1, -5);
		glEnd();*/
		glBindTexture(GL_TEXTURE_3D, 0);
		shaderPosInit->SetActive(false);

		glBindFramebufferEXT(GL_FRAMEBUFFER, 0);

		velocityFbos[i] = new ZFbo(fboDesc);
		
		glBindFramebufferEXT(GL_FRAMEBUFFER, velocityFbos[i]->id);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
	}
	//CheckGlError(true);
	bufferIndex = 0;
}


void Farticles::Run(int style, glm::mat4& proj, glm::mat4& view)
{
	Farticles *f = get();
	
	extern double timeVal;
	// Recalc time step
	double globalTime = timeVal;// f->globalTimer.GetTime();
	float timeStep = (float)(globalTime - f->lastTime)+0.0001f;
	f->lastTime = globalTime;

	// Particle velocity integration pass
	int bufferIndex2 = 1 - f->bufferIndex;
	glBindFramebufferEXT(GL_FRAMEBUFFER, f->velocityFbos[f->bufferIndex]->id);
	shaderVel->SetActive(true);
	{
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_3D, Noise3D::GetHandle(1));

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_3D, Noise3D::GetHandle());

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, f->positionLifeFbos[bufferIndex2]->colorBufferID[0]);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, f->velocityFbos[bufferIndex2]->colorBufferID[0]);


		glUniform1i(glGetUniformLocation(shaderVel->shaderProg, "OldVelTex"), 0);
		glUniform1i(glGetUniformLocation(shaderVel->shaderProg, "OldPosLifeTex"), 1);
		glUniform1i(glGetUniformLocation(shaderVel->shaderProg, "NoiseTex"), 2);
		glUniform1i(glGetUniformLocation(shaderVel->shaderProg, "NoiseTex2"), 3);
		glUniform1f(glGetUniformLocation(shaderVel->shaderProg, "Time"), (float) globalTime);
		glUniform1f(glGetUniformLocation(shaderVel->shaderProg, "TimeStep"), timeStep);

		glUniform1f(glGetUniformLocation(shaderVel->shaderProg, "AccSpeed"), 4);
		glUniform1f(glGetUniformLocation(shaderVel->shaderProg, "VelDamp"), 0.99000001f);
		//oglUniform1f(oglGetUniformLocation(f->progParticleVelPass->GetHandle(), "AccSpeed"), Sync::GetTrackValue("Acc Speed"));
		//oglUniform1f(oglGetUniformLocation(f->progParticleVelPass->GetHandle(), "VelDamp"), Sync::GetTrackValue("Vel Damping"));


		glRecti(-1, -1, 1, 1);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_3D, 0);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_3D, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	shaderVel->SetActive(false);

	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);

	// Particle position integration pass
	glBindFramebufferEXT(GL_FRAMEBUFFER, f->positionLifeFbos[f->bufferIndex]->id);
	shaderPos->SetActive(true);
	{
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_3D, Noise3D::GetHandle());
		glUniform1i(glGetUniformLocation(shaderPos->shaderProg, "NoiseTex"), 3);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, f->fbo->colorBufferID[0]);
		glUniform1i(glGetUniformLocation(shaderPos->shaderProg, "PosTex"), 2);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, f->velocityFbos[f->bufferIndex]->colorBufferID[0]);
		glUniform1i(glGetUniformLocation(shaderPos->shaderProg, "VelTex"), 1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, f->positionLifeFbos[bufferIndex2]->colorBufferID[0]);
		glUniform1i(glGetUniformLocation(shaderPos->shaderProg, "OldPosLifeTex"), 0);
				
		glUniform1f(glGetUniformLocation(shaderPos->shaderProg, "TimeStep"), timeStep);
		glUniform1f(glGetUniformLocation(shaderPos->shaderProg, "VelSpeed"), 0.050000001f);
		glUniform1f(glGetUniformLocation(shaderPos->shaderProg, "DeathSpeed"), 1);
		//oglUniform1f(glGetUniformLocation(f->progParticlePosPass->GetHandle(), "VelSpeed"), Sync::GetTrackValue("Vel Speed"));
		//oglUniform1f(glGetUniformLocation(f->progParticlePosPass->GetHandle(), "DeathSpeed"), Sync::GetTrackValue("Death Speed"));

		//glRecti(-1, -1, 1, 1);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_3D, 0);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);	
	}
	shaderPos->SetActive(false);

    // Particle render pass
    GLint renderProg;
    switch (style)
    {
        case Solid: renderProg = shaderRender->shaderProg; break;
		case Additive: renderProg = shaderRenderAdd->shaderProg; break;
    }

    //glBindFramebuffer(GL_FRAMEBUFFER, PerkProc::GetBufferHandle());
    glBindFramebufferEXT(GL_FRAMEBUFFER, 0);

    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shaderRender->SetActive(true);
    {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, f->positionLifeFbos[bufferIndex2]->colorBufferID[0]);
		glUniform1i(glGetUniformLocation(shaderRender->shaderProg, "PosTex"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, f->fbo->colorBufferID[1]);
		glUniform1i(glGetUniformLocation(shaderRender->shaderProg, "ColorTex"), 1);
		
		glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_3D, Noise3D::GetHandle());
		glUniform1i(glGetUniformLocation(shaderRender->shaderProg, "NoiseTex"), 2);
		
		glUniformMatrix4fv(glGetUniformLocation(shaderRender->shaderProg, "Proj"), 1, false, (float *)&proj[0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderRender->shaderProg, "View"), 1, false, (float *)&view[0]);
		glUniform1f(glGetUniformLocation(shaderRender->shaderProg, "Time"), (float)globalTime);
		glUniform1f(glGetUniformLocation(shaderRender->shaderProg, "CamShake"), 0.0f);
		glUniform1f(glGetUniformLocation(shaderRender->shaderProg, "Brightness"), 1.0f);
        switch (style)
        {
            case Solid: glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break;
            case Additive: glBlendFunc(GL_ONE, GL_ONE); break;
        }
        glEnable(GL_BLEND);
        glBindBuffer(GL_ARRAY_BUFFER, f->particleRenderBuffer);
        glEnableClientState(GL_VERTEX_ARRAY);
        glClientActiveTexture(GL_TEXTURE0);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glVertexPointer(2, GL_INT, 16, 0);
        glTexCoordPointer(2, GL_INT, 16, (const GLvoid *) 8);
        glDrawArrays(GL_QUADS, 0, NUM_PARTS_X * NUM_PARTS_Y * 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisable(GL_BLEND);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_3D, 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
	shaderRender->SetActive(false);

    // PerkProc
    //glBindFramebuffer(GL_FRAMEBUFFER, Bloom::GetBufferHandle());
   // PerkProc::Run();

    // Bloom
    glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
    //Bloom::Run();
    //Bloom::Mix(Sync::GetTrackValue("Bloom Wet"), Sync::GetTrackValue("Bloom Dry"));

    // Swap buffers
    f->bufferIndex = bufferIndex2;
}

Farticles::~Farticles()
{
	delete fbo;

	delete vertParticlePosInit;
	delete fragParticlePosInit;
	delete progParticlePosInit;

	delete vertParticleVelPass;
	delete fragParticleVelPass;
	delete progParticleVelPass;

	delete vertParticlePosPass;
	delete fragParticlePosPass;
	delete progParticlePosPass;

	delete vertParticleRender;
	delete fragParticleRender;
	delete progParticleRender;

	delete vertParticleRenderAdditive;
	delete fragParticleRenderAdditive;
	delete progParticleRenderAdditive;

	//glDeleteBuffers(1, &particleRenderBuffer);

	for (int i = 0; i < 2; i++)
	{
		delete positionLifeFbos[i];
		delete velocityFbos[i];
	}

	lastTime = 0.0;
}

Farticles *Farticles::get()
{
	static Farticles f;
	return &f;
}
