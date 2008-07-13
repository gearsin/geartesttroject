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

	if( m_TxtHelper )
	{
		delete m_TxtHelper;
		m_TxtHelper = NULL;
	}
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

	RenderText();
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
void cRenderer::PrintText( const WCHAR *pFormat, ... )
{
    wchar_t strBuffer[512];
    
    va_list args;
    va_start( args, pFormat );
    StringCchVPrintf( strBuffer, 512, pFormat, args );
    strBuffer[511] = L'\0';
    va_end(args);

	//conver wide char * to char * 
 //  size_t convertedChars = 0;
 //  size_t  sizeInBytes = ( wcslen( strBuffer ) * 2 );
 //  errno_t err = 0;
 //  char * scrStr = (char *)malloc(sizeInBytes);

 //  err = wcstombs_s( &convertedChars, scrStr, sizeInBytes, strBuffer, sizeInBytes );
 //  
 //  if ( err != 0 )
 //     Assert( false, "wcstombs_s  failed!\n" );
	//
	//std::string scrTxt = scrStr;
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

	//Print text
	for( tStringList::iterator it = m_ScreenTextList.begin(); it != m_ScreenTextList.end(); it++ )
	{
		std::wstring scrText = *it;
		m_TxtHelper->DrawTextLine( scrText.c_str() );
	}
	m_TxtHelper->End();

	m_ScreenTextList.clear();
}