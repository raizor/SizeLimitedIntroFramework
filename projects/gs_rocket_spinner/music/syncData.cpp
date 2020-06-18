#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include "../../framework/src/config.h"

#ifndef SYNC_PLAYER
#include "../../rocket/sync/device.h"
#else
#include "../rocket/rocketControl.h"
#endif

const struct sync_track *syncScene = 0;
const struct sync_track *syncTest = 0;

const struct sync_track *syncEyeX = 0;
const struct sync_track *syncEyeY = 0;
const struct sync_track *syncEyeZ = 0;

const struct sync_track *cubeX = 0;
const struct sync_track *cubeY = 0;
const struct sync_track *cubeZ = 0;

const struct sync_track *rotPitch = 0;
const struct sync_track *rotRoll = 0;
const struct sync_track *rotHeading = 0;

extern sync_device *g_pRocket; 

void syncDataInit()
{
	syncScene = sync_get_track(g_pRocket, "sync.scene");
	syncTest = sync_get_track(g_pRocket, "sync.test");

	syncEyeX = sync_get_track(g_pRocket, "eye.x");
	syncEyeY = sync_get_track(g_pRocket, "eye.y");
	syncEyeZ = sync_get_track(g_pRocket, "eye.z");
	
	cubeX = sync_get_track(g_pRocket, "cube.x");
	cubeY = sync_get_track(g_pRocket, "cube.y");
	cubeZ = sync_get_track(g_pRocket, "cube.z");

	rotPitch = sync_get_track(g_pRocket, "rot.pitch");
	rotRoll = sync_get_track(g_pRocket, "rot.roll");
	rotHeading = sync_get_track(g_pRocket, "rot.heading");
}
