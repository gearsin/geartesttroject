//------------------------------------------------------------------------------------------------------------------
#include "StdAfx.h"


//------------------------------------------------------------------------------------------------------------------
void Assert( bool pExpression, const char * pMsg )
{
	if( !pExpression )
	{
		Log( false, pMsg );
		__debugbreak();
	}
}


//------------------------------------------------------------------------------------------------------------------
void Log( bool pWriteToFile, const char * pMsg )
{
	if( pWriteToFile )
	{
		//TODO:: Write to file implementation
	}
	OutputDebugStringA( pMsg );
}


//------------------------------------------------------------------------------------------------------------------
