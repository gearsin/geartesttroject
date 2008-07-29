//------------------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include <io.h>
#include "Game.h"


//------------------------------------------------------------------------------------------------------------------
// Called during device initialization, this code checks the device for some 
// minimum set of capabilities, and rejects those that don't pass by returning false.
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
// This callback function will be called immediately after the Direct3D device has 
// been destroyed, which generally happens as a result of application termination or 
// windowed/full screen toggles. Resources created in the OnCreateDevice callback 
// should be released here, which generally includes all D3DPOOL_MANAGED resources. 
void CALLBACK OnDestroyDevice( void * pUserContext )
{
	g_Game->Destroy();
}


//------------------------------------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has 
// entered a lost state and before IDirect3DDevice9::Reset is called. Resources created
// in the OnResetDevice callback should be released here, which generally includes all 
// D3DPOOL_DEFAULT resources. See the "Lost Devices" section of the documentation for 
// information about lost devices.
void CALLBACK OnLostDevice( void * pUserContext )
{

}


//------------------------------------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been 
// created, which will happen during application initialization and windowed/full screen 
// toggles. This is the best location to create D3DPOOL_MANAGED resources since these 
// resources need to be reloaded whenever the device is destroyed. Resources created  
// here should be released in the OnDestroyDevice callback. 
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9 * pDevice, const D3DSURFACE_DESC * pBackBufferSurfacceDesc, void * pUserContext )
{
	cGame::Create( pDevice );
	return S_OK;
}


//------------------------------------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been 
// reset, which will happen after a lost device scenario. This is the best location to 
// create D3DPOOL_DEFAULT resources since these resources need to be reloaded whenever 
// the device is lost. Resources created here should be released in the OnLostDevice 
// callback. 
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9 * pDevice, const D3DSURFACE_DESC * pBackBufferSurfaceDesc, void * pUserContext )
{
    //pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    //pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    //pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

    // Setup the camera's projection parameters
    float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
    g_Camera.SetProjParams( D3DX_PI/4, fAspectRatio, 0.1f, 1000.0f );

    if( DXUTIsWindowed() )
    {
        g_Camera.SetRotateButtons( 0, 0, true );
        g_Camera.SetResetCursorAfterMove( false );
    }
    else
    {
        g_Camera.SetRotateButtons( 0, 0, false, true );
        g_Camera.SetResetCursorAfterMove( true );
    }

	return S_OK;
}


//------------------------------------------------------------------------------------------------------------------
// This callback function is called immediately before a device is created to allow the 
// application to modify the device settings. The supplied pDeviceSettings parameter 
// contains the settings that the framework has selected for the new device, and the 
// application can make any desired changes directly to this structure.  Note however that 
// DXUT will not correct invalid device settings so care must be taken 
// to return valid device settings, otherwise IDirect3D9::CreateDevice() will fail.  
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

	//g_Game->ChangeDeviceSetting( );
	return true;
}


//------------------------------------------------------------------------------------------------------------------
void CALLBACK OnKeyPress( UINT pChar, bool pKeyDown, bool pAltDown, void * pUserContext )
{

}


//------------------------------------------------------------------------------------------------------------------
// This callback function will be called at the end of every frame to perform all the 
// rendering calls for the scene, and it will also be called if the window needs to be 
// repainted. After this function has returned, DXUT will call 
// IDirect3DDevice9::Present to display the contents of the next buffer in the swap chain
void CALLBACK OnFrameRender( IDirect3DDevice9 * pDevice, double pTime, float pElapsedTime, void * pUserContext )
{
	g_Game->Render();
}


//------------------------------------------------------------------------------------------------------------------
// This callback function will be called once at the beginning of every frame. This is the
// best location for your application to handle updates to the scene, but is not 
// intended to contain actual rendering calls, which should instead be placed in the 
// OnFrameRender callback.  
void CALLBACK OnUpdateGame( double pTime, float pElapsedTime, void * pUserContext )
{
	g_Game->Update();
}


//------------------------------------------------------------------------------------------------------------------
// Before handling window messages, DXUT passes incoming windows 
// messages to the application through this callback function. If the application sets 
// *pbNoFurtherProcessing to TRUE, then DXUT will not process this message.
LRESULT CALLBACK MsgProc( HWND pHwnd, UINT pMsg, WPARAM pWparam, LPARAM pLparam, bool * pNoFurtherProcessing, void * pUserContext )
{
	switch( pWparam )
	{
		case VK_F5:
			{
				//save screen shots
				unsigned int screen_shotCnt = 0;
				TCHAR screen_shotName[256];
				//search if ScreenShot is already there
				while( 1 )
				{
					if( screen_shotCnt < 10 )
					{
						swprintf( screen_shotName, L"screenshot0%d.png", screen_shotCnt );
					}
					else
					{
						swprintf( screen_shotName , L"screenshot%d.png", screen_shotCnt );
					}

					int file_exist = _waccess( screen_shotName, 0  );
					if( file_exist == -1 )
					{
						break;
					}
					screen_shotCnt++;
				}

				HRESULT hr = DXUTSnapD3D9Screenshot( screen_shotName );
				if( hr != S_OK )
				{
					Log ( " Unable To save screenshot" );
				}
			}
				 break;
		default:
				g_Camera.HandleMessages( pHwnd, pMsg, pWparam, pLparam );	
	}

	return S_OK;
}
