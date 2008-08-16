//------------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "DXUTcamera.h"
#include <fstream>
#include "Vector.h"
#include "Object.h"
#include "Level.h"
#include "MathUtils.h"
#include "Frustum.h"


//------------------------------------------------------------------------------------------------------------
//const char * LevelNames[]
//{
//	
//};


//------------------------------------------------------------------------------------------------------------
cLevel * g_Level = NULL;


//------------------------------------------------------------------------------------------------------------
void cLevel::Create( int pLevelId )
{
	g_Level = new cLevel( pLevelId );
}


//------------------------------------------------------------------------------------------------------------
void cLevel::Create( const WCHAR *pLevelName )
{
	g_Level = new cLevel( pLevelName );		
}


//------------------------------------------------------------------------------------------------------------
cLevel::cLevel( int pLevelID )
{
}


//------------------------------------------------------------------------------------------------------------
cLevel::cLevel( const WCHAR *pLevelName )
{
	ReadLevelData( pLevelName );
	LoadLevelData();
	m_CurrentCamera = new CFirstPersonCamera();
	m_CurrentCamera->SetProjParams( DegToRad( 45.0f ), 1.0f, 1.0f, 60.0f );
	m_Frustum = new cFrustum( m_CurrentCamera->GetFov(), m_CurrentCamera->GetNearClip(), 
							  m_CurrentCamera->GetFarClip(), m_CurrentCamera->GetAspect() );

}


//------------------------------------------------------------------------------------------------------------
cLevel::~cLevel()
{
	for( tObjectPtrVector::iterator objIt = m_Objects.begin(); objIt != m_Objects.end(); ++objIt )
	{
		cObject * obj = *objIt;
		delete obj;
		obj = NULL;
	}

	if( m_CurrentCamera )
	{
		delete m_CurrentCamera;
		m_CurrentCamera = NULL;
	}

	if( m_Frustum )
	{
		delete m_Frustum;
		m_Frustum = NULL;
	}
	g_Level = NULL;
}


//------------------------------------------------------------------------------------------------------------
void cLevel::LoadLevelData()
{
	const int TAGCOUNT = 5;
	int objectArr = m_ObjectInfo.GetSize();
	int objectTagCnt = TAGCOUNT;
	for ( int objIdx = 0; objIdx < objectArr; objIdx++ )
	{
		WCHAR  modelFileName[128];
		cVector pos;
		cVector rotation;
		float w = 0.0f;
		std::string tagName, value;

		m_ObjectInfo.GetObjectInfo( tagName, value, objIdx );
		if( tagName.compare( "<type>" ) == 0 )
		{
			//yet to implement
			objectTagCnt--;
			continue;	
		}
		else if( tagName.compare( "<ModelFileName>" ) == 0 )
		{
			objectTagCnt--;
			mbstowcs( modelFileName, value.c_str(), strlen( value.c_str() ) + 1 );
		}
		else if( tagName.compare( "<Pos>" ) == 0 )
		{
			objectTagCnt--;
			float * tempPos = GetVectorFromString( value );
			pos.m_X = tempPos[0];
			pos.m_Y = tempPos[1];
			pos.m_Z = tempPos[2];
		}
		else if( tagName.compare( "<Rotation>" ) == 0 )
		{
			objectTagCnt--;
			//TODO:: convestion quat ==> matrix
			//float * tempRotXYZ = GetVectorFromString( value );
			//float wIdx = value.find_last_not_of( ' ' );
		}
		else if( tagName.compare( "<Scale>" ) == 0 )
		{
			objectTagCnt--;
			//TODO :: include the scale parameter
			//obsloute
		}

		if( objectTagCnt == 0 )
		{
			objectTagCnt = TAGCOUNT;
			//create the object
			WCHAR fullPath[128];
			swprintf( fullPath, LEVELS_DIR L"%s\\%s", m_LevelName.c_str(), modelFileName );
			char model[128];
			wcstombs( model, fullPath, wcslen ( fullPath ) + 1 );
			Log( "Loading model: %s\n", model );
			cObject * object = new cObject( fullPath );
			m_Objects.push_back( object );

//Place holder:: Render all object in every frame
//g_Renderer->AddToRenderList( object );
		}
	}
}


