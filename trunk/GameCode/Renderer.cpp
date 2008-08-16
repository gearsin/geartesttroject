//------------------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "Object.h"
#include "Level.h"
#include "Frustum.h"
#include "Renderer.h"


//------------------------------------------------------------------------------------------------------------------
cRenderer * g_Renderer = NULL;


//------------------------------------------------------------------------------------------------------------------
cRenderer::cRenderer( IDirect3DDevice9 * pD3Ddevice, unsigned int  pLineWidth )
{
	m_D3DRenderer = pD3Ddevice;
	m_Camera = NULL;
	m_ObjectsVisible = 0;
	m_D3DRenderer->LightEnable( 0, TRUE );
	m_D3DRenderer->SetRenderState( D3DRS_LIGHTING, TRUE );
	m_D3DRenderer->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_XRGB( 120, 120, 120 ) );
	m_D3DRenderer->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	m_D3DRenderer->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
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

	m_RenderObjects.clear();
	m_D3DRenderer = NULL;
	g_Renderer = NULL;
}


//------------------------------------------------------------------------------------------------------------------
void cRenderer::RenderScene( )
{
    D3DXMATRIXA16 mView;
    D3DXMATRIXA16 mProj;
    //D3DXMATRIXA16 mWorld;
    //D3DXMATRIXA16 mWorldViewProjection;

	// Clear the render target and the zbuffer 
	assert( m_D3DRenderer->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 255, 0, 0, 255), 1.0f, 0 ) == S_OK );
	// Render the scene
    if( SUCCEEDED( m_D3DRenderer->BeginScene() ) )
    {
	    // Get the projection & view matrix from the camera class
		//D3DXMatrixIdentity( &mWorld );
        mProj = *m_Camera->GetProjMatrix();
        mView = *m_Camera->GetViewMatrix();

        //mWorldViewProjection = g_Render.mCellWorld * mView * mProj;
        //mWorldView = g_Render.mCellWorld * mView;
		m_D3DRenderer->SetTransform( D3DTS_VIEW, &mView );
		m_D3DRenderer->SetTransform( D3DTS_PROJECTION, &mProj );
		for( tObjectPtrList::iterator objIt = m_RenderObjects.begin(); objIt != m_RenderObjects.end(); ++objIt)
		{
			cObject * obj = *objIt;
			obj->Render();
		}
	}

	D3DXMATRIX viewProj = mView * mProj;
	g_Level->GetFrustum()->DrawFrustum( m_D3DRenderer, viewProj );
	RenderText();
	assert( m_D3DRenderer->EndScene() == S_OK );
	m_ObjectsVisible = 0;
	m_RenderObjects.clear();
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
	m_TxtHelper->DrawFormattedTextLine( L"Pos: %0.2f, %0.2f, %0.2f", m_Camera->GetEyePt()->x, m_Camera->GetEyePt()->y, m_Camera->GetEyePt()->z );
	m_TxtHelper->DrawFormattedTextLine( L"Eye: %0.2f, %0.2f, %0.2f", m_Camera->GetLookAtPt()->x, m_Camera->GetLookAtPt()->y, m_Camera->GetLookAtPt()->z );
	m_TxtHelper->DrawFormattedTextLine( L"Visibe Objects: %d", m_ObjectsVisible );
	//Print text
	for( tWStringList::iterator it = m_ScreenTextList.begin(); it != m_ScreenTextList.end(); it++ )
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
//#define GROUND_Y 15.0f // -GROUND_Y is the Y coordinate of the ground.
//#define CAMERA_SIZE 0.2f // CAMERA_SIZE is used for clipping camera movement
//#define GRAVITY 3.0f  // GRAVITY defines the magnitude of the downward force applied to ammos.
//
//// MinBound and MaxBound are the bounding box representing the cell mesh.
//const D3DXVECTOR3         g_MinBound( -160.0f, -GROUND_Y, -160.0f );
//const D3DXVECTOR3         g_MaxBound( 160.0f, GROUND_Y, 160.0f );
//
//    // Setup the camera
//    D3DXVECTOR3 MinBound( g_MinBound.x + CAMERA_SIZE, g_MinBound.y + CAMERA_SIZE, g_MinBound.z + CAMERA_SIZE );
//    D3DXVECTOR3 MaxBound( g_MaxBound.x - CAMERA_SIZE, g_MaxBound.y - CAMERA_SIZE, g_MaxBound.z - CAMERA_SIZE );
//    m_Camera->SetClipToBoundary( true, &MinBound, &MaxBound );
//    m_Camera->SetEnableYAxisMovement( true );
//    m_Camera->SetRotateButtons( false, false, true );
//    m_Camera->SetScalers( 0.001f, 4.0f );
//    D3DXVECTOR3 vecEye( 0.0f,  0.46f, 0.49f );
//    D3DXVECTOR3 vecAt ( 0.0f, -0.19f, -0.26f );
//    m_Camera->SetViewParams( &vecEye, &vecAt );
//
//	//m_D3DRenderer->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );
//
//   // Enable this light with Direct3D.
//	m_D3DRenderer->SetRenderState( D3DRS_LIGHTING, TRUE );
//	m_D3DRenderer->LightEnable( 0, TRUE );
//
//	// Create ambient light.
//	m_D3DRenderer->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_COLORVALUE( 0.5f, 0.5f, 0.5f, 1.0f ) );
//	m_TestObj = new cObject( OBJECTS_DIR L"Multiobj_Test.x" );
//
//	// D3DXVECTOR3 direction;
//	//D3DLIGHT9 lightSource;
//
//	//// We are creating a white directional light.
//	//lightSource.Type = D3DLIGHT_DIRECTIONAL;
//	//lightSource.Range = 1000.0f;
//	//lightSource.Diffuse.r = 0.5f;
//	//lightSource.Diffuse.g = 0.5f;
//	//lightSource.Diffuse.b = 0.5f;
//	//lightSource.Diffuse.a = 1.0f;
//
//	//// Now we set up the lights direction.
//	//direction = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
//	//D3DXVec3Normalize( ( D3DXVECTOR3* )&lightSource.Direction, &direction );
//
//	//// Set the light to the scene.   
//	//m_D3DRenderer->SetLight( 0, &lightSource );
}
