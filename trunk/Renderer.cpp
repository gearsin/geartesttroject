//------------------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "Renderer.h"


//------------------------------------------------------------------------------------------------------------------
cRenderer * g_Renderer = NULL;


//------------------------------------------------------------------------------------------------------------------
cRenderer::cRenderer()
{

}


//------------------------------------------------------------------------------------------------------------------
cRenderer::cRenderer( IDirect3DDevice9 * pD3Ddevice, unsigned int  pLineWidth )
{
	CreateFont( pD3Ddevice, pLineWidth );
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

	//if( m_TxtHelper )
	//{
	//	delete m_TxtHelper;
	//	m_TxtHelper = NULL;
	//}
}


//------------------------------------------------------------------------------------------------------------------
void cRenderer::RenderScene( IDirect3DDevice9 *pD3DRendererDevice )
{
	HRESULT hr;

	// Clear the render target and the zbuffer 
	V( pD3DRendererDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 255, 0, 0, 255), 1.0f, 0 ) );

	// Render the scene
    if( SUCCEEDED( pD3DRendererDevice->BeginScene() ) )
    {

	}

	RenderText( L"Test Message" );
	V( pD3DRendererDevice->EndScene() );	
}


//------------------------------------------------------------------------------------------------------------------
HRESULT cRenderer::CreateFont( IDirect3DDevice9 *pD3Ddevice, unsigned int pLineWidth  )
{
	HRESULT hr;
	// Initialize the font
    V_RETURN( D3DXCreateFont( pD3Ddevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
                         OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
                         L"Arial", &m_Font ) );

	V_RETURN( D3DXCreateSprite( pD3Ddevice, &m_TextSprite ) );

	m_TxtHelper = new CDXUTTextHelper( m_Font, m_TextSprite, pLineWidth );
	return hr;
}


//------------------------------------------------------------------------------------------------------------------
void cRenderer::RenderText( const WCHAR *pFormat, ... )
{
    WCHAR strBuffer[512];
    
    va_list args;
    va_start( args, pFormat );
    StringCchVPrintf( strBuffer, 512, pFormat, args );
    strBuffer[511] = L'\0';
    va_end(args);

	m_TxtHelper->Begin();
    m_TxtHelper->SetInsertionPos( 2, 0 );
    m_TxtHelper->SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
    m_TxtHelper->DrawTextLine( DXUTGetFrameStats(true) );
    m_TxtHelper->DrawTextLine( DXUTGetDeviceStats() );
    m_TxtHelper->DrawTextLine( L"X: Add Droid  Y: Toggle Droid Movement  B: Mass Kill" );
    //m_TxtHelper->DrawFormattedTextLine( L"Pos: %0.2f, %0.2f, %0.2f", g_Camera.GetEyePt()->x, g_Camera.GetEyePt()->y, g_Camera.GetEyePt()->z );
	m_TxtHelper->End();

}