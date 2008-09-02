//------------------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "Callbacks.h"
#include "Game.h"


//------------------------------------------------------------------------------------------------------------------
INT WINAPI wWinMain( HINSTANCE hInstance,HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd )
{
    HRESULT hr = S_OK;

	//Register callbacks::Direct3D9
	DXUTSetCallbackD3D9DeviceAcceptable( IsDeviceAcceptable );
	DXUTSetCallbackD3D9DeviceCreated( OnCreateDevice );
	DXUTSetCallbackD3D9DeviceDestroyed( OnDestroyDevice );
	DXUTSetCallbackD3D9DeviceReset( OnResetDevice );
	DXUTSetCallbackD3D9DeviceLost( OnLostDevice );
	DXUTSetCallbackFrameMove( OnUpdateGame );				//scene update
	DXUTSetCallbackD3D9FrameRender( OnFrameRender );
	DXUTSetCallbackMsgProc( MsgProc );
	DXUTSetCallbackKeyboard( OnKeyPress );
	DXUTSetCallbackDeviceChanging( OnModifyDeviceSettings );

	//Set cursor settings :: Hide in full screen mode 
	DXUTSetCursorSettings( false, true );

	//initalize Directx Utility
	// Parse the command line, handle the default hotkeys, and show msgboxes
	DXUTInit( true, true );
	DXUTCreateWindow( L"KillTheGerms", hInstance );
	DXUTCreateDevice( true, 800, 600 );

	//create the game object
	cGame::Create();

	//run the main game loop
	DXUTMainLoop();

	cGame::Destroy();
   // TODO: Perform any application-level cleanup here
    return DXUTGetExitCode();
}