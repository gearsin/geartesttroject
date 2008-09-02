//------------------------------------------------------------------------------------------------------------------
#ifndef RENDERER_H
#define RENDERER_H


//------------------------------------------------------------------------------------------------------------------
#include "UnitTest.h"


//------------------------------------------------------------------------------------------------------------------
class cObject;
CFirstPersonCamera;


//------------------------------------------------------------------------------------------------------------------
typedef std::list< std::wstring > tWStringList;
typedef std::list< cObject* > tObjectPtrList;

//------------------------------------------------------------------------------------------------------------------
class cRenderer : public cUnitTest
{
public:
	static void Create();
	static void Destroy();
	cRenderer( unsigned int pLineWidht );
	~cRenderer();
	void RenderScene( );
	void PrintText( const WCHAR * pFormat, ... );
	HRESULT CreateFont( unsigned int pLineWidth = 15 );
	IDirect3DDevice9 * GetRendererDevice();
	virtual void UnitTestFunction();
	void SetCamera( CFirstPersonCamera * pCamera );
	CFirstPersonCamera * GetCamera();
	void AddToRenderList( cObject * pObject );

private:
	CDXUTTextHelper		*	m_TxtHelper;
    ID3DXFont			*	m_Font;        // Font for drawing text
    ID3DXSprite			*	m_TextSprite;  // Sprite for batching draw text calls
	tWStringList			m_ScreenTextList;
	IDirect3DDevice9	*	m_D3DRenderer;
	CFirstPersonCamera	*	m_Camera;
	tObjectPtrList			m_RenderObjects;
	unsigned int			m_ObjectsVisible; 

private:
	void RenderText();


protected:
};


//------------------------------------------------------------------------------------------------------------------
extern cRenderer * g_Renderer;


//------------------------------------------------------------------------------------------------------------------
inline void cRenderer::SetCamera( CFirstPersonCamera * pCamera )
{
	m_Camera = pCamera;
}


//------------------------------------------------------------------------------------------------------------------
inline IDirect3DDevice9 * cRenderer::GetRendererDevice()
{
	return m_D3DRenderer;
}


//------------------------------------------------------------------------------------------------------------------
inline void cRenderer::AddToRenderList( cObject * pObject )
{
	m_RenderObjects.push_back( pObject );
	m_ObjectsVisible++;
}


//------------------------------------------------------------------------------------------------------------------
#endif