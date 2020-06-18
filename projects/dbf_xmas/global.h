#pragma once
#include "framework/utils8k.h"
#include "framework/gl/fbo.h"
#include "framework/gl/gl8k.h"
#include "framework/math/vector.h"
#include "framework/math/matrix.h"
#include "zassert.h"

#define ENABLE_PP 1

#define M_PI 3.141926

#define INTRO_TITLE "Eventide Mini Framework v0.1"

#define ENABLE_ROCKET

#define MUTE_AUDIO

#ifdef DEBUG
#define ENABLE_SCRUBBER
#define ENABLE_MUSIC	
#else
#define SYNC_PLAYER
#endif

#define CLINKSTER_EVENTS
#define VBO_INDEX_BUFFER

// utils toggles

#define ENABLE_ZSTREQL

#define fzn  1.0f
#define fzf  1000.0f

#define STATE global::GetState();
#define ROCKET_ROW global::GetState()->rocketRow

#define SYNC_CAM_RADIUS global::GetState()->camRadiusVal
#define SYNC_OBJ_MIX_1 global::GetState()->objectMix1
#define SYNC_LIGHT_1_POS global::GetState()->lightPos1

#define MAT_PROJ global::GetState()->matrixProjection
#define MAT_PROJ_INV global::GetState()->matrixProjectionInverse
#define MAT_MV global::GetState()->matrixModelView
#define MAT_MV_INV global::GetState()->matrixModelViewInverse
#define MAT_NORM global::GetState()->matrixNormal

#define POS_EYE global::GetState()->eye
#define POS_LOOK global::GetState()->look
#define POS_CUBE global::GetState()->cube
#define POS_LOOK global::GetState()->look
#define POS_OBJ global::GetState()->objectPos

#define RES_X  global::GetState()->resX
#define RES_Y  global::GetState()->resY
#define DRAW_X  global::GetState()->drawX
#define DRAW_Y  global::GetState()->drawY
#define BAR_SIZE  global::GetState()->barSize
#define AR  global::GetState()->ar

#define FBO global::GetState()->fbo
#define FBO_CULLING global::GetState()->fboCulling

struct ZPointGridVertex
{
	float x, y, z, w;
	float fx, fy;
	float dummy[2]; // 32 bytes
};

typedef unsigned int ZPointGridVertexIndex;

class state
{
public:
	float normalMatrix[9];
	ZVector eye, look, cube, objectPos;
	ZFbo  *fbo, *fboCulling, *fboRender;

	ZMatrix matrixProjection;
	ZMatrix matrixProjectionInverse;
	ZMatrix matrixModelView;
	ZMatrix matrixModelViewInverse;
	ZMatrix matrixNormal;

	int resX;
	int resY;
	int drawX;
	int drawY;
	int barSize;
	float ar;

	// todo: zero init these!
	float objectMixVal;
	float camRadiusVal;
	double rocketRow;
	float lightPos1[4];
	float objectMix1;
	state();
};

class global
{
public:
	static state* GetState();

private:
	static state* _state;
};