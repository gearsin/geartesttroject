//------------------------------------------------------------------------------------------------------------------
#include "StdAfx.h"


//------------------------------------------------------------------------------------------------------------------
void Assert( bool pExpression, const char * pMsg, ... )
{
	if( !pExpression )
	{
		char szBuffer[1024];
 		va_list args;
		va_start( args, pMsg );
		vsprintf_s( szBuffer, pMsg, args );

		Log( szBuffer );
		__debugbreak();
	}
}


//------------------------------------------------------------------------------------------------------------------
void Log( const WCHAR * pFormat, ... )
{
	WCHAR szBuffer[1024];
 	va_list args;
	va_start( args, pFormat );
	vswprintf_s( szBuffer, pFormat, args );

	OutputDebugStringW( szBuffer );
}


//------------------------------------------------------------------------------------------------------------------
void Log( const char * pFormat, ... )
{
	char szBuffer[1024];
 	va_list args;
	va_start( args, pFormat );
	vsprintf_s( szBuffer, pFormat, args );

	OutputDebugStringA( szBuffer );
}


//------------------------------------------------------------------------------------------------------------------
char * TrimString( char * pStr )
{
	unsigned int strLen = strlen( pStr );
	int removeCharIdx = 0;
	//Left trim
	while( *( pStr + removeCharIdx ) == ' ' || 	*( pStr + removeCharIdx ) == '\t' )
	{
		removeCharIdx++;
	}

	//trim left
	unsigned int idx = 0;
	for( idx = 0; idx < strlen( pStr ); idx++ )
	{
		pStr[idx] = pStr[removeCharIdx + idx ];
	}

	strLen = strlen( pStr ) - 1;

	//Right trim
	while( *( pStr + strLen ) == ' ' || *( pStr + strLen ) == '\t' )
	{	
		strLen--;
	}
	
	pStr[strLen+1] = '\0';

return pStr;
}


//------------------------------------------------------------------------------------------------------------------
float * GetVectorFromString( std::string pStr )
{
	float vec[3];
	int seprator = pStr.find( ' ' );
	std::string strFloat = pStr.substr( 0, seprator );
	vec[0] = (float)atof( strFloat.c_str() );

	int seprator2 = pStr.find( ' ', seprator + 1 );
	strFloat = pStr.substr( seprator + 1, ( seprator2 - seprator ) );
	vec[1] = (float)atof( strFloat.c_str() );

	int seprator3 = pStr.find( ' ', seprator + 1 );
	strFloat = pStr.substr( seprator3, ( seprator3 - seprator2 ) );
	vec[2] = (float)atof( strFloat.c_str() );

	return vec;
}
