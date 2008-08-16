//------------------------------------------------------------------------------------------------------------------
#ifndef UTILITY_FUNC_H
#define UTILITY_FUNC_H


//------------------------------------------------------------------------------------------------------------------
void Assert( bool pExpression, const char * pMsg, ... );
void Log( const WCHAR * pFormat, ... );//Unicode
void Log( const char * pFormat, ...);
char * TrimString( char * pStr );
float * GetVectorFromString( std::string pStr );


//------------------------------------------------------------------------------------------------------------------
#endif