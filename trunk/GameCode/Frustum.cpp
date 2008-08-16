//----------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "MathUtils.h"
#include "Frustum.h"


//----------------------------------------------------------------------------------------------------------
cFrustum::cFrustum( float pFov, float pNearDist, float pFarDist, float pRatio )
{
	m_Fov = pFov;
	m_NearDist = pNearDist;
	m_FarDist = pFarDist;
	m_Ratio = pRatio;
	m_TanVal = tan( pFov / 2.0f );

	m_HalfNearHeight = m_TanVal * m_NearDist;
	m_HalfNearWidth = m_HalfNearHeight * m_Ratio;
	m_HalfFarHeight = m_TanVal * m_FarDist;
	m_HalfFarWidth = m_HalfFarHeight * m_Ratio;
}


//----------------------------------------------------------------------------------------------------------
cFrustum::~cFrustum()
{

}


//----------------------------------------------------------------------------------------------------------
void cFrustum::UpdateFrustum( CFirstPersonCamera * pCam  )
{
//http://www.lighthouse3d.com/opengl/viewfrustum/index.php?gaplanes
//An optimization can be performed using this approach to extract the near and far planes. 
//A plane can be defined with a normal and a point, and for those planes these are known based on the camera 
//definition. The near plane can be defined with d as a normal, and nc as a point on the plane. 
//As for the far plane the normal is -d and the point is fc. The other planes can also be computed in a 
//more efficient manner, namely by using a normal vector and a point to define a plane. The following code 
//provides the normal for the right plane. As for the point, the camera position, p is a point in all top,
//bottom, left and right planes (considering a perspective projection).
//	nc = p + d * nearDist
//	fc = p + d * farDist

//	a = (nc + right * Wnear / 2) - p
//	a.normalize();
//	normalRight = up * a
//Both up and a are vectors that belong to the right plane, so their cross product will provide a 
//normal for the plane. 

	cVector camPos = D3DXVecToVector( *( pCam->GetEyePt() ) );
	cVector lookAt = D3DXVecToVector( *( pCam->GetLookAtPt() ) );
	cVector upVec = D3DXVecToVector( *( pCam->GetWorldUp() ) );

	cVector dirVec = camPos - lookAt;
	dirVec.Normalize();

	cVector rightVec = upVec.Cross( dirVec );
	rightVec.Normalize();

	upVec = dirVec.Cross( rightVec );

	cVector nearPlaneCenter = camPos - dirVec * m_NearDist;
	cVector farPlaneCenter = camPos - dirVec * m_FarDist;

	cVector nearTop = upVec * m_HalfNearHeight;
	cVector nearRight = rightVec * m_HalfNearWidth;
	cVector farTop = upVec * m_HalfFarHeight;
	cVector farRight = rightVec * m_HalfFarWidth;

	//get the near plane corner pts
	m_NearTopLeftSide = nearPlaneCenter + nearTop - nearRight;
	m_NearTopRightSide = nearPlaneCenter + nearTop + nearRight;
	m_NearBottomLeftSide = nearPlaneCenter - nearTop - nearRight;
	m_NearBottomRightSide = nearPlaneCenter - nearTop + nearRight;

	//get the near plane corner pts
	m_FarTopLeftSide = farPlaneCenter + farTop - farRight;
	m_FarTopRightSide = farPlaneCenter + farTop + farRight;
	m_FarBottomLeftSide = farPlaneCenter - farTop - farRight;
	m_FarBottomRightSide = farPlaneCenter - farTop + farRight;

	// compute the six planes
	// the function set3Points assumes that the points
	// are given in counter clockwise order

	m_FrustumPlanes[ePlane_Top].Set3Points( m_NearTopRightSide, m_NearTopLeftSide, m_FarTopLeftSide );
	m_FrustumPlanes[ePlane_Bottom].Set3Points( m_NearBottomLeftSide, m_NearBottomRightSide, m_FarBottomRightSide );
	m_FrustumPlanes[ePlane_Left].Set3Points( m_NearTopLeftSide, m_NearBottomLeftSide, m_FarBottomLeftSide );
	m_FrustumPlanes[ePlane_Right].Set3Points( m_NearBottomRightSide, m_NearTopRightSide, m_FarBottomRightSide );
	m_FrustumPlanes[ePlane_Near].Set3Points( m_NearTopLeftSide, m_NearTopRightSide, m_NearBottomRightSide );
	m_FrustumPlanes[ePlane_Far].Set3Points( m_FarTopRightSide, m_FarTopLeftSide, m_FarBottomLeftSide );
}


