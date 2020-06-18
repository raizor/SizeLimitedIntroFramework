#pragma once
#include "..\..\config.h"
#include "..\noise.h"
#include "framework\math\vector.h"

class ZMatrix;
class ZVector;
class ZFbo;

class ZSplinePath
{
public:
	GLUquadric* quadric;
	int pointsMax;
	int pointsAllocated;
	int pointsCurve;
	int hotPointIndex;
	int selectedPointIndex;
	ZVector* vertices;
	ZVector* pathPoints;
	ZSplinePath(int numPoints = 500);
	~ZSplinePath(void);
	void GenerateSpline(int segmentsPerCurve);
	void AddPoint(ZVector* vertex);
	void Draw(bool drawControlPoints, bool drawCurve, bool drawLines, bool doPicking);
};

