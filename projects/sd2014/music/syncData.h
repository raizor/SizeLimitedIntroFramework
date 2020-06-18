#pragma once

extern const struct sync_track *syncEyeX;
extern const struct sync_track *syncEyeY;
extern const struct sync_track *syncEyeZ;


extern const struct sync_track *cubeX;
extern const struct sync_track *cubeY;
extern const struct sync_track *cubeZ;

extern const struct sync_track *objectX;
extern const struct sync_track *objectY;
extern const struct sync_track *objectZ;

extern const struct sync_track *objectMix;
extern const struct sync_track *camRadius;

extern const struct sync_track *rotPitch;
extern const struct sync_track *rotRoll;
extern const struct sync_track *rotHeading;

extern const struct sync_track *terrainEffectNum;

extern const struct sync_track *skyCol1_R;
extern const struct sync_track *skyCol1_G;
extern const struct sync_track *skyCol1_B;

extern const struct sync_track *skyCol2_R;
extern const struct sync_track *skyCol2_G;
extern const struct sync_track *skyCol2_B;


extern const struct sync_track *screenSize1;
extern const struct sync_track *screenSize2;
extern const struct sync_track *screenSize3;

extern const struct sync_track *screenDeform;

extern const struct sync_track *textPosX;
extern const struct sync_track *textPosY;
extern const struct sync_track *textPosZ;

extern const struct sync_track *textPos2X;
extern const struct sync_track *textPos2Y;
extern const struct sync_track *textPos2Z;

extern const struct sync_track *sunPosX;
extern const struct sync_track *sunPosY;
extern const struct sync_track *sunPosZ;

extern const struct sync_track *textNum;

extern const struct sync_track *objectNum;

extern const struct sync_track *textAmount;

void syncDataInit();