#pragma once 
#include <stdio.h>
#include <windows.h>

typedef struct {
  int scrWidth;
  int scrHeight;
  int scrBPP;
  int nWindowed;
  int nVsync;
  int nMultisample;
  int nAlwaysOnTop;

  HINSTANCE hInstance;
  HGLRC hRC;
  HWND hWnd;
  HDC hDC;
} fwzSettings; 

int OpenSetupDialog(fwzSettings * setup);