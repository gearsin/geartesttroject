//------------------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "Game.h"
#include "Renderer.h"


//------------------------------------------------------------------------------------------------------------------
cGame * g_Game = NULL;


//------------------------------------------------------------------------------------------------------------------
cGame::cGame()
{
	g_Game = this;
}


//------------------------------------------------------------------------------------------------------------------
cGame::~cGame()
{
}


//------------------------------------------------------------------------------------------------------------------
void cGame::Create( IDirect3DDevice9 *pD3Ddevice )
{
	g_Game = new cGame();

    // Initialize the font
    //V_RETURN( D3DXCreateFont( pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
    //                     OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
    //                     L"Arial", &g_Render.pFont ) );

    //// Read the D3DX effect file
    //WCHAR str[MAX_PATH];
    //V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"Main.fx" ) );

	//Create renderer object, init the font too
	g_Renderer = new cRenderer( pD3Ddevice, 15 );
	g_Renderer->UnitTestFunction();
//	cVector Test;
//	Test.UnitTestFunction();

}



//------------------------------------------------------------------------------------------------------------------
void cGame::Destroy()
{
	//clean up renderer
	if( g_Renderer )
	{
		delete g_Renderer;
		g_Renderer = NULL;
	}

	//cleanup Game
	if ( g_Game )
	{
		delete g_Game;
		g_Game = NULL;
	}
}


//------------------------------------------------------------------------------------------------------------------
void cGame::Update()
{
	g_Camera.FrameMove( 0.005f ); // Place holder there is not update for renderer.
}


//------------------------------------------------------------------------------------------------------------------
void cGame::Render()
{
	static int Count = 0;
g_Renderer->PrintText( L"Test Screen Text: %d", Count++ );
	g_Renderer->RenderScene( );

}
