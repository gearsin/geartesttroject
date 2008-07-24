//------------------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "DXUTCamera.h"
#include "Object.h"
#include "Renderer.h"


//------------------------------------------------------------------------------------------------------------------
cRenderer * g_Renderer = NULL;
CFirstPersonCamera g_Camera;


//------------------------------------------------------------------------------------------------------------------
cRenderer::cRenderer()
{
	m_D3DRenderer = NULL;
}


//------------------------------------------------------------------------------------------------------------------
cRenderer::cRenderer( IDirect3DDevice9 * pD3Ddevice, unsigned int  pLineWidth )
{
	m_D3DRenderer = pD3Ddevice;
	CreateFont( pLineWidth );
}


//------------------------------------------------------------------------------------------------------------------
cRenderer::~cRenderer()
{
	if( m_Font )
	{
		SAFE_RELEASE( m_Font );
	}

	if( m_TextSprite )
	{
		SAFE_RELEASE( m_TextSprite );
	}

	if( m_TxtHelper )
	{
		delete m_TxtHelper;
		m_TxtHelper = NULL;
	}

if( m_TestObj )
{
	delete m_TestObj;
	m_TestObj = NULL;
}

	m_D3DRenderer = NULL;
}


//------------------------------------------------------------------------------------------------------------------
void cRenderer::RenderScene( )
{
	HRESULT hr;
    D3DXMATRIXA16 mView;
    D3DXMATRIXA16 mProj;
    D3DXMATRIXA16 mWorld;
    D3DXMATRIXA16 mWorldViewProjection;

	// Clear the render target and the zbuffer 
	V( m_D3DRenderer->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 255, 0, 0, 255), 1.0f, 0 ) );

	// Render the scene
    if( SUCCEEDED( m_D3DRenderer->BeginScene() ) )
    {
	        // Get the projection & view matrix from the camera class
		D3DXMatrixIdentity( &mWorld );
        mProj = *g_Camera.GetProjMatrix();
        mView = *g_Camera.GetViewMatrix();

        //mWorldViewProjection = g_Render.mCellWorld * mView * mProj;
        //mWorldView = g_Render.mCellWorld * mView;
		m_D3DRenderer->SetTransform( D3DTS_WORLD, &mWorld );
		m_D3DRenderer->SetTransform( D3DTS_VIEW, &mView );
		m_D3DRenderer->SetTransform( D3DTS_PROJECTION, &mProj );
		m_TestObj->Render();
	}

	RenderText();
	V( m_D3DRenderer->EndScene() );	
}


//------------------------------------------------------------------------------------------------------------------
HRESULT cRenderer::CreateFont( unsigned int pLineWidth  )
{
	HRESULT hr;
	// Initialize the font
    V_RETURN( D3DXCreateFont( m_D3DRenderer, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
                         OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
                         L"Arial", &m_Font ) );

	V_RETURN( D3DXCreateSprite( m_D3DRenderer, &m_TextSprite ) );

	m_TxtHelper = new CDXUTTextHelper( m_Font, m_TextSprite, pLineWidth );
	return hr;
}


//------------------------------------------------------------------------------------------------------------------
void cRenderer::PrintText( const WCHAR *pFormat, ... )
{
    wchar_t strBuffer[512];
    
    va_list args;
    va_start( args, pFormat );
    StringCchVPrintf( strBuffer, 512, pFormat, args );
    strBuffer[511] = L'\0';
    va_end(args);

	m_ScreenTextList.push_back( strBuffer );
}


//------------------------------------------------------------------------------------------------------------------
void cRenderer::RenderText()
{
	m_TxtHelper->Begin();
    m_TxtHelper->SetInsertionPos( 2, 1 );
    m_TxtHelper->SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
    m_TxtHelper->DrawTextLine( DXUTGetFrameStats(true) );
    m_TxtHelper->DrawTextLine( DXUTGetDeviceStats() );
	m_TxtHelper->DrawFormattedTextLine( L"Pos: %0.2f, %0.2f, %0.2f", g_Camera.GetEyePt()->x, g_Camera.GetEyePt()->y, g_Camera.GetEyePt()->z );
	//Print text
	for( tStringList::iterator it = m_ScreenTextList.begin(); it != m_ScreenTextList.end(); it++ )
	{
		std::wstring scrText = *it;
		m_TxtHelper->DrawTextLine( scrText.c_str() );
	}
	m_TxtHelper->End();

	m_ScreenTextList.clear();
}


//------------------------------------------------------------------------------------------------------------------
void cRenderer::UnitTestFunction()
{
#define GROUND_Y 15.0f // -GROUND_Y is the Y coordinate of the ground.
#define CAMERA_SIZE 0.2f // CAMERA_SIZE is used for clipping camera movement
#define GRAVITY 3.0f  // GRAVITY defines the magnitude of the downward force applied to ammos.

// MinBound and MaxBound are the bounding box representing the cell mesh.
const D3DXVECTOR3         g_MinBound( -160.0f, -GROUND_Y, -160.0f );
const D3DXVECTOR3         g_MaxBound( 160.0f, GROUND_Y, 160.0f );

    // Setup the camera
    D3DXVECTOR3 MinBound( g_MinBound.x + CAMERA_SIZE, g_MinBound.y + CAMERA_SIZE, g_MinBound.z + CAMERA_SIZE );
    D3DXVECTOR3 MaxBound( g_MaxBound.x - CAMERA_SIZE, g_MaxBound.y - CAMERA_SIZE, g_MaxBound.z - CAMERA_SIZE );
    g_Camera.SetClipToBoundary( true, &MinBound, &MaxBound );
    g_Camera.SetEnableYAxisMovement( true );
    g_Camera.SetRotateButtons( false, false, true );
    g_Camera.SetScalers( 0.001f, 4.0f );
    D3DXVECTOR3 vecEye(0.0f, -0 + 0.7f, 0.0f);
    D3DXVECTOR3 vecAt (0.0f, -0 + 0.7f, 1.0f);
    g_Camera.SetViewParams( &vecEye, &vecAt );

	m_TestObj = new cObject( OBJECTS_DIR L"EvilDrone_low.x" );
}
