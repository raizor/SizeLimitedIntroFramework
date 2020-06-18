//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008                                        //
//--------------------------------------------------------------------------//

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include "config.h"
#include "intro.h"
#include "clinkster.h"

float introTimeSecs = 0;
float introLength = 0;

static const PIXELFORMATDESCRIPTOR pfd =
    {
    sizeof(PIXELFORMATDESCRIPTOR),
    1,
    PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,
    PFD_TYPE_RGBA,
    32,
    0, 0, 0, 0, 0, 0, 8, 0,
    0, 0, 0, 0, 0,
    32, 0, 0,
    PFD_MAIN_PLANE,
    0, 0, 0, 0
    };

#ifdef SETRESOLUTION
static DEVMODE screenSettings = { {0},
    #if _MSC_VER < 1400
    0,0,148,0,0x001c0000,{0},0,0,0,0,0,0,0,0,0,{0},0,32,XRES,YRES,0,0,      // Visual C++ 6.0
    #else
    0,0,156,0,0x001c0000,{0},0,0,0,0,0,{0},0,32,XRES,YRES,{0}, 0,           // Visuatl Studio 2005
    #endif
    #if(WINVER >= 0x0400)
    0,0,0,0,0,0,
    #if (WINVER >= 0x0500) || (_WIN32_WINNT >= 0x0400)
    0,0
    #endif
    #endif
    };
#endif

void entrypoint( void )
{              
    // full screen
    #ifdef SETRESOLUTION
    if( ChangeDisplaySettings(&screenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL) return;
    ShowCursor( 0 );
    #endif
    // create window
    HWND hWnd = CreateWindow( "static",0,WS_POPUP|WS_VISIBLE|WS_MAXIMIZE,0,0,0,0,0,0,0,0);
    HDC hDC = GetDC(hWnd);
    // initalize opengl
    if( !SetPixelFormat(hDC,ChoosePixelFormat(hDC,&pfd),&pfd) ) return;
    HGLRC hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC,hRC);

	// init intro
	intro_init();

	// generate and play Clinkster tune
	Clinkster_GenerateMusic();
	Clinkster_StartMusic();

	long to = timeGetTime();
    do 
	{
		float pos = Clinkster_GetPosition();
		if (pos > Clinkster_MusicLength) break;

		for (unsigned int i = 0 ; i < Clinkster_NumTracks ; i++) {
			float t = Clinkster_GetInstrumentTrigger(i, pos);
			int ti = (int)(t * 0.25);
			if (ti > 3) ti = 3;
			//printf("%c",  "|:. "[ti]);
		}

		long nowMs = timeGetTime();
		long elapsedMs = nowMs - to;
		introTimeSecs += (float)elapsedMs * 0.001f;
		to = nowMs;
		
        intro_do( introTimeSecs );
    
        //SwapBuffers ( hDC );   
        wglSwapLayerBuffers( hDC, WGL_SWAP_MAIN_PLANE );

	}while ( !GetAsyncKeyState(VK_ESCAPE)/* && MMTime.u.sample < MAX_SAMPLES*/ );

    ExitProcess(0);
}

