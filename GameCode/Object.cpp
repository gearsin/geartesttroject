//------------------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "Mesh.h"
#include "Object.h"
#include "Renderer.h"
#include "Frustum.h"
#include "MathUtils.h"


//------------------------------------------------------------------------------------------------------------------
cObject::cObject( const WCHAR *pFileName ) :
	m_Translation( 0.0f, 0.0f, 0.0f ),
	m_Rotation( 0.0f, 0.0f, 0.0f ),
	m_Scale( 1.0f, 1.0f, 1.0f )
{
	m_Mesh = new cMesh( pFileName );
	std::wstring fileName = pFileName;
	unsigned int fwdSlash = fileName.find_last_of( L"\\" );
	unsigned int lastdot = fileName.find_last_of( L"." );
	m_Name= fileName.substr( fwdSlash + 1, ( lastdot - fwdSlash - 1) );
	ComputeBoundingBoxAndSphere();
	D3DXMatrixIdentity( &m_WorldMat );
}


//------------------------------------------------------------------------------------------------------------------
cObject::cObject( const WCHAR *pFileName, const cVector & pPos )
{
	m_Mesh = new cMesh( pFileName );
	ComputeBoundingBoxAndSphere();
	SetTranslation( pPos );
}


//------------------------------------------------------------------------------------------------------------------
cObject::~cObject()
{
	if( m_Mesh )
	{
		delete m_Mesh;
		m_Mesh = NULL;
	}

}


//------------------------------------------------------------------------------------------------------------------
void cObject::ComputeBoundingBoxAndSphere()
{
   IDirect3DVertexBuffer9* vertexBuffer = NULL;
   void* vertices;
   assert( m_Mesh->GetMesh()->GetVertexBuffer( &vertexBuffer ) == S_OK );
   assert( vertexBuffer->Lock( 0, 0, &vertices, 0 ) == S_OK );

   D3DVERTEXELEMENT9 declaration[ MAXD3DDECLLENGTH + 1 ];
   m_Mesh->GetMesh()->GetDeclaration( declaration );
   DWORD dwStride = D3DXGetDeclVertexSize( declaration, 0 );
   D3DXVECTOR3 minBox, maxBox;
   D3DXVECTOR3 sphereCenter;
   float radius;
   assert( D3DXComputeBoundingBox( ( D3DXVECTOR3* )vertices, m_Mesh->GetMesh()->GetNumVertices(), dwStride, 
									 &minBox, &maxBox ) == S_OK);
   assert( D3DXComputeBoundingSphere( (D3DXVECTOR3 *) vertices, m_Mesh->GetMesh()->GetNumVertices(), dwStride,
										&sphereCenter, &radius ) == S_OK );
   vertexBuffer->Unlock();
   SAFE_RELEASE( vertexBuffer );

   m_BoxMin = D3DXVecToVector( minBox );
   m_BoxMax = D3DXVecToVector( maxBox );
   cVector pos = ( m_BoxMin + m_BoxMax ) /2.0f;
   m_BoundSphere.m_Center = D3DXVecToVector( sphereCenter );
   m_BoundSphere.m_Radius = radius;
   //m_Translation = m_BoundSphere.m_Center;
}


//------------------------------------------------------------------------------------------------------------------
void cObject::SetTranslation( const cVector & pPos )
{
	m_Translation = pPos;
	SetSphereBound( m_Translation, m_BoundSphere.m_Radius );
}


//------------------------------------------------------------------------------------------------------------------
void cObject::SetTranslation( const float pX, const float pY, const float pZ )
{
	m_Translation.m_X = pX;
	m_Translation.m_Y = pY;
	m_Translation.m_Z = pZ;
	SetSphereBound( m_Translation, m_BoundSphere.m_Radius );
}


//------------------------------------------------------------------------------------------------------------------
void cObject::SetRotation( const cVector & pRot )
{
	m_Rotation = pRot;
}


//------------------------------------------------------------------------------------------------------------------
void cObject::SetRotation( const float pRotX, const float pRotY, const float pRotZ )
{
	m_Rotation.m_X = pRotX;
	m_Rotation.m_Y = pRotY;
	m_Rotation.m_Z = pRotZ;
}


//------------------------------------------------------------------------------------------------------------------
void cObject::SetScale( const float pScaleX, const float pScaleY, const float pScaleZ )
{
	m_Scale.m_X = pScaleX;
	m_Scale.m_Y = pScaleY;
	m_Scale.m_Z = pScaleZ;

	//TODO:: Calculate the new bouds on scale
}


//------------------------------------------------------------------------------------------------------------------
void cObject::ScaleBy( const float pScaleVal )
{
	m_Scale *= pScaleVal;

	//TODO:: Calculate the new bouds on scale
}


//------------------------------------------------------------------------------------------------------------------
void cObject::ScaleBy( const float pScaleX, const float pScaleY, const float pScaleZ )
{
	m_Scale.m_X *= pScaleX;
	m_Scale.m_Y *= pScaleY;
	m_Scale.m_Z *= pScaleZ;

	//TODO:: Calculate the new bouds on scale
}


//------------------------------------------------------------------------------------------------------------------
void cObject::Update( cFrustum * pFrustum )
{
	int res = pFrustum->SphereInFrusutm( &m_BoundSphere );
	if( res > 0 )
	{
		D3DXMATRIX rotMat, scaleMat, transMat;
		D3DXMatrixTranslation( &transMat, m_Translation.m_X, m_Translation.m_Y, m_Translation.m_Z );
		D3DXMatrixRotationYawPitchRoll( &rotMat, m_Rotation.m_Y, m_Rotation.m_X, m_Rotation.m_Z );
		D3DXMatrixScaling( &scaleMat, m_Scale.m_X, m_Scale.m_Y, m_Scale.m_Z );
		m_WorldMat = scaleMat * rotMat * transMat;
		g_Renderer->AddToRenderList( this );
	}
	else
	{
	//	Log( L" Clip object: %s\n", GetName() );
	}
}


//------------------------------------------------------------------------------------------------------------------
void cObject::Render()
{
	g_Renderer->GetRendererDevice()->SetTransform( D3DTS_WORLD, &m_WorldMat );
	m_Mesh->Render();
}
