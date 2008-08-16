//----------------------------------------------------------------------------------------------------------
#ifndef FRUSTUM_H
#define FRUSTUM_H


//----------------------------------------------------------------------------------------------------------
enum
{
	eOutside_Frustum = 0,
	eInside_Frustum,
	eIntersect_Frustum
};


//----------------------------------------------------------------------------------------------------------
class cFrustum
{
public:
	cFrustum( float pFov, float pNearDist, float pFarDist, float pRatio );
	~cFrustum();
	int SphereInFrusutm( const cSphere * pSphere );
	void UpdateFrustum( CFirstPersonCamera * pCam );
	void DrawFrustum( IDirect3DDevice9 * pDevice, D3DXMATRIX & pMat );

private:
	enum ePlane
	{
		ePlane_Top = 0,
		ePlane_Bottom,
		ePlane_Left,
		ePlane_Right,
		ePlane_Near,
		ePlane_Far,
		ePlane_Max
	};

	float	m_NearDist;
	float	m_FarDist;
	float	m_Fov;
	float	m_Ratio;
	float	m_TanVal;
	float	m_HalfNearHeight;
	float	m_HalfNearWidth;
	float	m_HalfFarHeight;
	float	m_HalfFarWidth;
	
	////plane extraction from cam
	cVector m_FarTopLeftSide;
	cVector m_FarTopRightSide;
	cVector m_FarBottomLeftSide;
	cVector m_FarBottomRightSide;

	cVector m_NearTopLeftSide;
	cVector m_NearTopRightSide;
	cVector m_NearBottomLeftSide;
	cVector m_NearBottomRightSide;

	//six plane for frustum
	cPlane	m_FrustumPlanes[ePlane_Max];
};


//----------------------------------------------------------------------------------------------------------
#endif