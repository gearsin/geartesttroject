//------------------------------------------------------------------------------------------------------------------
#ifndef VECTOR_H
#define VECTOR_H


//------------------------------------------------------------------------------------------------------------------
#include "UnitTest.h"


//------------------------------------------------------------------------------------------------------------------
class cVector : public cUnitTest
{
public:
	float m_X;
	float m_Y;
	float m_Z;
	static cVector ZERO;

public:
	//constructor
	cVector();
	cVector( float pX, float pY, float pZ );
	cVector( cVector & pVec );

	//Arithmetic  operator overolad
	inline cVector operator +( const cVector & pVec );
	inline cVector operator -( const cVector & pVec );
	inline cVector operator -( const cVector & pVec ) const;
	inline cVector& operator +=( const cVector & pVec );
	inline cVector& operator -=( const cVector & pVec );
	
	////dot product
	//cVector operator *( const cVector & pVec );
	//cVector& operator *=( const cVector & pVec );

	inline cVector operator *( const float  pScale );
	inline cVector operator *=( const float  pScale );
	inline cVector operator /( const float  pVal );
	inline cVector operator /=( const float  pVal );

	//conditional opeartor overload
	inline bool operator ==( const cVector & pVec );
	inline bool operator !=( const cVector & pVec );
	inline bool operator <( const cVector & pVec );
	inline bool operator <=( const cVector & pVec );
	inline bool operator >( const cVector & pVec );
	inline bool operator >=( const cVector & pVec );

	//sign operator
	inline cVector operator -( ) const;


	//assignement operator
	inline cVector& operator = ( const cVector & pVec );

	//Lenght
	inline float Length();
	inline float SqrLength();

	//Vector multiplication
	float Dot( const cVector & pVec );
	cVector Cross( const cVector & pVec );

	//normalize
	inline cVector& Normalize();

	cVector RotateByMatrix( const float pMat[16] ) const;

//TestFunction
	 virtual void UnitTestFunction();
};


//------------------------------------------------------------------------------------------------------------------
inline cVector cVector::operator +( const cVector & pVec )
{
	return cVector( m_X + pVec.m_X, m_Y + pVec.m_Y, m_Z + pVec.m_Z );
}


//------------------------------------------------------------------------------------------------------------------
inline cVector& cVector::operator +=( const cVector & pVec )
{
	m_X += pVec.m_X;
	m_Y += pVec.m_Y;
	m_Z += pVec.m_Z;
	return *this;
}


//------------------------------------------------------------------------------------------------------------------
inline cVector cVector::operator -( const cVector & pVec ) const
{
	return cVector( m_X - pVec.m_X, m_Y - pVec.m_Y, m_Z - pVec.m_Z );
}


//------------------------------------------------------------------------------------------------------------------
inline cVector cVector::operator -( const cVector & pVec )
{
	return cVector( m_X - pVec.m_X, m_Y - pVec.m_Y, m_Z - pVec.m_Z );
}


//------------------------------------------------------------------------------------------------------------------
inline cVector& cVector::operator -=( const cVector & pVec )
{
	m_X -= pVec.m_X;
	m_Y -= pVec.m_Y;
	m_Z -= pVec.m_Z;
	return *this;	
}


//------------------------------------------------------------------------------------------------------------------
inline cVector& cVector::operator =( const cVector & pVec )
{
	m_X = pVec.m_X;
	m_Y = pVec.m_Y;
	m_Z = pVec.m_Z;
	return *this;
}


//------------------------------------------------------------------------------------------------------------------
inline cVector cVector::operator -() const
{
	return ( cVector ( -m_X, -m_Y, -m_Z ) );
}


//------------------------------------------------------------------------------------------------------------------
inline float cVector::Length()
{
	return sqrtf( m_X * m_X + m_Y * m_Y + m_Z * m_Z );
}


//------------------------------------------------------------------------------------------------------------------
inline float cVector::SqrLength()
{
	return ( m_X * m_X + m_Y * m_Y + m_Z * m_Z );
}


//------------------------------------------------------------------------------------------------------------------
inline cVector cVector::operator *=( const float pScale )
{
	m_X *= pScale;
	m_Y *= pScale;
	m_Z *= pScale;
	return *this;
}


//------------------------------------------------------------------------------------------------------------------
inline cVector cVector::operator *( const float pScale )
{
	//*this *= pScale;
	return cVector( m_X * pScale, m_Y * pScale, m_Z * pScale );
}


//------------------------------------------------------------------------------------------------------------------
inline cVector cVector::operator /=( const float pVal )
{
	float scale = 1.0f / pVal;
	*this *= scale;
	return *this;
}


//------------------------------------------------------------------------------------------------------------------
inline cVector cVector::operator /( const float pVal )
{
	float scale = 1.0f / pVal;
	
	return cVector( m_X * scale, m_Y * scale, m_Z * scale );
}


//------------------------------------------------------------------------------------------------------------------
inline bool cVector::operator == ( const cVector & pVec )
{
	return ( m_X == pVec.m_X && m_Y == pVec.m_Y && m_Z == pVec.m_Z );
}


//------------------------------------------------------------------------------------------------------------------
inline bool cVector::operator != ( const cVector & pVec )
{
	return ( m_X != pVec.m_X && m_Y != pVec.m_Y && m_Z != pVec.m_Z );
}


//------------------------------------------------------------------------------------------------------------------
inline bool cVector::operator < ( const cVector & pVec )
{
	return ( m_X < pVec.m_X && m_Y < pVec.m_Y && m_Z < pVec.m_Z );
}


//------------------------------------------------------------------------------------------------------------------
inline bool cVector::operator <= ( const cVector & pVec )
{
	return ( m_X <= pVec.m_X && m_Y <= pVec.m_Y && m_Z <= pVec.m_Z );
}


//------------------------------------------------------------------------------------------------------------------
inline bool cVector::operator > ( const cVector & pVec )
{
	return ( m_X > pVec.m_X && m_Y > pVec.m_Y && m_Z > pVec.m_Z );
}


//------------------------------------------------------------------------------------------------------------------
inline bool cVector::operator >= ( const cVector & pVec )
{
	return ( m_X >= pVec.m_X && m_Y >= pVec.m_Y && m_Z >= pVec.m_Z );
}


//------------------------------------------------------------------------------------------------------------------
inline cVector& cVector::Normalize()
{
	float length = Length();
	assert( length > 0 );
	*this *= ( 1.0f / length );
	return *this;
}


//------------------------------------------------------------------------------------------------------------------
#endif
