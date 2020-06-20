//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008                                        //
//--------------------------------------------------------------------------//

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <clinkster.h>
#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include "config.h"
#include "intro.h"
#include "clinkster.h"
#include "framework/math/vector.h"
#include "rocket/rocketControl.h"
#include <direct3d.h>
#include "framework/dialog/fwzSetup.h"

static const char wndclass[] = "Merry Xmas DBF - Eventide";

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
    0,0,148,0,0x001c0000,{0},0,0,0,0,0,0,0,0,0,{0},0,32,RES_X,RES_Y,0,0,      // Visual C++ 6.0
    #else
    0,0,156,0,0x001c0000,{0},0,0,0,0,0,{0},0,32,RES_X,RES_Y,{0}, 0,           // Visuatl Studio 2005
    #endif
    #if(WINVER >= 0x0400)
    0,0,0,0,0,0,
    #if (WINVER >= 0x0500) || (_WIN32_WINNT >= 0x0400)
    0,0
    #endif
    #endif
    };
#endif

static LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( uMsg==WM_SYSCOMMAND && (wParam==SC_SCREENSAVE || wParam==SC_MONITORPOWER) )
		return 0 ;

	if( (uMsg==WM_CLOSE || (uMsg==WM_KEYDOWN && wParam==VK_ESCAPE)))
	{
		PostQuitMessage(0);
		return 0 ;
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

#define ALLOWWINDOWED
static int window_init(fwzSettings* setup)
{
	// hardcoded display settings
	setup->scrWidth = 1280;
	setup->scrHeight = 720;
	setup->nWindowed = 1;
	setup->nAlwaysOnTop = 0;
	
	// store res float values
	RES_X = (float)setup->scrWidth;
	RES_Y = (float)setup->scrHeight;
	
	unsigned int	PixelFormat;
	DWORD			dwExStyle, dwStyle;
	RECT			rec;
	WNDCLASSA		wc;

	setup->hInstance = GetModuleHandle( 0 );

	memset( &wc, 0, sizeof(WNDCLASSA) );

	wc.style         = CS_OWNDC;
	wc.lpfnWndProc   = WndProc;
	wc.hInstance     = setup->hInstance;
	wc.lpszClassName = wndclass;

	if( !RegisterClass((WNDCLASSA*)&wc) )
		return( 0 );

#ifdef ALLOWWINDOWED
	if( !setup->nWindowed )
#endif
	{
		//screenSettings.dmPelsWidth = info->screenInfo->width;
		//screenSettings.dmPelsHeight = info->screenInfo->height;
		//if( ChangeDisplaySettings(&screenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		//	return( 0 );
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle   = WS_VISIBLE | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		ShowCursor( 0 );
	}
#ifdef ALLOWWINDOWED
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_TOPMOST;
		dwStyle   = WS_VISIBLE | WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU;
	}
#endif
	rec.left   = 0;
	rec.top    = 0;
	rec.right  = 800;
	rec.bottom = 600;

#ifdef ALLOWWINDOWED
	AdjustWindowRect( &rec, dwStyle, 0 );
	setup->hWnd = CreateWindowEx( dwExStyle, wc.lpszClassName, wc.lpszClassName, dwStyle,
		(GetSystemMetrics(SM_CXSCREEN)-rec.right+rec.left)>>1,
		(GetSystemMetrics(SM_CYSCREEN)-rec.bottom+rec.top)>>1,
		rec.right-rec.left, rec.bottom-rec.top, 0, 0, setup->hInstance, 0 );
#else
	info->hWnd = CreateWindowEx( dwExStyle, wc.lpszClassName, wc.lpszClassName, dwStyle, 0, 0, 
		rec.right-rec.left, rec.bottom-rec.top, 0, 0, info->hInstance, 0 );
#endif

	if( !setup->hWnd )
		return( 0 );

	if( !(setup->hDC=GetDC(setup->hWnd)) )
		return( 0 );

	if( !(PixelFormat=ChoosePixelFormat(setup->hDC,&pfd)) )
		return( 0 );

	if( !SetPixelFormat(setup->hDC,PixelFormat,&pfd) )
		return( 0 );

	if( !(setup->hRC=wglCreateContext(setup->hDC)) )
		return( 0 );

	if( !wglMakeCurrent(setup->hDC,setup->hRC) )
		return( 0 );

	SetForegroundWindow(setup->hWnd);
	SetFocus(setup->hWnd);

	return( 1 );
}

void entrypoint( void )
{              
	MSG msg;

	window_init(&RES_SETTINGS);
	
	// init d3d - adding this call earlier causes a crash, not sure why.
	initDirect3d(RES_SETTINGS.hWnd);

	// init intro
	intro_init();

	// generate and play Clinkster tune
	Clinkster_GenerateMusic();
	Clinkster_StartMusic();

	// init/start audio
	InitRocket();

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
		
		while( PeekMessage(&msg,0,0,0,PM_REMOVE) )
		{
			if( msg.message==WM_QUIT ) 
			{
				ExitProcess(0);
			}
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

        intro_do( introTimeSecs );
    
        //SwapBuffers ( hDC );   
        wglSwapLayerBuffers(RES_SETTINGS.hDC, WGL_SWAP_MAIN_PLANE );

	}while ( !GetAsyncKeyState(VK_ESCAPE)/* && MMTime.u.sample < MAX_SAMPLES*/ );

    ExitProcess(0);
}

int WINAPI WinMain( HINSTANCE instance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	entrypoint();
}


