#include "splinePath.h"
#include "framework\utils8k.h"
#include "framework\gl\gl8k.h"
#include "framework\gl\shader.h"
#include "framework\gl\fbo.h"
#include "framework\math\matrix.h"

#include "../../rocket/rocketControl.h"
#include "music/syncData.h"

#include "../../testStuff/stb_image.h"
#include "../../testStuff/noise26_png.h"

#include "../noise.h"

#include <math.h>

#include <stdio.h>

extern int MOUSE_X, MOUSE_Y;
extern bool MOUSE_DOWN;


ZSplinePath::ZSplinePath(int numPoints)
{
	pointsMax = numPoints;
	pointsAllocated = 0;
	vertices = new ZVector[numPoints];
	quadric = gluNewQuadric(); 
	pathPoints = 0;
	hotPointIndex = -1;
	selectedPointIndex = -1;
}


ZSplinePath::~ZSplinePath(void)
{
}

void ZSplinePath::AddPoint(ZVector* vertex)
{
	if (pointsAllocated > pointsMax -2)
	{
		pointsMax *= 2;
		realloc(vertex, sizeof(ZVector) * pointsMax);
	}
	vertices[pointsAllocated++] = *vertex;
}

ZVector CalculateBezierPoint(float t, ZVector p0, ZVector p1, ZVector p2, ZVector p3)
{
	float u = 1.0f - t;
	float tt = t*t;
	float uu = u*u;
	float uuu = uu * u;
	float ttt = tt * t;

	ZVector p = p0.Mul(uuu); //first term
	p = p.Plus(p1.Mul(uu * t * 3.0f)); //second term
	p = p.Plus(p2.Mul(u * tt * 3.0f)); //third term
	p = p.Plus(p3.Mul(ttt)); //fourth term

	return p;
}

void ZSplinePath::GenerateSpline(int segmentsPerCurve)
{
	if (pathPoints)
	{
		delete(pathPoints);
		pathPoints = NULL;
	}

	pathPoints = new ZVector[((pointsAllocated - 3) * segmentsPerCurve) + 1];

	int idx = 0;

	for(int i = 0; i < pointsAllocated - 3; i+=3)
	{
		ZVector p0 = vertices[i];
		ZVector p1 = vertices[i + 1];
		ZVector p2 = vertices[i + 2];
		ZVector p3 = vertices[i + 3];   

		if (i==0)
			pathPoints[idx++] = CalculateBezierPoint(0, p0, p1, p2, p3);

		for(int j = 1; j <= segmentsPerCurve; j++)
		{
			float t = j / (float) segmentsPerCurve;
			pathPoints[idx++] = CalculateBezierPoint(t, p0, p1, p2, p3);
		}
	}

	pointsCurve = idx;
}

#define BUFSIZE 512
GLuint selectBuf[BUFSIZE];

void processHits2 (GLint hits, GLuint buffer[])
{
	//unsigned int i, j;
	GLuint names, *ptr, minZ,*ptrNames, numberOfNames;

	printf ("hits = %d\n", hits);
	ptr = (GLuint *) buffer;
	minZ = 0xffffffff;
	for (int i = 0; i < hits; i++)
	{	
		names = *ptr;
		ptr++;
		if (*ptr < minZ) {
			numberOfNames = names;
			minZ = *ptr;
			ptrNames = ptr+2;
		}
		ptr += names+2;
	}
	printf ("The closest hit names are ");
	//ptr = ptrNames;
	//for (j = 0; j < numberOfNames; j++,ptr++) {
		//printf ("%d ", *ptr);
	//}
	//printf ("\n");

}

void startPicking(int cursorX, int cursorY) 
{
	GLint viewport[4];

	//glSelectBuffer(BUFSIZE,selectBuf);
	//glRenderMode(GL_SELECT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glGetIntegerv(GL_VIEWPORT ,viewport);
	gluPickMatrix(cursorX,viewport[3]-(600-cursorY),	5,5,viewport);
	gluPerspective(45, (float)RES_X / RES_X,0.1,1000);
	//glMatrixMode(GL_MODELVIEW);
	glInitNames();
}

void stopPicking() 
{
	int hits = 0;

	// restoring the original projection matrix
	//glMatrixMode(GL_PROJECTION);
	//glPopMatrix();
	//glMatrixMode(GL_MODELVIEW);
	//glFlush();

	// returning to normal rendering mode
	//hits = glRenderMode(GL_RENDER);

	// if there are hits process them
	if (hits != 0)
		processHits2(hits, selectBuf);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void ZSplinePath::Draw(bool drawControlPoints, bool drawCurve, bool drawLines, bool doPicking)
{
	double pointSize = 0.05;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	//glLoadIdentity();
	if (drawControlPoints)
	{
		for (unsigned int i=0; i<(unsigned int)pointsAllocated; i++)
		{		
			if (doPicking)
			{
				int f = i+1;
				glColor3f((float)((f >> 16) & 255)/255.f, (float)((f >> 8) & 255)/255.f, (float)(f & 255)/255.f);
				//glColor4f(1, 1, 1, 1);
			}else{
				if (hotPointIndex == i+1)
				{
					glColor4f(1, 1, 0, 1);
				}else if (selectedPointIndex == i+1){
					glColor4f(0, 1, 0, 1);			
				}else{
					glColor4f(1, 1, 1, 1);
				}
				
			}

			ZVector* v = &vertices[i];		
			glTranslatef(v->x, v->y, v->z);
			gluSphere(quadric, pointSize, 4, 4);
			glTranslatef(-v->x, -v->y, -v->z);

			if (doPicking)
			{				
				unsigned int pixelColor = 0;


				glReadPixels( MOUSE_X, 600-MOUSE_Y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pixelColor );

				int clickedFaceIndex = ((pixelColor >> 16) & 255) + (((pixelColor >> 8) & 255) << 8) + (((pixelColor >> 0) & 255) << 16);

				hotPointIndex = clickedFaceIndex;

				if (MOUSE_DOWN)
				{
					selectedPointIndex = clickedFaceIndex;
				}

			
			}
		}
	}
		
	if (!doPicking)
	{
		if (drawCurve)
		{
			glColor4f(1, 1, 0, 1);
			glBegin(GL_LINE_STRIP);
			for (int i=0; i<pointsCurve; i++)
			{			
				ZVector* v = &pathPoints[i];		
				glVertex3d(v->x, v->y, v->z);
			}
			glEnd();
		}


		//glLoadIdentity();

		if (drawLines)
		{
			glColor4f(1, 0, 0, 1);
			glBegin(GL_LINE_STRIP);
			for (int i=0; i<pointsAllocated; i++)
			{			
				ZVector* v = &vertices[i];		
				glVertex3d(v->x, v->y, v->z);
			}
			glEnd();
		}
	}


	glPopMatrix();

}

