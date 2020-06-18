#pragma once 
#include <stdio.h>

typedef struct {
  int scrWidth;
  int scrHeight;
  int scrBPP;
  int nWindowed;
  int nVsync;
  int nMultisample;
  int nAlwaysOnTop;

  HINSTANCE hInstance;
} fwzSettings; 

int OpenSetupDialog(fwzSettings * setup);