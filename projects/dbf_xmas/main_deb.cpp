//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008                                        //
//--------------------------------------------------------------------------//

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <GL/gl.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <shlwapi.h>
#include "config.h"
#include "intro.h"
#include "clinkster.h"
#include "framework/debug/audio.h"
#include "framework/debug/scrubber.h"
#include "../rocket/rocketControl.h"
#include "framework/math/vector.h"
#include "framework/dialog/fwzSetup.h"
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include <direct.h>
#include <direct3d.h>
#define GetCurrentDir _getcwd

Scrubber* scrubber = NULL;

bool addPoint = false;

float introTimeSecs;
float introLength = 0;
bool paused;

int MOUSE_X;
int MOUSE_Y;
bool MOUSE_DOWN;

ZVector eye;
ZVector look;
ZVector cube;
ZVector objectPos;

HDC hDC;

extern void ReloadShaders();

static const char wndclass[] = "Merry Xmas DBF - Eventide";

//----------------------------------------------------------------------------

static const PIXELFORMATDESCRIPTOR pfd =
{
	sizeof(PIXELFORMATDESCRIPTOR),
	1,
	PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,
	PFD_TYPE_RGBA,
	32,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	32,             // zbuffer
	0,              // stencil!
	0,
	PFD_MAIN_PLANE,
	0, 0, 0, 0
};

//==============================================================================================


static LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// salvapantallas
	if( uMsg==WM_SYSCOMMAND && (wParam==SC_SCREENSAVE || wParam==SC_MONITORPOWER) )
		return( 0 );

	// boton x o pulsacion de escape
	if( uMsg==WM_CLOSE || uMsg==WM_DESTROY || (uMsg==WM_KEYDOWN && wParam==VK_ESCAPE) )
		{
		PostQuitMessage(0);
        return( 0 );
		}

    if( uMsg==WM_CHAR )
        {
			if( wParam==VK_ESCAPE )
            {
				PostQuitMessage(0);
				return( 0 );
            }

			if( wParam=='r' )
			{
				ReloadShaders();
			}

			if (wParam == 'z')
				introTimeSecs-=0.01f;

			if (wParam == 'x')
				introTimeSecs-=0.001f;

			if (wParam == 'c')
				introTimeSecs+=0.001f;

			if (wParam == 'v')
				introTimeSecs+=0.01f;



			if (wParam == 'q')
				introTimeSecs+=0.01f;
			
			if (wParam == 'a')
				introTimeSecs+=0.01f;


			if (wParam == 'p')
				addPoint = true;
			
        }

	if( uMsg==WM_LBUTTONUP )
	{
		int x = (short)LOWORD(lParam);
		int y = (short)HIWORD(lParam);

		MOUSE_DOWN = false;		

#ifdef ENABLE_SCRUBBER
		scrubber->MouseUp();
#endif
	}

	if( uMsg==WM_LBUTTONDOWN )
	{
		int x = (short)LOWORD(lParam);
		int y = (short)HIWORD(lParam);

		MOUSE_DOWN = true;

#ifdef ENABLE_SCRUBBER
		scrubber->MouseDown();
#endif
	}

	if( uMsg==WM_MOUSEMOVE )
		{
			int x = (short)LOWORD(lParam);
			int y = (short)HIWORD(lParam);
			
			MOUSE_X = x;
			MOUSE_Y = y;

#ifdef ENABLE_SCRUBBER
			scrubber->MouseMove(x, y);
#endif
		}

    return( DefWindowProc(hWnd,uMsg,wParam,lParam) );
}

static void window_end( fwzSettings *setup )
{
    if( setup->hRC )
    {
        wglMakeCurrent( 0, 0 );
        wglDeleteContext( setup->hRC );
    }

    if( setup->hDC  ) ReleaseDC( setup->hWnd, setup->hDC );
    if( setup->hWnd ) DestroyWindow( setup->hWnd );

    UnregisterClass( wndclass, setup->hInstance );

    if( !setup->nWindowed )
    {
        ChangeDisplaySettings( 0, 0 );
		ShowCursor( 1 );
    }
}

void SetNormalizedTime(float timeNormalized)
{
	//if (abs(introTimeSecs - (introLength * timeNormalized)) > 0.001f)
	//{
		introTimeSecs = introLength * timeNormalized;
		audioSetNormalizedPos(timeNormalized);
	//}
}

void TogglePause(bool pause)
{
	paused = pause;
	SetNormalizedTime(musicPosNormalized);
	bass_playPause(paused);
}

