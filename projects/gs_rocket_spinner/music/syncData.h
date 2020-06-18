#pragma once

extern const struct sync_track *syncScene;
extern const struct sync_track *syncTest;

extern const struct sync_track *syncEyeX;
extern const struct sync_track *syncEyeY;
extern const struct sync_track *syncEyeZ;


extern const struct sync_track *cubeX;
extern const struct sync_track *cubeY;
extern const struct sync_track *cubeZ;

extern const struct sync_track *rotPitch;
extern const struct sync_track *rotRoll;
extern const struct sync_track *rotHeading;

void syncDataInit();