//----------------------------------------------------------------------------------------------------------
#ifndef MATHUTILS_H
#define MATHUTILS_H


//----------------------------------------------------------------------------------------------------------
#define MATH_PI 3.14159265358979323846f

//----------------------------------------------------------------------------------------------------------
#define DegToRad( x )  x * MATH_PI/180.0f
#define RadToDeg( x ) x * 180.0f/ MATH_PI


//----------------------------------------------------------------------------------------------------------
cVector D3DXVecToVector( const D3DXVECTOR3 & pVector );
D3DXVECTOR3 VectorToD3DXVec( const cVector & pVector );


//----------------------------------------------------------------------------------------------------------
#endif