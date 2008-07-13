//------------------------------------------------------------------------------------------------------------------
#ifndef RENDERER_H
#define RENDERER_H


//------------------------------------------------------------------------------------------------------------------
typedef std::list< std::wstring > tStringList;


//------------------------------------------------------------------------------------------------------------------
class cRenderer
{
public:
	cRenderer();
	cRenderer( IDirect3DDevice9 * pD3Ddevice, unsigned int pLineWidht );
	~cRenderer();
	void RenderScene( IDirect3DDevice9 * pD3DRendererDevice );
	void PrintText( const WCHAR * pFormat, ... );
	HRESULT CreateFont( IDirect3DDevice9 * pD3Ddevice, unsigned int pLineWidth = 15 );

private:
	//IDirect3DDevice9 * m_RendererDevice;
	CDXUTTextHelper	*  m_TxtHelper;
    ID3DXFont*         m_Font;        // Font for drawing text
    ID3DXSprite*       m_TextSprite;  // Sprite for batching draw text calls
	tStringList		   m_ScreenTextList;

private:
	void RenderText();

protected:
};


//------------------------------------------------------------------------------------------------------------------
extern cRenderer * g_Renderer;


//------------------------------------------------------------------------------------------------------------------
#endif