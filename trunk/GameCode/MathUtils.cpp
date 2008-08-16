//----------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "MathUtils.h"


//------------------------------------------------------------------------------------------------------------------
cVector D3DXVecToVector( const D3DXVECTOR3 & pVector )
{
	return cVector( pVector.x, pVector.y, pVector.z );
}


//------------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 VectorToD3DXVec( const cVector & pVector )
{
	return D3DXVECTOR3( pVector.m_X, pVector.m_Y, pVector.m_Z );
}