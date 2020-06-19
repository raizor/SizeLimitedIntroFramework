#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include "config.h"
#include "direct3d.h"

#include "framework/zassert.h"

IDirect3D9 *d3dInterface = NULL;
IDirect3DDevice9 *d3dDevice = NULL;

static D3DPRESENT_PARAMETERS devParams = 
{
	0, 0,   // xres, yres
	D3DFMT_A8R8G8B8, 
	0,      // backBufferCount;
	D3DMULTISAMPLE_4_SAMPLES, 
	0,      // multiSampleQuality;
	D3DSWAPEFFECT_DISCARD, 
	0,      // hDeviceWindow;
	true,   // windowed
	true,
	D3DFMT_D24S8,
	0,  // flags;
	0,  // fullScreen_RefreshRateInHz;
	D3DPRESENT_INTERVAL_IMMEDIATE   // presentationInterval
};

// create d3d context
// reference d3dDevice object when done to use context.
void initDirect3d(HWND hWnd)
{	
	d3dInterface = Direct3DCreate9( D3D_SDK_VERSION );
	ZASSERT( d3dInterface != 0 );

	devParams.hDeviceWindow = hWnd;
	HRESULT hResult = d3dInterface->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, devParams.hDeviceWindow,
		// D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_FPU_PRESERVE, &devParams, &d3dDevice );
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &devParams, &d3dDevice );

	ZASSERT( SUCCEEDED( hResult ) );
}
