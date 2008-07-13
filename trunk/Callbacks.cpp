//------------------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "Renderer.h"

//------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// Called during device initialization, this code checks the device for some 
// minimum set of capabilities, and rejects those that don't pass by returning false.
//--------------------------------------------------------------------------------------
bool CALLBACK IsDeviceAcceptable( D3DCAPS9 * pCaps, D3DFORMAT pAdapterFormat, D3DFORMAT pBackBufferFormat, bool pWindowed, void * pUserContext )
{
	//Check for pixel shader 1.1
	if( pCaps->VertexShaderVersion < D3DVS_VERSION( 1, 1) )
	{
		return false;
	}

	if( pCaps->PixelShaderVersion < D3DPS_VERSION( 1, 1 ) )
	{
		return false;
	}

	return true;
}


//------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has 
// been destroyed, which generally happens as a result of application termination or 
// windowed/full screen toggles. Resources created in the OnCreateDevice callback 
// should be released here, which generally includes all D3DPOOL_MANAGED resources. 
//--------------------------------------------------------------------------------------
void CALLBACK OnDestroyDevice( void * pUserContext )
{
	if( g_Renderer )
	{
		delete g_Renderer;
		g_Renderer = NULL;
	}
}


//------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has 
// entered a lost state and before IDirect3DDevice9::Reset is called. Resources created
// in the OnResetDevice callback should be released here, which generally includes all 
// D3DPOOL_DEFAULT resources. See the "Lost Devices" section of the documentation for 
// information about lost devices.
//--------------------------------------------------------------------------------------
void CALLBACK OnLostDevice( void * pUserContext )
{

}


//------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been 
// created, which will happen during application initialization and windowed/full screen 
// toggles. This is the best location to create D3DPOOL_MANAGED resources since these 
// resources need to be reloaded whenever the device is destroyed. Resources created  
// here should be released in the OnDestroyDevice callback. 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9 * pDevice, const D3DSURFACE_DESC * pBackBufferSurfacceDesc, void * pUserContext )
{
    // Initialize the font
    //V_RETURN( D3DXCreateFont( pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
    //                     OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
    //                     L"Arial", &g_Render.pFont ) );

    //// Read the D3DX effect file
    //WCHAR str[MAX_PATH];
    //V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"Main.fx" ) );


	//Create renderer object, init the font too
	g_Renderer = new cRenderer( pDevice, 15);
	return S_OK;
}


//------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been 
// reset, which will happen after a lost device scenario. This is the best location to 
// create D3DPOOL_DEFAULT resources since these resources need to be reloaded whenever 
// the device is lost. Resources created here should be released in the OnLostDevice 
// callback. 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9 * pDevice, const D3DSURFACE_DESC * pBackBufferSurfaceDesc, void * pUserContext )
{
    //pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    //pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    //pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	//DXUTSetCursorSettings( (g_GameState.gameMode != GAME_RUNNING), true );
	return S_OK;
}


//------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// This callback function is called immediately before a device is created to allow the 
// application to modify the device settings. The supplied pDeviceSettings parameter 
// contains the settings that the framework has selected for the new device, and the 
// application can make any desired changes directly to this structure.  Note however that 
// DXUT will not correct invalid device settings so care must be taken 
// to return valid device settings, otherwise IDirect3D9::CreateDevice() will fail.  
//--------------------------------------------------------------------------------------
bool CALLBACK OnModifyDeviceSettings( DXUTDeviceSettings * pDeviceSettings, void * pUserContext )
{	
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    D3DCAPS9 caps;
    HRESULT hr;
    
    V( pD3D->GetDeviceCaps( pDeviceSettings->d3d9.AdapterOrdinal, pDeviceSettings->d3d9.DeviceType, &caps ) );

    // If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW then switch to SWVP.
    if( ( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 || caps.VertexShaderVersion < D3DVS_VERSION(1,1) )
    {
        pDeviceSettings->d3d9.BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }

	return true;
}


//------------------------------------------------------------------------------------------------------------------
void CALLBACK OnKeyPress( UINT pChar, bool pKeyDown, bool pAltDown, void * pUserContext )
{

}


//------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// This callback function will be called at the end of every frame to perform all the 
// rendering calls for the scene, and it will also be called if the window needs to be 
// repainted. After this function has returned, DXUT will call 
// IDirect3DDevice9::Present to display the contents of the next buffer in the swap chain
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameRender( IDirect3DDevice9 * pDevice, double pTime, float pElapsedTime, void * pUserContext )
{
g_Renderer->PrintText( L"Test Screen Text: %d", 100 );
	g_Renderer->RenderScene( pDevice );
}


//------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// This callback function will be called once at the beginning of every frame. This is the
// best location for your application to handle updates to the scene, but is not 
// intended to contain actual rendering calls, which should instead be placed in the 
// OnFrameRender callback.  
//--------------------------------------------------------------------------------------
void CALLBACK OnUpdateGame( double pTime, float pElapsedTime, void * pUserContext )
{

}


//------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// Before handling window messages, DXUT passes incoming windows 
// messages to the application through this callback function. If the application sets 
// *pbNoFurtherProcessing to TRUE, then DXUT will not process this message.
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND pHwnd, UINT pMsg, WPARAM pWparam, LPARAM pLparam, bool * pNoFurtherProcessing, void * pUserContext )
{
	return S_OK;
}
