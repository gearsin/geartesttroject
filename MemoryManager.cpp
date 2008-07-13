//------------------------------------------------------------------------------------------------------------------
#include <windows.h>
#include <new>
#include <list>
#include "UtilityFunction.h"
#include "MemoryManager.h"


//------------------------------------------------------------------------------------------------------------------
#undef	new
#undef	delete
#undef	malloc
#undef	calloc
#undef	realloc
#undef	free


//------------------------------------------------------------------------------------------------------------------
cMemoryManager g_MemoryManager;


//------------------------------------------------------------------------------------------------------------------
//override new and delete 
#if defined( _DEBUG ) | defined( DEBUG )
void * operator new( size_t pReportedSize )
{
	void *memPtr = ( void* ) malloc( pReportedSize );
	Assert( !!memPtr, "Memory allocfailed!" );
	g_MemoryManager.AddTrack( (DWORD)memPtr, pReportedSize, "Test", 1/*pFile, pLineNum*/ );
	return memPtr;
};


//------------------------------------------------------------------------------------------------------------------ 
void operator delete( void *pAddr )
{
	g_MemoryManager.RemoveTrack( (DWORD)pAddr );
	free( pAddr );
};

//------------------------------------------------------------------------------------------------------------------ 
#endif


//------------------------------------------------------------------------------------------------------------------
cMemoryManager::cMemoryManager()
{
	void * pMemBuff = malloc ( sizeof( tAllocList ) );
	m_AllocList = (cMemoryManager::tAllocList*) pMemBuff;
}


//------------------------------------------------------------------------------------------------------------------
cMemoryManager::~cMemoryManager()
{
	DumpUnfreed();
	Assert( m_AllocList->size() != 0 , "Memory Leak Detected" );
}


//------------------------------------------------------------------------------------------------------------------
void cMemoryManager::AddTrack( DWORD pAddr, DWORD pSize, const char *pFname, DWORD pLineNum )
{
	ALLOC_INFO * memInfo = NULL;
	if( !m_AllocList )
	{
		return ;
		//m_AllocList = new ( tAllocList );
	}

	memInfo = new ( ALLOC_INFO );
	memInfo->m_Address = pAddr;
	memInfo->m_LineNum = pLineNum;
	memInfo->m_Size = pSize;
	strcpy_s( memInfo->m_File, pFname );
	m_AllocList->insert( m_AllocList->begin(), memInfo );
}


//------------------------------------------------------------------------------------------------------------------
void cMemoryManager::RemoveTrack( DWORD pAddr )
{
	if( !m_AllocList )
	{
		return;
	}

	unsigned int testCount = 0;
	for( tAllocList::iterator It = m_AllocList->begin(); It != m_AllocList->end(); It++ )
	{
		testCount++;
		if( (*It)->m_Address == pAddr )
		{
			m_AllocList->remove( (*It) );
			break;
		}
	}
}


//------------------------------------------------------------------------------------------------------------------
void cMemoryManager::DumpUnfreed()
{
  if( !m_AllocList )
  {
	  return;
  }

  DWORD totalSize = 0;
  char strBuff[1024];
  Log( false, "\n====================================================================================" );
  for( tAllocList::iterator It = m_AllocList->begin(); It != m_AllocList->end(); It++ )
  {
	  ALLOC_INFO * memInfo = *It;
	  sprintf_s( strBuff, "\nLeak in File: %-50s: (%d),\t\tAddress: %d, \tUnfreed: %d", memInfo->m_File, memInfo->m_LineNum, 
								memInfo->m_Address, memInfo->m_Size );
	  Log( false, strBuff );
	  totalSize += memInfo->m_Size;
  }
  Log( false, "\n====================================================================================" );
}


//------------------------------------------------------------------------------------------------------------------
void SetNewOwner( const char * pFileName, const char * pFunctionName, int pLineNum )
{

	Log( false, "\n Test Success" );
}












//Absolute functions
#if DETECT_MEMORY_LEAK
#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h>
#include <crtdbg.h>
#endif

//------------------------------------------------------------------------------------------------------------------
void cMemoryManager::FindMemoryLeak( int pBreakAlloc )
{
#ifdef DETECT_MEMORY_LEAK
#if defined( _DEBUG ) | defined( DEBUG )
	_crtBreakAlloc=-1;

	// Get current flag
	int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );
	// Turn on leak-checking bit.
	tmpFlag |= _CRTDBG_LEAK_CHECK_DF;	
	tmpFlag |= _CRTDBG_ALLOC_MEM_DF;
	tmpFlag |= _CRTDBG_CHECK_ALWAYS_DF;
	tmpFlag |= _CRTDBG_REPORT_FLAG;

	_CrtSetDbgFlag( tmpFlag );
#endif
#endif
}


//------------------------------------------------------------------------------------------------------------------
void cMemoryManager::GenerateMemoryLeakReport()
{
#ifdef DETECT_MEMORY_LEAK
#if defined( DEBUG ) | defined ( _DEBUG ) 
	_CrtDumpMemoryLeaks();
#endif
#endif
}
