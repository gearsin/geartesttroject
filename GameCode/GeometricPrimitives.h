//---------------------------------------------------------------------------------------------------------
#ifndef GEOMETRICPRIMITIVE_H
#define GEOMETRICPRIMITIVE_H


//---------------------------------------------------------------------------------------------------------
class cSphere
{
public:
	cVector m_Center;
	float m_Radius;
};


//---------------------------------------------------------------------------------------------------------
class cPlane
{
public:
	enum eSide
	{
		eSide_On = 0,
		eSide_Front,
		eSide_Back 
	};

	cVector m_PlaneNormal;
	cVector	m_Point;
	float	m_Distance;

public:
	float ComputeDistance( const cVector & pPoint );
	void SetNormalAndPoint( const cVector & pNormal, const cVector & pPoint );
	void Set3Points( const cVector & pVec1, const cVector & pVec2, const cVector & pVec3 );
};


//---------------------------------------------------------------------------------------------------------
inline float cPlane::ComputeDistance( const cVector & pPoint )
{
	return ( m_Distance + m_PlaneNormal.Dot( pPoint ) );
}


//---------------------------------------------------------------------------------------------------------
inline void cPlane::SetNormalAndPoint( const cVector & pNormal, const cVector & pPoint )
{
	m_PlaneNormal = pNormal;
	m_Point = pPoint;
	m_Distance = -m_PlaneNormal.Dot( m_Point );
}


//---------------------------------------------------------------------------------------------------------
inline void cPlane::Set3Points( const cVector &pVec1, const cVector &pVec2, const cVector &pVec3 )
{
	cVector edge0 = pVec1 - pVec2;
	cVector edge1 = pVec3 - pVec2;
	cVector normal = edge1.Cross( edge0 );
	normal.Normalize();
	SetNormalAndPoint( normal, pVec2 );
}


//---------------------------------------------------------------------------------------------------------
#endif