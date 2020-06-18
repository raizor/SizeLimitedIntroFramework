#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <math.h>
#include "rocketControl.h"
#include "..\src\framework\utils8k.h"

#include "framework_config.h"

#ifdef ENABLE_ROCKET
#ifdef SYNC_PLAYER
	//#include "../../sync.rocket.h"
#endif
#endif

static bool isRPSInited = false;
static double rowspersecond = 1;

extern float introTimeSecs;

double rocket_get_row()
{
	float time = introTimeSecs;
	return time * rowspersecond;
}

#ifndef SYNC_PLAYER

extern void TogglePause(bool pause);

void rocket_pause(void *d, int flag)
{
	if (flag)
		TogglePause(flag!=0);	
	else
		TogglePause(flag!=0);
}

void rocket_set_row(void *d, int row)
{
	float time = (float)row / (float)rowspersecond + 0.02f;
	introTimeSecs = time;
}

int rocket_is_playing(void *d)
{
	return 1;//!g_pDemo->IsPaused();
}

struct sync_cb rocket_cb = {
	rocket_pause,
	rocket_set_row,
	rocket_is_playing
};
#endif /* !defined(SYNC_PLAYER) */

bool isRocketAvailable = false;

sync_device* g_pRocket = NULL;

void InitRocket()
{
#ifdef ENABLE_ROCKET
	isRocketAvailable = true;

#ifndef SYNC_PLAYER
	g_pRocket = sync_create_device("sync");
	if (!g_pRocket)
	{
		isRocketAvailable = false;
		//umsg("Rocket failed to create sync device;");
	}

	if (isRocketAvailable)
	{
		//sync_set_callbacks(g_pRocket, &rocket_cb, NULL);
		if (sync_connect(g_pRocket, "localhost", SYNC_DEFAULT_PORT))
		{
			isRocketAvailable = false;
			//umsg("Rocket failed to connect to host.");
		}
	}
#endif
#endif
}

void UpdateRocket()
{

#ifdef ENABLE_ROCKET
	if (isRocketAvailable)
	{
		if (!isRPSInited)
		{
			const int rpb = 6; /* rows per beat */
			rowspersecond = 194.0 / 60.f * (float)rpb;
			isRPSInited = true;
		}

#ifndef SYNC_PLAYER
		static int lastUpdateRow = -1;

		int row = (int)floor(rocket_get_row());

		//if (row != lastUpdateRow)
		{
			if (sync_update(g_pRocket, row, &rocket_cb, NULL))
				sync_connect(g_pRocket, "localhost", SYNC_DEFAULT_PORT);

			lastUpdateRow = row;
		}
#endif
#endif
	}
}

#ifdef SYNC_PLAYER
enum key_type {
	KEY_STEP,   /* stay constant */
	KEY_LINEAR, /* lerp to the next value */
	KEY_SMOOTH, /* smooth curve to the next value */
	KEY_RAMP,
	KEY_TYPE_COUNT
};

struct sync_device;

struct track_key {
	int row;
	float value;
	enum key_type type;
};

struct sync_track {
	char *name;
	struct track_key *keys;
	size_t num_keys;
};

int sync_find_key(const struct sync_track *, int);
static inline int key_idx_floor(const struct sync_track *t, int row)
{
	int idx = sync_find_key(t, row);
	if (idx < 0)
		idx = -idx - 2;
	return idx;
}

static float key_linear(const struct track_key k[2], double row)
{
	double t = (row - k[0].row) / (k[1].row - k[0].row);
	return (float)(k[0].value + (k[1].value - k[0].value) * t);
}

static float key_smooth(const struct track_key k[2], double row)
{
	double t = (row - k[0].row) / (k[1].row - k[0].row);
	t = t * t * (3 - 2 * t);
	return (float)(k[0].value + (k[1].value - k[0].value) * t);
}

static float key_ramp(const struct track_key k[2], double row)
{
	double t = (row - k[0].row) / (k[1].row - k[0].row);
	t = pow(t, 2.0);
	return (float)(k[0].value + (k[1].value - k[0].value) * t);
}

const struct sync_track *sync_get_track(const char* const trackNamesIn[], const short numKeysIn[], const float valuesIn[], const short deltaRowsIn[], const char interpolationsIn[], const char *trackName)
{
	// Find track
	int trackIndex = 0;
	int keyIndex = 0;

	while (1)
	{
		if (zstreql(trackName, trackNamesIn[trackIndex]))
			break;

		keyIndex += numKeysIn[trackIndex];
		trackIndex++;
	}

	// Setup track structure
	int numKeys = numKeysIn[trackIndex];

	sync_track* track = new sync_track();
	track->name = (char*)trackNamesIn[trackIndex];
	track->num_keys = numKeysIn[trackIndex];
	track->keys = new struct track_key[numKeys];

	// Add keys
	int currentRow = 0;

	for (int i=0; i<numKeys; i++)
	{
		// De-delta row indices
		currentRow += deltaRowsIn[keyIndex];

		track->keys[i].row = currentRow;
		track->keys[i].type = (key_type)interpolationsIn[keyIndex];
		track->keys[i].value = valuesIn[keyIndex];

		keyIndex++;
	}

	return track;
}

int sync_find_key(const struct sync_track *t, int row)
{
	int lo = 0, hi = t->num_keys;
	while (lo < hi) {
		int mi = (lo + hi) / 2;
		//ZASSERT(mi != hi);

		if (t->keys[mi].row < row)
			lo = mi + 1;
		else if (t->keys[mi].row > row)
			hi = mi;
		else
			return mi;
	}
	return -lo - 1;
}

double sync_get_val(const struct sync_track *t, double row)
{
	int idx, irow;
	if (!t->num_keys)
		return 0.0f;

	irow = (int)floor(row);
	idx = key_idx_floor(t, irow);
	if (idx < 0)
		return t->keys[0].value;
	if (idx > (int)t->num_keys - 2)
		return t->keys[t->num_keys - 1].value;

	switch (t->keys[idx].type) {
	case KEY_STEP:
		return t->keys[idx].value;
	case KEY_LINEAR:
		return key_linear(t->keys + idx, row);
	case KEY_SMOOTH:
		return key_smooth(t->keys + idx, row);
	case KEY_RAMP:
		return key_ramp(t->keys + idx, row);
	default:
#ifdef DEBUG
		ZASSERT(0);
#endif
		return 0.0f;
	}
}

#endif