//----------------------------------------------------------------------------------------------------------
int cFrustum::SphereInFrusutm( const cSphere * pSphere )
{
	float distance = 0.0f;
	int result = eInside_Frustum;

	const char *strClipPlane[] =
	{
		"Top",
		"Bottom",
		"Left",
		"Right",
		"Near",
		"Far"
	};

	for( int idx = 0; idx < ePlane_Max; ++idx )
	{
		distance = m_FrustumPlanes[idx].ComputeDistance( pSphere->m_Center );
		if ( distance < -pSphere->m_Radius )
		{
			Log( " InterSect @ %s", strClipPlane[idx] );
			return eOutside_Frustum;
		}
		else if (distance < pSphere->m_Radius )
		{
			result =  eIntersect_Frustum;
		}
	}

	return( result );
}


//----------------------------------------------------------------------------------------------------------
void cFrustum::DrawFrustum( IDirect3DDevice9 * pDevice, D3DXMATRIX & pMat )
{
	return ;
	//obsolute 
	ID3DXLine * frustumLine;
	HRESULT hr = D3DXCreateLine( pDevice, &frustumLine );
	cVector lineStart, lineEnd;

	//near Plane Normal
	lineStart = ( m_NearTopLeftSide + m_NearTopRightSide + m_NearBottomLeftSide + m_NearBottomRightSide ) * 0.25f;
	lineEnd = lineStart + m_FrustumPlanes[ePlane_Near].m_PlaneNormal;

	D3DXVECTOR3 nearPlaneNormal [] = 
	{
		VectorToD3DXVec( lineStart ),
		VectorToD3DXVec( lineEnd )
	};

	//far Plane Normal
	lineStart = ( m_FarTopLeftSide + m_FarTopRightSide + m_FarBottomLeftSide + m_FarBottomRightSide ) * 0.25f;
	lineEnd = lineStart + m_FrustumPlanes[ePlane_Far].m_PlaneNormal;

	D3DXVECTOR3 farPlaneNormal [] = 
	{
		VectorToD3DXVec( lineStart ),
		VectorToD3DXVec( lineEnd )
	};

	//left Plane Normal
	lineStart = ( m_NearTopLeftSide + m_NearBottomLeftSide + m_FarTopLeftSide + m_FarBottomLeftSide ) * 0.25f;
	lineEnd = lineStart + m_FrustumPlanes[ePlane_Left].m_PlaneNormal + cVector( 12.0f, 0.0f, 0.0f );

	D3DXVECTOR3 leftPlaneNormal [] = 
	{
		VectorToD3DXVec( lineStart ),
		VectorToD3DXVec( lineEnd )
	};

	assert( frustumLine->Begin() == S_OK );
	assert( frustumLine->DrawTransform( nearPlaneNormal, 2, &pMat, D3DCOLOR_RGBA( 255, 0, 0, 255 ) ) == S_OK );
	assert( frustumLine->DrawTransform( farPlaneNormal, 2, &pMat, D3DCOLOR_RGBA( 0, 255, 0, 255 ) ) == S_OK );
	assert( frustumLine->DrawTransform( leftPlaneNormal, 2, &pMat, D3DCOLOR_RGBA( 0, 0, 255, 255 ) ) == S_OK );
	assert( frustumLine->End() == S_OK );
	SAFE_RELEASE( frustumLine );
}