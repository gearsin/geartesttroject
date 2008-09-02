//----------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "MathUtils.h"
#include "BaseCamera.h"


//----------------------------------------------------------------------------------------------------
cBaseCamera::cBaseCamera( const float pNearPlaneDist, const float pFarPlaneDist, const float pFov, 
						 const float pAspect )
{
	m_NearDist = pNearPlaneDist;
	m_FarDist = pFarPlaneDist;
	m_Fov = pFov;
	m_Aspect = pAspect;

	D3DXMatrixPerspectiveFovLH( &m_ProjMat, m_Fov, m_Aspect, m_NearDist, m_FarDist );
}


//----------------------------------------------------------------------------------------------------
void cBaseCamera::SetLookAtParams( const cVector &pCamPos, const cVector &pLookAt, const cVector & pUp )
{
	m_CamPos = pCamPos;
	m_LookAt = pLookAt;
	m_Up = pUp;
	D3DXMatrixLookAtLH( &m_ViewMat, &VectorToD3DXVec( m_CamPos ), &VectorToD3DXVec( m_LookAt ), 
		&VectorToD3DXVec( m_Up ) );
}


//----------------------------------------------------------------------------------------------------
void cBaseCamera::Update()
{
	
}