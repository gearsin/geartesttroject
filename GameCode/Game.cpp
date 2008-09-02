//------------------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "Game.h"
#include "Level.h"
#include "Renderer.h"
#include "GameInput.h"


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
void cGame::Create( )
{
	g_Game = new cGame();

	//Create renderer object, init the font too
	cRenderer::Create();

	//create level
	cLevel::Create( L"Test/multitest01.XML" );
	cGameInput::Create();
	g_Renderer->SetCamera( g_Level->GetCurrentCamera() );
}



//------------------------------------------------------------------------------------------------------------------
void cGame::Destroy()
{
	//TODO:: CALL the destroy for all objects e.g level and render should handle releasing the resources
	//cleanup level stuff
	cLevel::Destroy();

	//clean up renderer
	cRenderer::Destroy();
	cGameInput::Destroy();

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
	g_GameInput->Update();
	g_Level->Update();
}


//------------------------------------------------------------------------------------------------------------------
void cGame::Render()
{
	g_Renderer->RenderScene( );
}
