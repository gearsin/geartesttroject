//------------------------------------------------------------------------------------------------------------------
#ifndef RENDERER_H
#define RENDERER_H


//------------------------------------------------------------------------------------------------------------------
class cRenderer
{
public:
	cRenderer();
	cRenderer( IDirect3DDevice9 * pD3Ddevice, unsigned int pLineWidht );
	~cRenderer();
	void RenderScene( IDirect3DDevice9 * pD3DRendererDevice );
	void RenderText( const WCHAR * pFormat, ... );
	HRESULT CreateFont( IDirect3DDevice9 * pD3Ddevice, unsigned int pLineWidth = 15 );
private:

	//IDirect3DDevice9 * m_RendererDevice;
	CDXUTTextHelper	*  m_TxtHelper;
    ID3DXFont*         m_Font;        // Font for drawing text
    ID3DXSprite*       m_TextSprite;  // Sprite for batching draw text calls

protected:
};


//------------------------------------------------------------------------------------------------------------------
extern cRenderer * g_Renderer;


//------------------------------------------------------------------------------------------------------------------
#endif