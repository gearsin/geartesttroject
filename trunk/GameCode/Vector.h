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
	cVector Dot( const cVector & pVec );
	cVector Cross( const cVector & pVec );

	//normalize
	inline cVector& Normalize();

	cVector RotateByMatrix( const float pMat[16] ) const;

//TestFunction
	 virtual void UnitTestFunction();
};


//------------------------------------------------------------------------------------------------------------------
#endif



//------------------------------------------------------------------------------------------------------------------