static int window_init(fwzSettings* setup)
{
	unsigned int	PixelFormat;
	DWORD			dwExStyle, dwStyle;
	DEVMODE			dmScreenSettings;
	RECT			rec;

	WNDCLASS		wc;

	// You HAVE to initialize this before trying to open a dialog!  
	//#ifdef _CONSOLE
	setup->hInstance = GetModuleHandle(NULL);
	//#else
	//setup.hInstance = hInstance;
	//#endif

	// These are just some default settings I found convenient to work with.
	// If you modify them, the setupbox will initialize accordingly.
	setup->scrBPP = 32;
	setup->nVsync = 1;
	setup->nMultisample = 0;
	
	USE_RES_SELECTOR = INTRO_DEBUG_USE_SETUP_DIALOG;
		
	if (USE_RES_SELECTOR){
		
		if (!OpenSetupDialog(setup)){
			ExitProcess(0);
		}
	}
	else
	{
		setup->scrWidth = 1280;
		setup->scrHeight = 720;
		setup->nWindowed = 1;
		setup->nAlwaysOnTop = 0;
	}

	// store res float values
	RES_X = (float)setup->scrWidth;
	RES_Y = (float)setup->scrHeight;

    ZeroMemory( &wc, sizeof(WNDCLASS) );
    wc.style         = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = setup->hInstance;
    wc.lpszClassName = wndclass;
	
    if( !RegisterClass(&wc) )
        return( 0 );

	DWORD exStyle = WS_EX_APPWINDOW;
	if (setup->nAlwaysOnTop)
		exStyle |= WS_EX_TOPMOST;

    if( !setup->nWindowed )
    {
        dmScreenSettings.dmSize       = sizeof(DEVMODE);
        dmScreenSettings.dmFields     = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
        dmScreenSettings.dmBitsPerPel = 32;
        dmScreenSettings.dmPelsWidth  = setup->scrWidth;
        dmScreenSettings.dmPelsHeight = setup->scrHeight;
        if( ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
            return( 0 );
        dwExStyle = exStyle;
        dwStyle   = WS_VISIBLE | WS_POPUP;// | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		ShowCursor( 0 );
    }
    else
    {
        dwExStyle = exStyle;
        dwStyle   = WS_VISIBLE | WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU;
    }

    rec.left   = 0;
    rec.top    = 0;
    rec.right  = setup->scrWidth;
    rec.bottom = setup->scrHeight;
    AdjustWindowRect( &rec, dwStyle, 0 );

    setup->hWnd = CreateWindowEx( dwExStyle, wc.lpszClassName, introTitle, dwStyle,
                               (GetSystemMetrics(SM_CXSCREEN)-rec.right+rec.left)>>1,
                               (GetSystemMetrics(SM_CYSCREEN)-rec.bottom+rec.top)>>1,
                               rec.right-rec.left, rec.bottom-rec.top, 0, 0, setup->hInstance, 0 );
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

	hDC = setup->hDC;

	SetForegroundWindow(setup->hWnd);
    	
    return( 1 );
}

void RenderAudioToFile()
{
	HMODULE hModule = GetModuleHandleW(NULL);
	char path[MAX_PATH];
	char pathAudio[MAX_PATH];
	char pathEvents[MAX_PATH];
	GetModuleFileName(hModule, path, MAX_PATH);
	PathRemoveFileSpecA(path);

	sprintf(pathAudio, "%s\\%s", path, "music.dat");
	sprintf(pathEvents, "%s\\%s", path, "musicEvents.dat");

	if( !PathFileExists(pathAudio) || !PathFileExists(pathEvents))
	{
		// generate Clinkster tune
		Clinkster_GenerateMusic();
	}

	// render music to file?
	// assumes audio data follows a 44 byte wav header.

	if( !PathFileExists(pathAudio) )
	{
		FILE* fileWaveData = fopen(pathAudio, "wb");
		// write header data
		fwrite(&Clinkster_MusicLength, sizeof(Clinkster_MusicLength), 1, fileWaveData); // uint
		fwrite(&Clinkster_TicksPerSecond, sizeof(Clinkster_TicksPerSecond), 1, fileWaveData); // float

		fwrite(&Clinkster_WavFileHeader, sizeof(Clinkster_WavFileHeader), 1, fileWaveData);
		fwrite(&Clinkster_MusicBuffer, Clinkster_WavFileHeader[10], 1, fileWaveData);

		fclose(fileWaveData);			
	}

	// render the event data?
	if( !PathFileExists(pathEvents) )
	{
		FILE* fileEventData = fopen(pathEvents, "wb");
		fwrite(&Clinkster_NoteTiming, sizeof(unsigned int) * Clinkster_NumTracks * Clinkster_MusicLength, 1, fileEventData); // uint - number of tracks in clinkster file						
		fclose(fileEventData);			
	}
}

void LoadMusicEventData()
{
	HMODULE hModule = GetModuleHandleW(NULL);
	char path[MAX_PATH];
	GetModuleFileName(hModule, path, MAX_PATH);
	PathRemoveFileSpecA(path);
	sprintf(path, "%s\\%s", path, "musicEvents.dat");

	FILE* fileWaveData = fopen(path, "rb");		
	fseek(fileWaveData, sizeof(Clinkster_MusicLength) + sizeof(Clinkster_TicksPerSecond) + sizeof(Clinkster_WavFileHeader), SEEK_SET);
	//Clinkster_NumTracks * Clinkster_MusicLength Clinkster_NoteTiming = 
	fread(&Clinkster_NoteTiming, sizeof(unsigned int) * Clinkster_NumTracks * Clinkster_MusicLength, 1, fileWaveData);
}


static void DrawTime( fwzSettings *info, float t )
{
	static int      frame=0;
	static float    to=0.0;
	static int      fps=0;
	char            str[64];
	int             s, m, c;

	if( t<0.0f) return;
	//if( info->screenInfo->fullscreen ) return;

	frame++;
	if( (t-to)>1.0f )
	{
		fps = frame;
		to = t;
		frame = 0;
	}

	if( !(frame&3) )
	{
		m = (int)floorf( t/60.0f );
		s = (int)floorf( t-60.0f*(float)m );
		c = (int)floorf( t*100.0f ) % 100;
		
		sprintf( str, "%02d:%02d:%02d  [%d fps]", m, s, c, fps);
		SetWindowText( info->hWnd, str );
	}
}

//==============================================================================================

int WINAPI WinMain( HINSTANCE instance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
    MSG         msg;
    int         done=0;
	
	paused = false;
	
    RES_SETTINGS.hInstance = GetModuleHandle( 0 );

    //if( MessageBox( 0, "fullscreen?", info->wndclass, MB_YESNO|MB_ICONQUESTION)==IDYES ) info->full++;

    if( !window_init(&RES_SETTINGS) )
    {
        window_end( &RES_SETTINGS);
        MessageBox( 0, "window_init()!","error",MB_OK|MB_ICONEXCLAMATION );
        return( 0 );
    }
	
	scrubber = new Scrubber(RES_SETTINGS.hWnd);
	scrubber->SetWindowSize(RES_SETTINGS.scrWidth, RES_SETTINGS.scrHeight);

	long to=timeGetTime();
	
	RenderAudioToFile();

	LoadMusicEventData();

	// init/start audio
	InitRocket();
	
	openAudioFileStream();
	introLength = musicLengthSecs;

	// init d3d - adding this call earlier causes a crash, not sure why.
	initDirect3d(RES_SETTINGS.hWnd);
	
	// init intro
	intro_init();


#ifndef MUTE_AUDIO
	// start audio
	bass_playPause(0);
#endif
			
	while( !done )
	{
		// get sample position for timing
		//waveOutGetPosition(hWaveOut, &MMTime, sizeof(MMTIME));
		
		//float pos = Clinkster_GetPosition();
				//if (pos > Clinkster_MusicLength) break;

		long nowMs = timeGetTime();
		if (!paused)
		{			
			long elapsedMs = nowMs - to;
			introTimeSecs += (float)elapsedMs * 0.001f;
		}	
		introTimeSecs = min(introTimeSecs, musicLengthSecs);
		musicPosNormalized = introTimeSecs / (float)musicLengthSecs;			
		to = nowMs;
		
		float pos = Clinkster_TicksPerSecond * introTimeSecs;
		
        while( PeekMessage(&msg,0,0,0,PM_REMOVE) )
        {
			for (unsigned int i = 0 ; i < Clinkster_NumTracks ; i++)
			{
				float t = Clinkster_GetInstrumentTrigger(i, pos);
				int ti = (int)(t * 0.25);
				if (ti > 3) ti = 3;
				//printf("%c",  "|:. "[ti]);
			}

            if( msg.message==WM_QUIT ) done=1;
		    TranslateMessage( &msg );
            DispatchMessage( &msg );
        }

        intro_do( introTimeSecs );

		DrawTime(&RES_SETTINGS, introTimeSecs);
		
		scrubber->Draw(musicPosNormalized);

        //if( MMTime.u.sample >= MAX_SAMPLES ) done = 1;
        SwapBuffers(RES_SETTINGS.hDC );
	}

    window_end( &RES_SETTINGS);

    return( 0 );
}



