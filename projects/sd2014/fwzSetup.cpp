#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include "fwzSetup.h"
#include "resource.h"

fwzSettings * setupcfg;

typedef struct {
  int w,h;
} RES;

// if your windows supports more than 4000 reolutions, well, we're screwed :)

#define MAXNUMRES 4096

int nRes = 0;
RES ress[MAXNUMRES];

BOOL CALLBACK DlgFunc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
  lParam;
  switch ( uMsg ) {
    case WM_INITDIALOG: {
      char s[500];
      int i=0;

      DEVMODE d;
      EnumDisplaySettings(NULL,0,&d);
      while(1) {
        BOOL h = EnumDisplaySettings(NULL,i++,&d);
        if (!h || nRes>MAXNUMRES) break;
        
/*** You can use this following line to select only certain aspect ratios ***/
//        if ((d.dmPelsWidth * 3) / 4 != d.dmPelsHeight) continue;

/*** You can use this following line to avoid "rotated" pictures on e.g. laptops ***/
//        if (d.dmDisplayOrientation != DMDO_DEFAULT) continue;

        if (d.dmBitsPerPel != setupcfg->scrBPP) continue;
        if (!nRes || ress[nRes-1].w != d.dmPelsWidth || ress[nRes-1].h != d.dmPelsHeight) {
          ress[nRes].w = d.dmPelsWidth;
          ress[nRes].h = d.dmPelsHeight;
          nRes++;
          //vsnprintf(s,500,"%d * %d",d.dmPelsWidth,d.dmPelsHeight);
		  wsprintf(s, "%d * %d",d.dmPelsWidth,d.dmPelsHeight);
          SendDlgItemMessage(hWnd, IDC_RESOLUTION, CB_ADDSTRING, 0, (LPARAM)s);
        }
      }
      
      for (i=0; i<nRes; i++)
        if (ress[i].w==setupcfg->scrWidth && ress[i].h==setupcfg->scrHeight)
          SendDlgItemMessage(hWnd, IDC_RESOLUTION, CB_SETCURSEL, i, 0);

      if (!setupcfg->nWindowed) {
        SendDlgItemMessage(hWnd, IDC_FULLSCREEN, BM_SETCHECK, 1, 1);
        EnableWindow(GetDlgItem(hWnd,IDC_ONTOP),0);
      }
      if (setupcfg->nVsync)
        SendDlgItemMessage(hWnd, IDC_VSYNC, BM_SETCHECK, 1, 1);
      if (setupcfg->nMultisample)
        SendDlgItemMessage(hWnd, IDC_MULTI, BM_SETCHECK, 1, 1);

    } break;
    case WM_COMMAND:
    switch( LOWORD(wParam) ) {
      case IDOK: {
        setupcfg->scrWidth = ress[SendDlgItemMessage(hWnd, IDC_RESOLUTION, CB_GETCURSEL, 0, 0)].w;
        setupcfg->scrHeight= ress[SendDlgItemMessage(hWnd, IDC_RESOLUTION, CB_GETCURSEL, 0, 0)].h;
        setupcfg->nWindowed    =!SendDlgItemMessage(hWnd, IDC_FULLSCREEN, BM_GETCHECK , 0, 0);
        setupcfg->nAlwaysOnTop = SendDlgItemMessage(hWnd, IDC_ONTOP,      BM_GETCHECK , 0, 0);
        setupcfg->nVsync       = SendDlgItemMessage(hWnd, IDC_VSYNC,      BM_GETCHECK , 0, 0);
        setupcfg->nMultisample = SendDlgItemMessage(hWnd, IDC_MULTI,      BM_GETCHECK, 0, 0);
        EndDialog (hWnd, TRUE);
      } break;
      case IDCANCEL: {
        EndDialog (hWnd, FALSE);
      } break;
      case IDC_FULLSCREEN: {
        /* cake. */
        if (SendDlgItemMessage(hWnd, IDC_FULLSCREEN, BM_GETCHECK , 0, 0)) {
          SendDlgItemMessage(hWnd, IDC_ONTOP, BM_SETCHECK, 0, 0);
          EnableWindow(GetDlgItem(hWnd,IDC_ONTOP),0);
        } else {
          EnableWindow(GetDlgItem(hWnd,IDC_ONTOP),1);
        }
      } break;
    } break;
  }
  return ( WM_INITDIALOG == uMsg ) ? TRUE : FALSE;
}

/*
int OpenSetupDialog(fwzSettings * s) {
  setupcfg = s;
  return DialogBox(s->hInstance, MAKEINTRESOURCE(IDD_SETUP),GetForegroundWindow(),DlgFunc);
}*/


