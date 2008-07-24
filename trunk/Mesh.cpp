//------------------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "Renderer.h"
#include "Mesh.h"


//------------------------------------------------------------------------------------------------------------------
cMesh::cMesh( const WCHAR * pFileName ) :
	CDXUTXFileMesh( pFileName )
{
	HRESULT hr;
	WCHAR filePath[1024];

	hr = DXUTFindDXSDKMediaFileCch( filePath, MAX_PATH, pFileName );
	Assert( hr == S_OK, " File not found\n" );
	hr = Create( g_Renderer->GetRendererDevice() , filePath );
	Assert( hr == S_OK, "Can't load mesh file\n" );
}


//------------------------------------------------------------------------------------------------------------------
cMesh::~cMesh()
{
}


//------------------------------------------------------------------------------------------------------------------
void cMesh::Render()
{	
	CDXUTXFileMesh::Render( g_Renderer->GetRendererDevice() );
}