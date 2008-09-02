//----------------------------------------------------------------------------------------------------
#ifndef BaseCamera_H
#define BaseCamera_H


//----------------------------------------------------------------------------------------------------
class cBaseCamera
{

public:
	cBaseCamera( const float pNearPlaneDist, const float pFarPlaneDist, const float pFov, 
				const float pAspect );
	void SetLookAtParams( const cVector & pCamPos, const cVector & pLookAt, const cVector & pUp );
	void Update();

private:
	void Move();
	void Rotate();

private:
	float		m_NearDist;
	float		m_FarDist;
	float		m_Aspect;
	float		m_Fov;
	cVector		m_CamPos;	//Camera Pos
	cVector		m_LookAt;
	cVector		m_Up;
	D3DXMATRIX	m_ViewMat;
	D3DXMATRIX	m_ProjMat;
};


//----------------------------------------------------------------------------------------------------
#endif