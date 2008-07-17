//------------------------------------------------------------------------------------------------------------------
#include "StdAfx.h"


//------------------------------------------------------------------------------------------------------------------
void Assert( bool pExpression, const char * pMsg )
{
	if( !pExpression )
	{
		Log( pMsg );
		__debugbreak();
	}
}


//------------------------------------------------------------------------------------------------------------------
void Log( const char * pMsg, bool pWriteToFile )
{
	if( pWriteToFile )
	{
		//TODO:: Write to file implementation
	}
	OutputDebugStringA( pMsg );
}


//------------------------------------------------------------------------------------------------------------------
