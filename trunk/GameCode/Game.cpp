//------------------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "Game.h"
#include "Level.h"
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

	//Create renderer object, init the font too
	g_Renderer = new cRenderer( pD3Ddevice, 15 );

	//create level
	cLevel::Create( L"Test/multitest01.XML" );

	g_Renderer->SetCamera( g_Level->GetCamera() );
}



//------------------------------------------------------------------------------------------------------------------
void cGame::Destroy()
{
	//cleanup level stuff
	if( g_Level )
	{
		delete g_Level;
	}

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
	g_Level->Update();
}


//------------------------------------------------------------------------------------------------------------------
void cGame::Render()
{
	g_Renderer->RenderScene( );
}
