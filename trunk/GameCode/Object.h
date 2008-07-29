//------------------------------------------------------------------------------------------------------------------
#ifndef OBJECT_H
#define OBJECT_H


//------------------------------------------------------------------------------------------------------------------
#include "Vector.h"


//------------------------------------------------------------------------------------------------------------------
class cMesh;


//------------------------------------------------------------------------------------------------------------------
class cObject
{

public:
	cObject( const WCHAR * pFileName );
	cObject( const WCHAR * pFileName, const cVector & pPos );
	~cObject();
	void SetTranslation( const cVector & pPos );
	void SetTranslation( const float pX, const float pY, const float pZ );
	void SetRotation( const cVector & pRot );
	void SetRotation( const float pRotX, const float pRotY, const float pRotZ );
	void SetScale( const float pScaleVal );
	void SetScale( const float pScaleX, const float pScaleY, const float pScaleZ );
	void Update();
	void Render(); //Obsolute shuld be removed

protected:
		
private:
	cVector		m_Translation;
	cVector		m_Rotation;
	cVector		m_Scale;
	cVector		m_BoxMin;	//Bounding box min
	cVector		m_BoxMax;	//bounding box max
	D3DMATRIX	m_WorldMat;
	cMesh	  * m_ModelMesh;
};


//------------------------------------------------------------------------------------------------------------------
#endif
