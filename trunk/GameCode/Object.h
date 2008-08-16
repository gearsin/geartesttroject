//------------------------------------------------------------------------------------------------------------------
#ifndef OBJECT_H
#define OBJECT_H


//------------------------------------------------------------------------------------------------------------------
#include "Vector.h"


//------------------------------------------------------------------------------------------------------------------
class cMesh;
class cFrustum;

//------------------------------------------------------------------------------------------------------------------
class cObject
{

public:
	cObject( const WCHAR * pFileName );
	cObject( const WCHAR * pFileName, const cVector & pPos );
	~cObject();
	void SetTranslation( const cVector & pPos );
	void SetTranslation( const float pX, const float pY, const float pZ );
	cVector GetTranslation();
	void SetRotation( const cVector & pRot );
	cVector GetRotation();
	void SetRotation( const float pRotX, const float pRotY, const float pRotZ );
	void ScaleBy( const float pScaleVal );
	void ScaleBy( const float pScaleX, const float pScaleY, const float pScaleZ );
	void SetScale( const float pScaleX, const float pScaleY, const float pScaleZ );
	void Update( cFrustum * pFrustum );
	void Render(); //Obsolute shuld be removed
	const WCHAR * GetName();
	void SetName( const WCHAR * pName );
	void SetSphereBound( const cVector & pCenter, const float Radius );
	void SetSphereBound( const cSphere & pSphere );

protected:
		
private:
	void ComputeBoundingBoxAndSphere();

private:
	std::wstring	m_Name;
	cVector			m_Translation;
	cVector			m_Rotation;
	cVector			m_Scale;
	cVector			m_BoxMin;	//Bounding box min
	cVector			m_BoxMax;	//bounding box max
	cSphere			m_BoundSphere;
	D3DXMATRIX		m_WorldMat;
	cMesh	  *		m_Mesh;
};


//------------------------------------------------------------------------------------------------------------------
inline void cObject::SetName( const WCHAR * pName )
{
	m_Name = pName;
}


//------------------------------------------------------------------------------------------------------------------
inline const WCHAR * cObject::GetName()
{
  return m_Name.c_str();
}


//------------------------------------------------------------------------------------------------------------------
inline void cObject::SetSphereBound( const cVector & pCenter, const float Radius )
{
	m_BoundSphere.m_Center = pCenter;
}


//------------------------------------------------------------------------------------------------------------------
inline void cObject::SetSphereBound( const cSphere &pSphere )
{
	m_BoundSphere.m_Center = pSphere.m_Center;
	m_BoundSphere.m_Radius = pSphere.m_Radius;
}


//------------------------------------------------------------------------------------------------------------------
inline cVector cObject::GetTranslation()
{
	return m_Translation;
}


//------------------------------------------------------------------------------------------------------------------
inline cVector cObject::GetRotation()
{
//	cVector  Rot = m_Rotation;
//
//
////toEuler() 
////{
//   cVector euler;
//   euler.x = asin( m_WorldMat._11 );
//
//   if( acos( euler.x ) != 0.f)
//   {
//      euler.y = atan2( -m_WorldMat._31, m_WorldMat._33 );
//      euler.z = atan2( -m_WorldMat._12, m_WorldMat._22 );
//   }
//   else
//   {
//      euler.y = 0.f;
//      euler.z = atan2( m_WorldMat._21, m_WorldMat._11 );
//   }
//
////}
//
	return m_Rotation;
}


//------------------------------------------------------------------------------------------------------------------
#endif
