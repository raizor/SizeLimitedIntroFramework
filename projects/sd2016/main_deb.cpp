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
#include "clinkster/player/clinkster.h"
#include "framework/debug/audio.h"
#include "framework/debug/scrubber.h"
#include "rocket/rocketControl.h"

#include <direct.h>
#define GetCurrentDir _getcwd

Scrubber* scrubber = NULL;

float introTimeSecs;
float introLength = 0;
bool paused;

WININFO wininfo = {  0,0,0,0,0,
{'i','q','_',0}, 
800, 600,
};

//----------------------------------------------------------------------------

static const PIXELFORMATDESCRIPTOR pfd =
    {
    sizeof(PIXELFORMATDESCRIPTOR),
    1,
    PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,
    PFD_TYPE_RGBA,
    32,
    0, 0, 0, 0, 0, 0, 8, 0,
    0, 0, 0, 0, 0,  // accum
    32,             // zbuffer
    0,              // stencil!
    0,              // aux
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
        }

	if( uMsg==WM_LBUTTONUP )
	{
		int x = (short)LOWORD(lParam);
		int y = (short)HIWORD(lParam);

#ifdef ENABLE_SCRUBBER
		scrubber->MouseUp();
#endif
	}

	if( uMsg==WM_LBUTTONDOWN )
	{
		int x = (short)LOWORD(lParam);
		int y = (short)HIWORD(lParam);

#ifdef ENABLE_SCRUBBER
		scrubber->MouseDown();
#endif
	}

	if( uMsg==WM_MOUSEMOVE )
		{
			int x = (short)LOWORD(lParam);
			int y = (short)HIWORD(lParam);

#ifdef ENABLE_SCRUBBER
			scrubber->MouseMove(x, y);
#endif
		}


    return( DefWindowProc(hWnd,uMsg,wParam,lParam) );
}

static void window_end( WININFO *info )
{
    if( info->hRC )
    {
        wglMakeCurrent( 0, 0 );
        wglDeleteContext( info->hRC );
    }

    if( info->hDC  ) ReleaseDC( info->hWnd, info->hDC );
    if( info->hWnd ) DestroyWindow( info->hWnd );

    UnregisterClass( info->wndclass, info->hInstance );

    if( info->full )
    {
        ChangeDisplaySettings( 0, 0 );
		ShowCursor( 1 );
    }
}

void SetNormalizedTime(float timeNormalized)
{
	introTimeSecs = introLength * timeNormalized;
	audioSetNormalizedPos(timeNormalized);
}

void TogglePause(bool pause)
{
	paused = pause;
	bass_playPause(paused);
}

static int window_init( WININFO *info )
{
	unsigned int	PixelFormat;
    DWORD			dwExStyle, dwStyle;
    DEVMODE			dmScreenSettings;
    RECT			rec;

    WNDCLASS		wc;

    ZeroMemory( &wc, sizeof(WNDCLASS) );
    wc.style         = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = info->hInstance;
    wc.lpszClassName = info->wndclass;
	
    if( !RegisterClass(&wc) )
        return( 0 );

    if( info->full )
    {
        dmScreenSettings.dmSize       = sizeof(DEVMODE);
        dmScreenSettings.dmFields     = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
        dmScreenSettings.dmBitsPerPel = 32;
        dmScreenSettings.dmPelsWidth  = wininfo.width;
        dmScreenSettings.dmPelsHeight = wininfo.height;
        if( ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
            return( 0 );
        dwExStyle = WS_EX_APPWINDOW;
        dwStyle   = WS_VISIBLE | WS_POPUP;// | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		ShowCursor( 0 );
    }
    else
    {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        dwStyle   = WS_VISIBLE | WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU;
    }

    rec.left   = 0;
    rec.top    = 0;
    rec.right  = wininfo.width;
    rec.bottom = wininfo.height;
    AdjustWindowRect( &rec, dwStyle, 0 );

    info->hWnd = CreateWindowEx( dwExStyle, wc.lpszClassName, INTRO_TITLE, dwStyle,
                               (GetSystemMetrics(SM_CXSCREEN)-rec.right+rec.left)>>1,
                               (GetSystemMetrics(SM_CYSCREEN)-rec.bottom+rec.top)>>1,
                               rec.right-rec.left, rec.bottom-rec.top, 0, 0, info->hInstance, 0 );
    if( !info->hWnd )
        return( 0 );

    if( !(info->hDC=GetDC(info->hWnd)) )
        return( 0 );

    if( !(PixelFormat=ChoosePixelFormat(info->hDC,&pfd)) )
        return( 0 );

    if( !SetPixelFormat(info->hDC,PixelFormat,&pfd) )
        return( 0 );

    if( !(info->hRC=wglCreateContext(info->hDC)) )
        return( 0 );

    if( !wglMakeCurrent(info->hDC,info->hRC) )
        return( 0 );
    
    return( 1 );
}

void RenderAudioToFile()
{
	HMODULE hModule = GetModuleHandleW(NULL);
	char path[MAX_PATH];
	GetModuleFileName(hModule, path, MAX_PATH);
	PathRemoveFileSpecA(path);
	sprintf(path, "%s\\%s", path, "music.dat");

	// render music to file?
	// assumes audio data follows a 44 byte wav header.

	if( !PathFileExists(path) )
	{
		// generate and play Clinkster tune
		Clinkster_GenerateMusic();
		//Clinkster_StartMusic();

		FILE* fileWaveData = fopen(path, "wb");
		// write header data
		fwrite(&Clinkster_MusicLength, sizeof(Clinkster_MusicLength), 1, fileWaveData); // uint
		fwrite(&Clinkster_TicksPerSecond, sizeof(Clinkster_TicksPerSecond), 1, fileWaveData); // float

		fwrite(&Clinkster_WavFileHeader, sizeof(Clinkster_WavFileHeader), 1, fileWaveData);
		fwrite(&Clinkster_MusicBuffer, Clinkster_WavFileHeader[10], 1, fileWaveData);

		fclose(fileWaveData);			
	}
}

//==============================================================================================

int WINAPI WinMain( HINSTANCE instance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
    MSG         msg;
    int         done=0;
    WININFO     *info = &wininfo;
	
	paused = false;
	
    info->hInstance = GetModuleHandle( 0 );

    //if( MessageBox( 0, "fullscreen?", info->wndclass, MB_YESNO|MB_ICONQUESTION)==IDYES ) info->full++;

    if( !window_init(info) )
    {
        window_end( info );
        MessageBox( 0, "window_init()!","error",MB_OK|MB_ICONEXCLAMATION );
        return( 0 );
    }


	scrubber = new Scrubber(info->hWnd);
	scrubber->SetWindowSize(wininfo.width, wininfo.height);

	long to=timeGetTime();
	
	RenderAudioToFile();

	// init/start audio
	InitRocket();
	
	openAudioFileStream();
	introLength = musicLengthSecs;
	
	// init intro
	intro_init();

	// start audio
	bass_playPause(0);
	
	while( !done )
	{
		// get sample position for timing
		//waveOutGetPosition(hWaveOut, &MMTime, sizeof(MMTIME));
		
		float pos = Clinkster_GetPosition();
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

		scrubber->Draw(musicPosNormalized);

        //if( MMTime.u.sample >= MAX_SAMPLES ) done = 1;
        SwapBuffers( info->hDC );
	}

    window_end( info );

    return( 0 );
}



