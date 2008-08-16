//------------------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "Vector.h"
#include <math.h>


//------------------------------------------------------------------------------------------------------------------
cVector cVector::ZERO;


//------------------------------------------------------------------------------------------------------------------
cVector::cVector() 
{
	cVector( 0.0f, 0.0f, 0.0f );
}


//------------------------------------------------------------------------------------------------------------------
 cVector::cVector( float pX, float pY, float pZ ) :
 m_X( pX ),
 m_Y( pY ),
 m_Z( pZ )
{
}


//------------------------------------------------------------------------------------------------------------------
cVector::cVector( cVector &pVec )
{
	m_X = pVec.m_X;
	m_Y = pVec.m_Y;
	m_Z = pVec.m_Z;
	//cVector( pVec.m_X, pVec.m_Y, pVec.m_Z );
}


//------------------------------------------------------------------------------------------------------------------
float cVector::Dot( const cVector &pVec )
{
	cVector resultVec;
	resultVec.m_X = m_X * pVec.m_X;
	resultVec.m_Y = m_Y * pVec.m_Y;
	resultVec.m_Z = m_Z * pVec.m_Z;
	return ( resultVec.m_X + resultVec.m_Y + resultVec.m_Z );
}


//------------------------------------------------------------------------------------------------------------------
cVector cVector::Cross( const cVector & pVec )
{
	return cVector(	m_Y * pVec.m_Z  -  m_Z * pVec.m_Y,
					m_Z * pVec.m_X  -  m_X * pVec.m_Z,
					m_X * pVec.m_Y  -  m_Y * pVec.m_X );
}


//------------------------------------------------------------------------------------------------------------------
//Directx Matrix presenation
//mat.M11 = 1.0f;    mat.M12 = 0.0f;    mat.M13 = 0.0f;    mat.M14 = 0.0f;
//mat.M21 = 0.0f;    mat.M22 = 1.0f;    mat.M23 = 0.0f;    mat.M24 = 0.0f;
//mat.M31 = 0.0f;    mat.M32 = 0.0f;    mat.M33 = 1.0f;    mat.M34 = 0.0f;
//mat.M41 = p_x;     mat.M42 = p_y;     mat.M43 = p_z;     mat.M44 = 1.0f;

// Return vector rotated by the 3x3 portion of matrix
cVector cVector::RotateByMatrix(const float pMat[16]) const
{
	return cVector(	m_X * pMat[0] + m_Y * pMat[4] + m_Z * pMat[8],
					m_X * pMat[1] + m_Y * pMat[5] + m_Z * pMat[9],
					m_X * pMat[2] + m_Y * pMat[6] + m_Z * pMat[10]
				  );	
}


//------------------------------------------------------------------------------------------------------------------
void cVector::UnitTestFunction()
{
	cVector Test1( 1.0f, 0.0f, 0.0f );
	cVector Test2( 0.0f, 1.0f, 0.0f );
	Test1 *= 0.5f;
	cVector Test3 = Test2.Cross( Test1 ) ;
	Log( "\n" );
}


