//------------------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "Mesh.h"
#include "Object.h"
#include "UtilityFunction.h"


//------------------------------------------------------------------------------------------------------------------
cObject::cObject( const WCHAR *pFileName )
{
	m_ModelMesh = new cMesh( pFileName );
}


//------------------------------------------------------------------------------------------------------------------
cObject::cObject( const WCHAR *pFileName, const cVector & pPos )
{
	m_ModelMesh = new cMesh( pFileName );
//	SetTranslation( pPos );
}


//------------------------------------------------------------------------------------------------------------------
cObject::~cObject()
{
	if( m_ModelMesh )
	{
		delete m_ModelMesh;
		m_ModelMesh = NULL;
	}

}


//------------------------------------------------------------------------------------------------------------------
void cObject::Render()
{
	m_ModelMesh->Render();
}