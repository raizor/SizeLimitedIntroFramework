//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008                                        //
//--------------------------------------------------------------------------//

#ifndef _INTRO_H_
#define _INTRO_H_

typedef struct WININFO
{
	//---------------
	HINSTANCE   hInstance;
	HDC         hDC;
	HGLRC       hRC;
	HWND        hWnd;
	//---------------
	int         full;
	//---------------
	char        wndclass[4];	// window class and title :)
	DWORD       width;
	DWORD	    height;
	//---------------
}WININFO;

void intro_init( void );
void intro_do(double time);
void intro_end( void );

void draw_cube( void );

float sfrand( int *seed );

float floor4k(float x);

#endif