//
//void CVector4::CalculateTangentVector(CVector4 Point1, CVector4 Point2, CVector4 Point3,
//                                      CVector4 NormalOfA, CTexCoord P1Tex, CTexCoord P2Tex,
//                                      CTexCoord P3Tex)
//{
//   // Get the vector between point 1 and point 2.
//   CVector4 VectorAB = Point2 - Point1;
//
//   // Get the vector between point 1 and point 3.
//	CVector4 VectorAC = Point3 - Point1;
//
//   // Compute the components of the vectors to the vertex normal of the first point.
//   CVector4 ProjAB = VectorAB - (NormalOfA * NormalOfA.DotProduct4(VectorAB));
//   CVector4 ProjAC = VectorAC - (NormalOfA * NormalOfA.DotProduct4(VectorAC));
//
//   // Calculate the tu difference of point 2 and 1 then of point 3 and 1.
//   float TexCoorUAB = P2Tex.tu - P1Tex.tu;
//   float TexCoorUAC = P3Tex.tu - P1Tex.tu;
//
//	// Calculate the tv difference of point 2 and 1 then of point 3 and 1.
//   float TexCoorVAB = P2Tex.tv - P1Tex.tv;
//   float TexCoorVAC = P3Tex.tv - P1Tex.tv;
//
//   // Switch the sign if the u of point 1 and 3 * v of 1 and 2 is greater than u of 1 and 2 *
//   // v of point 1 and 3.
//   if((TexCoorUAC * TexCoorVAB) > (TexCoorUAB * TexCoorVAC))
//      {
//         TexCoorUAC = -TexCoorUAC;
//         TexCoorUAB = -TexCoorUAB;
//      }
//
//   // Calculate the tangent vector, normalize it, then return it (the normal values).
//   CVector4 Tangent = (ProjAB * TexCoorUAC) - (ProjAC * TexCoorUAB);
//   Tangent.Normal();
//
//   *this += Tangent;
//}
//
//
//void CVector4::ExtendVertexPos(CVector4 currentVertex, CVector4 lightPos, float Extend)
//{
//    CVector4 lightDir;  // Direction of the light to the vertex position.
//    CVector4 newPos;    // New extended vertex position to make up the shadow volume.
//
//    // Get the light direction from the vertex position and the light position.
//    lightDir = currentVertex - lightPos;
//
//    // Now that we know where its going we add it to the position of the light so
//    // we get the correct, new position.  We multiply it by a passed it value to
//    // give the volume some distance or things won't come out quite as we want.
//    newPos = lightPos + lightDir * Extend;
//
//    x = newPos.x;
//    y = newPos.y;
//    z = newPos.z;
//    w = newPos.w;
//}
//
//
//void CVector4::ExtendVertexPos(CVector4 lightPos, float Extend)
//{
//    CVector4 lightDir;  // Direction of the light to the vertex position.
//    CVector4 newPos;    // New extended vertex position to make up the shadow volume.
//
//    // Get the light direction from the vertex position and the light position.
//    lightDir = CVector4(x, y, z, w) - lightPos;
//
//    // Now that we know where its going we add it to the position of the light so
//    // we get the correct, new position.  We multiply it by a passed it value to
//    // give the volume some distance or things won't come out quite as we want.
//    newPos = lightPos + lightDir * Extend;
//
//    x = newPos.x;
//    y = newPos.y;
//    z = newPos.z;
//    w = newPos.w;
//}
//
//
//CVector4 CVector4::GetRotatedX(double angle)
//{
//	return CVector4(x, y * (float)cos(angle) - z * (float)sin(angle),
//				       y * (float)sin(angle) + z * (float)cos(angle));
//}
//
//
//CVector4 CVector4::GetRotatedY(double angle)
//{
//	return CVector4(x * (float)cos(angle) + z * (float)sin(angle),
//				       y, -x * (float)sin(angle) + z * (float)cos(angle));
//}
//
//
//CVector4 CVector4::GetRotatedZ(double angle)
//{
//	return CVector4(x * (float)cos(angle) - y * (float)sin(angle),
//				       x * (float)sin(angle) + y * (float)cos(angle), z);
//}
//
//
//CVector4 CVector4::GetRotatedAxis(double angle, CVector4 axis)
//{
//	if(angle == 0.0) return(*this);
//
//   axis.Normal();
//
//	CVector4 RotationRow1, RotationRow2, RotationRow3;
//
//   double newAngle = M_PI * angle / 180;
//	float sinAngle = (float)sin(newAngle);
//	float cosAngle = (float)cos(newAngle);
//	float oneSubCos = 1.0f - cosAngle;
//
//	RotationRow1.x = (axis.x) * (axis.x) + cosAngle * (1 - (axis.x) * (axis.x));
//	RotationRow1.y = (axis.x) * (axis.y) * (oneSubCos) - sinAngle * axis.z;
//	RotationRow1.z = (axis.x) * (axis.z) * (oneSubCos) + sinAngle * axis.y;
//
//	RotationRow2.x = (axis.x) * (axis.y) * (oneSubCos) + sinAngle * axis.z;
//	RotationRow2.y = (axis.y) * (axis.y) + cosAngle * (1 - (axis.y) * (axis.y));
//	RotationRow2.z = (axis.y) * (axis.z) * (oneSubCos) - sinAngle * axis.x;
//	
//	RotationRow3.x = (axis.x) * (axis.z) * (oneSubCos) - sinAngle * axis.y;
//	RotationRow3.y = (axis.y) * (axis.z) * (oneSubCos) + sinAngle * axis.x;
//	RotationRow3.z = (axis.z) * (axis.z) + cosAngle * (1 - (axis.z) * (axis.z));
//
//	return CVector4(this->DotProduct3(RotationRow1),
//                   this->DotProduct3(RotationRow2),
//                   this->DotProduct3(RotationRow3));
//}
//
//
//void CVector4::CalculateBinormalVector(CVector4 tangent, CVector4 normal)
//{
//   this->CrossProduct(tangent, normal);
//}
//
//
//void CVector4::ClampTo01()
//{
//   CVector4 temp(x, y, z, w);
//   temp.Normal();
//   temp = temp * 0.5f + CVector4(0.5f, 0.5f, 0.5f);
//
//   x = temp.x;
//   y = temp.y;
//   z = temp.z;
//}