//------------------------------------------------------------------------------------------------------------
void cLevel::ReadLevelData( const WCHAR * pLevelName )
{
	WCHAR levelFilePath[256];
	//WCHAR levelDir[256];
	WCHAR exeDir[256];
	GetCurrentDirectory( 256, exeDir );
	m_LevelName = pLevelName;
	int forwardSlash = m_LevelName.find( '/' );
	m_LevelName = m_LevelName.substr( 0, forwardSlash );
	assert( forwardSlash > 0 );

	//wcsncpy( levelDir, pLevelName, wcslen( pLevelName ) - 4  );
	//levelDir[wcslen( pLevelName ) - 4] ='\0';
	swprintf( levelFilePath, L"%s"LEVELS_DIR L"%s", exeDir, pLevelName ); 
	std::ifstream inLevelFile( levelFilePath );
	Assert( inLevelFile.bad() == false, "Level file not Found\n" );	

//**********************************************************************************
		//TODO: improve the parser code to handel the file reading.
//**********************************************************************************

	tStringVector xmlNodes;
	std::string tagName;
	char  buffer[512];
	int lineNum = 1;
	int tagStart = 0, tagEnd = 0;
	int tag2Start = 0, tag2End = 0;

	bool searchValue = false;
	bool checkTagEnd = false;

	inLevelFile.getline( buffer, 200 );
	TrimString( buffer );
	std::string checkStr( buffer );
	int error = checkStr.compare( "<?xml version=\"1.0\" encoding=\"iso-8859-1\" ?>"  );
	Assert( !error, "Error At Line: %d\n", lineNum++ );

	while( !inLevelFile.eof() )
	{
		inLevelFile.getline( buffer, 200 );
		TrimString( buffer );
		std::string strWholeLine( buffer );

		//if balnk line
		if( strWholeLine.compare( "" ) == 0 )
			continue;

		//get the tag start
		if( searchValue == false && checkTagEnd == false )
		{
			tagStart = strWholeLine.find( '<' );
			tagEnd = strWholeLine.find( '>' );
			Assert( tagStart != -1 && tagEnd != -1, "Level file parse error at line: %d\n", lineNum );
			Assert( tagStart < tagEnd, "Level file parse error at line: %d\n", lineNum );
			tagName = strWholeLine.substr( tagStart, tagEnd - tagStart + 1 );
			xmlNodes.push_back( tagName );
//			Log( "TagFound: %s\n", tagName.c_str() );
		}
	
		if( tagName.compare( "<LevelData>" ) == 0 )
		{
			searchValue = false;
			continue;
		}
		else if( tagName.compare( "<Object>" ) == 0 )
		{
			searchValue = false;
			continue;
		}
		else if( tagName.compare( "</LevelData>" ) == 0 )
		{
			xmlNodes.pop_back();
			std::string checkTag = xmlNodes.back();
			Assert( checkTag.compare( "<LevelData>" ) == 0, "No enclosing tag line: %d", lineNum  );
			xmlNodes.pop_back();
			continue;
		}
		else if( tagName.compare( "</Object>" ) == 0 )
		{
			xmlNodes.pop_back();
			std::string checkTag = xmlNodes.back();
			Assert( checkTag.compare( "<Object>" ) == 0, "No enclosing tag Line: %d", lineNum );
			xmlNodes.pop_back();
			continue;
		}

		//get the tag end
		//check if enclosing tag also in the same line
		tag2Start = strWholeLine.find( "</", tagEnd );
		tag2End = strWholeLine.find( '>', tagEnd + 1 );
		//Assert( !( tag2Start != -1 && tag2End == -1 ), "Level parse error @ line: %d", lineNum );
		//Assert( !( tag2Start == -1 && tag2End != -1 ), "Level parse error @ line: %d", lineNum );
		Assert( tag2Start != -1 && tag2End != -1, "no enclosing tag\n" );
		std::string tagValue;
		if( checkTagEnd == false )
		{
			std::string startTagName = strWholeLine.substr( tagStart + 1, tagEnd - 1 );
			std::string endTagName = strWholeLine.substr( tag2Start + 2, 
												( tag2End - ( tag2Start + 2 )  )  );
			Assert( startTagName.compare( endTagName ) == 0, "Level:: tag name mismatch @ line: %d\n", lineNum );
			tagValue = strWholeLine.substr( tagEnd + 1, ( tag2Start - tagEnd ) - 1 );
			m_ObjectInfo.Insert( tagName, tagValue );
			xmlNodes.pop_back();
		}	
		lineNum++;
	}

	//load
	inLevelFile.close();
}


//------------------------------------------------------------------------------------------------------------
void cLevel::Update()
{
	m_CurrentCamera->FrameMove( 0.005f );
	m_Frustum->UpdateFrustum( m_CurrentCamera );
	for( tObjectPtrVector::iterator objIt = m_Objects.begin(); objIt != m_Objects.end(); ++objIt )
	{
		cObject * obj = *objIt;
if( wcsnicmp( obj->GetName(), L"PlayerStart", wcslen( L"PlayerStart" ) ) == 0 )
{
	cVector rot = obj->GetRotation();
	rot.m_Y += 0.1f;
	//obj->SetTranslation( cVector( 0.05f, 0.0f, 0.0f ) );
	obj->SetRotation( rot );
}

		obj->Update( m_Frustum );
	}
}