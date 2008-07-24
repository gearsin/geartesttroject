//------------------------------------------------------------------------------------------------------------------
#ifndef RENDERER_H
#define RENDERER_H


//------------------------------------------------------------------------------------------------------------------
#include "UnitTest.h"


//------------------------------------------------------------------------------------------------------------------
class cObject;


//------------------------------------------------------------------------------------------------------------------
typedef std::list< std::wstring > tStringList;


//------------------------------------------------------------------------------------------------------------------
class cRenderer : public cUnitTest
{
public:
	cRenderer();
	cRenderer( IDirect3DDevice9 * pD3Ddevice, unsigned int pLineWidht );
	~cRenderer();
	void RenderScene( );
	void PrintText( const WCHAR * pFormat, ... );
	HRESULT CreateFont( unsigned int pLineWidth = 15 );
	IDirect3DDevice9 * GetRendererDevice();
	virtual void UnitTestFunction();

private:
	//IDirect3DDevice9 * m_RendererDevice;
	CDXUTTextHelper	*  m_TxtHelper;
    ID3DXFont*         m_Font;        // Font for drawing text
    ID3DXSprite*       m_TextSprite;  // Sprite for batching draw text calls
	tStringList		   m_ScreenTextList;

private:
	void RenderText();
cObject * m_TestObj;
	IDirect3DDevice9 * m_D3DRenderer;

protected:
};


//------------------------------------------------------------------------------------------------------------------
extern cRenderer * g_Renderer;


//------------------------------------------------------------------------------------------------------------------
inline IDirect3DDevice9 * cRenderer::GetRendererDevice()
{
	return m_D3DRenderer;
}

//------------------------------------------------------------------------------------------------------------------
#endif