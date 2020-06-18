#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include "../../framework/src/config.h"

#ifndef SYNC_PLAYER
#include "../../../framework/rocket/sync/device.h"
#else
#include "rocket/rocketControl.h"
#include "sync_rocket.h"
#endif

const struct sync_track *syncEyeX = 0;
const struct sync_track *syncEyeY = 0;
const struct sync_track *syncEyeZ = 0;

const struct sync_track *cubeX = 0;
const struct sync_track *cubeY = 0;
const struct sync_track *cubeZ = 0;

const struct sync_track *rotPitch = 0;
const struct sync_track *rotRoll = 0;
const struct sync_track *rotHeading = 0;

const struct sync_track *objectX = 0;
const struct sync_track *objectY = 0;
const struct sync_track *objectZ = 0;

const struct sync_track *objectMix = 0;
const struct sync_track *camRadius = 0;

const struct sync_track *terrainEffectNum = 0;

const struct sync_track *skyCol1_R = 0;
const struct sync_track *skyCol1_G = 0;
const struct sync_track *skyCol1_B = 0;

const struct sync_track *skyCol2_R = 0;
const struct sync_track *skyCol2_G = 0;
const struct sync_track *skyCol2_B = 0;

const struct sync_track *screenSize1 = 0;
const struct sync_track *screenSize2 = 0;
const struct sync_track *screenSize3 = 0;

const struct sync_track *screenDeform = 0;

const struct sync_track *textPosX = 0;
const struct sync_track *textPosY = 0;
const struct sync_track *textPosZ = 0;

const struct sync_track *textPos2X = 0;
const struct sync_track *textPos2Y = 0;
const struct sync_track *textPos2Z = 0;

const struct sync_track *sunPosX = 0;
const struct sync_track *sunPosY = 0;
const struct sync_track *sunPosZ = 0;

const struct sync_track *textNum = 0;

const struct sync_track *textAmount = 0;

const struct sync_track *objectNum = 0;

const struct sync_track *sceneNum = 0;


extern sync_device *g_pRocket; 

void syncDataInit()
{

#ifdef DEBUG
	
	sceneNum = sync_get_track(g_pRocket, "scene.num");

	syncEyeX = sync_get_track(g_pRocket, "eye.x");
	syncEyeY = sync_get_track(g_pRocket, "eye.y");
	syncEyeZ = sync_get_track(g_pRocket, "eye.z");

	cubeX = sync_get_track(g_pRocket, "cube.x");
	cubeY = sync_get_track(g_pRocket, "cube.y");

	cubeZ = sync_get_track(g_pRocket, "cube.z");

	rotPitch = sync_get_track(g_pRocket, "rot.pitch");
	rotRoll = sync_get_track(g_pRocket, "rot.roll");
	rotHeading = sync_get_track(g_pRocket, "rot.heading");

	objectX = sync_get_track(g_pRocket, "object.x");
	objectY = sync_get_track(g_pRocket, "object.y");
	objectZ = sync_get_track(g_pRocket, "object.z");

	objectMix = sync_get_track(g_pRocket, "object.mix");
	camRadius = sync_get_track(g_pRocket, "cam.radius");

	terrainEffectNum = sync_get_track(g_pRocket, "ter.effectNum");
	
	skyCol1_R = sync_get_track(g_pRocket, "sky1.r");
	skyCol1_G = sync_get_track(g_pRocket, "sky1.g");
	skyCol1_B = sync_get_track(g_pRocket, "sky1.b");

	skyCol2_R = sync_get_track(g_pRocket, "sky2.r");
	skyCol2_G = sync_get_track(g_pRocket, "sky2.g");
	skyCol2_B = sync_get_track(g_pRocket, "sky2.b");

	screenSize1 = sync_get_track(g_pRocket, "screen.size1");
	screenSize2 = sync_get_track(g_pRocket, "screen.size2");
	screenSize3 = sync_get_track(g_pRocket, "screen.size3");

	screenDeform = sync_get_track(g_pRocket, "screen.deform");

	textPosX = sync_get_track(g_pRocket, "txt.x");
	textPosY = sync_get_track(g_pRocket, "txt.y");
	textPosZ = sync_get_track(g_pRocket, "txt.z");

	textPos2X = sync_get_track(g_pRocket, "txt2.x");
	textPos2Y = sync_get_track(g_pRocket, "txt2.y");
	textPos2Z = sync_get_track(g_pRocket, "txt2.z");

	textNum = sync_get_track(g_pRocket, "txt.num");
	textAmount = sync_get_track(g_pRocket, "txt.amount");

	objectNum = sync_get_track(g_pRocket, "object.num");

	sunPosX = sync_get_track(g_pRocket, "sun.x");
	sunPosY = sync_get_track(g_pRocket, "sun.y");
	sunPosZ = sync_get_track(g_pRocket, "sun.z");
	

#else

	syncEyeX = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "eye.x");
	syncEyeY = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "eye.y");
	syncEyeZ = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "eye.z");

	cubeX = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "cube.x");
	cubeY = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "cube.y");

	cubeZ = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "cube.z");

	rotPitch = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "rot.pitch");
	rotRoll = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "rot.roll");
	rotHeading = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "rot.heading");

	objectX = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "object.x");
	objectY = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "object.y");
	objectZ = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "object.z");

	objectMix = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "object.mix");
	camRadius = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "cam.radius");

	terrainEffectNum = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "ter.effectNum");

	skyCol1_R = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "sky1.r");
	skyCol1_G = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "sky1.g");
	skyCol1_B = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "sky1.b");

	skyCol2_R = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "sky2.r");
	skyCol2_G = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "sky2.g");
	skyCol2_B = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "sky2.b");

	screenSize1 = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "screen.size1");
	screenSize2 = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "screen.size2");
	screenSize3 = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "screen.size3");

	screenDeform = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "screen.deform");

	textPosX = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "txt.x");
	textPosY = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "txt.y");
	textPosZ = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "txt.z");

	textPos2X = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "txt2.x");
	textPos2Y = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "txt2.y");
	textPos2Z = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "txt2.z");

	textNum = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "txt.num");

	textAmount = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "txt.amount");

	objectNum = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "object.num");

	sunPosX = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "sun.x");
	sunPosY = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "sun.y");
	sunPosZ = sync_get_track(trackNames, numKeys, values, deltaRows, interpolations, "sun.z");
#endif

}
