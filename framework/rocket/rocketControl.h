#pragma once

#ifndef SYNC_PLAYER
#include "sync\sync.h"
#endif

struct sync_device;
struct sync_cb;

extern bool isRocketAvailable;

extern sync_device *g_pRocket; 

#ifndef SYNC_PLAYER
extern struct sync_cb rocket_cb;
#endif /* !defined(SYNC_PLAYER) */

double rocket_get_row();

void InitRocket();
void UpdateRocket();

#ifdef SYNC_PLAYER
double sync_get_val(const struct sync_track *t, double row);
const struct sync_track *sync_get_track(const char* const trackNamesIn[], const short numKeysIn[], const float valuesIn[], const short deltaRowsIn[], const char interpolationsIn[], const char *trackName);
#endif


/*
struct sync_track;
const struct sync_track *sync_get_track(struct sync_device *, const char *trackName);
double sync_get_val(const struct sync_track *t, double row);*/
