//------------------------------------------------------------------------------------------------------------
#ifndef LEVEL_H
#define LEVEL_H


//------------------------------------------------------------------------------------------------------------
class cObject;
class CFirstPersonCamera;
class cFrustum;


//------------------------------------------------------------------------------------------------------------
typedef std::vector< cObject * > tObjectPtrVector;


//------------------------------------------------------------------------------------------------------------
class cLevel
{

public:
	cLevel( int pLevelID );
	cLevel( const WCHAR * pLevelName ); // for test purpose must be removed
	~cLevel();
	static void Create( int pLevelId  );
	static void Create( const WCHAR * pLevelName );// TODO
	void Update();
	CFirstPersonCamera * GetCamera();
	cFrustum * GetFrustum();
private:
	struct sObjectInfo
	{
		private:		
			tStringVector m_Tag;
			tStringVector m_Value;
		public:
			int GetSize();
			void Insert( std::string pTagName, std::string pVal );
			std::string GetTag( int pIdx );
			std::string GetValue( int pIdx );
			void GetObjectInfo( std::string & pTagName, std::string & pValue, int pAt );
	};
	sObjectInfo m_ObjectInfo;

private:
	void ReadLevelData( const WCHAR * pLevelName );
	void LoadLevelData();

private:
	std::wstring		 m_LevelName;
	tObjectPtrVector	 m_Objects;
	CFirstPersonCamera * m_CurrentCamera;
	cFrustum		   * m_Frustum;
	//tStringVector m_ObjectInfoTag;
	//tStringVector m_ObjectInfoValue;
};


//------------------------------------------------------------------------------------------------------------
inline int cLevel::sObjectInfo::GetSize()
{
	return m_Tag.size();
}


//------------------------------------------------------------------------------------------------------------
inline void cLevel::sObjectInfo::Insert( std::string pTagName, std::string pVal )
{
	m_Tag.push_back( pTagName );
	m_Value.push_back( pVal );
}


//------------------------------------------------------------------------------------------------------------
inline std::string cLevel::sObjectInfo::GetTag( int pIdx )
{
	return m_Tag[pIdx];
}


//------------------------------------------------------------------------------------------------------------
inline std::string cLevel::sObjectInfo::GetValue( int pIdx )
{
	return m_Value[pIdx];
}


//------------------------------------------------------------------------------------------------------------
inline void cLevel::sObjectInfo::GetObjectInfo( std::string & pTagName, std::string & pValue, int pAt )
{
	pTagName = GetTag( pAt );
	pValue = GetValue( pAt );
}


//------------------------------------------------------------------------------------------------------------
inline CFirstPersonCamera *  cLevel::GetCamera()
{
	return m_CurrentCamera;
}


//------------------------------------------------------------------------------------------------------------
inline cFrustum * cLevel::GetFrustum()
{
	return m_Frustum;
}


//------------------------------------------------------------------------------------------------------------
extern cLevel * g_Level;


//------------------------------------------------------------------------------------------------------------
#endif