INT_PTR CALLBACK Debug_DlgProc (
	HWND   hwnd,
	UINT   uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		{
		}
		break;

	case WM_COMMAND:
		{
			UINT wId = LOWORD(wParam);
			if (wId == IDOK || wId == IDCANCEL)
			{
				EndDialog (hwnd, wId);
			}
		}
		break;

	case WM_CLOSE:
		EndDialog(hwnd, IDCANCEL);
		break;
	}

	return FALSE;
}

#define DLGTITLE  L"eventide - merry xmas dbf"
#define DLGFONT   L"Tahoma"
#define DLGRUN  L"&Apply"
#define DLGCANCEL L"&Cancel"
#define NUMCHARS(aa) (sizeof(aa)/sizeof((aa)[0]))
#define IDC_BITMAP 99

#pragma pack(push, 4)                 
static struct { // dltt 

	DWORD  style; 
	DWORD  dwExtendedStyle; 
	WORD   ccontrols; 
	short  x; 
	short  y; 
	short  cx; 
	short  cy; 
	WORD   menu;         // name or ordinal of a menu resource
	WORD   windowClass;  // name or ordinal of a window class
	WCHAR  wszTitle[NUMCHARS(DLGTITLE)]; // title string of the dialog box
	short  pointsize;       // only if DS_SETFONT flag is set
	WCHAR  wszFont[NUMCHARS(DLGFONT)];   // typeface name, if DS_SETFONT is set

	// control info
	//
	struct {
		DWORD  style; 
		DWORD  exStyle; 
		short  x; 
		short  y; 
		short  cx; 
		short  cy; 
		WORD   id; 
		WORD   sysClass;       // 0xFFFF identifies a system window class
		WORD   idClass;        // ordinal of a system window class
		WCHAR  wszTitle[NUMCHARS(DLGRUN)];
		WORD   cbCreationData; // bytes of following creation data
		//       WORD   wAlign;         // align next control to DWORD boundry.
	} apply;

	struct {
		DWORD  style; 
		DWORD  exStyle; 
		short  x; 
		short  y; 
		short  cx; 
		short  cy; 
		WORD   id; 
		WORD   sysClass;       // 0xFFFF identifies a system window class
		WORD   idClass;        // ordinal of a system window class
		WCHAR  wszTitle[NUMCHARS(DLGCANCEL)];
		WORD   cbCreationData; // bytes of following creation data
	} cancel;

	struct {
		DWORD  style; 
		DWORD  exStyle; 
		short  x; 
		short  y; 
		short  cx; 
		short  cy; 
		WORD   id; 
		WORD   sysClass;       // 0xFFFF identifies a system window class
		WORD   idClass;        // ordinal of a system window class
		WCHAR  wszTitle[1];    // title string or ordinal of a resource
		WORD   cbCreationData; // bytes of following creation data
	} bitmap;

} g_DebugDlgTemplate = {

	WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU  // style  0x94c800c4
	| DS_MODALFRAME | DS_3DLOOK 
	| DS_SETFONT,
	0x0,        // exStyle;
	3,          // ccontrols
	0, 0, 186, 111,
	0,                       // menu: none
	0,                       // window class: none
	DLGTITLE,                // Window caption
	8,                       // font pointsize
	DLGFONT,

	{
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_GROUP | BS_DEFPUSHBUTTON,   // 0x50030001
			WS_EX_NOPARENTNOTIFY, // 0x4
			7,90,50,14,
			IDOK,
			0xFFFF, 0x0080, // button
			DLGRUN, 0,
	},

	{
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,    // 0x50010000
			WS_EX_NOPARENTNOTIFY, // 0x4
			128,90,50,14,
			IDCANCEL,
			0xFFFF, 0x0080, // button
			DLGCANCEL, 0,
		},

		{
			WS_CHILD | WS_VISIBLE | WS_GROUP | SS_LEFT,    // 0x50020000
				WS_EX_NOPARENTNOTIFY, // 0x4
				6,6,288,8,
				IDC_BITMAP,
				0xFFFF, 0x0082, // static
				L"", 0,
		},
};

#pragma pack(pop)

int OpenSetupDialog(fwzSettings * s)
{
	return DialogBoxIndirectParamW (s->hInstance, (LPCDLGTEMPLATEW)&g_DebugDlgTemplate, s->hWnd,
		Debug_DlgProc, (LPARAM)NULL);
}


int OpenSetupDialogx(fwzSettings * s) {
  setupcfg = s;
  return DialogBox(s->hInstance, MAKEINTRESOURCE(IDD_SETUP),GetForegroundWindow(),DlgFunc);
}