//------------------------------------------------------------------------------------------------------------------
#include <windows.h>
#include <new>
#include <list>
#include <crtdbg.h>
#include "UtilityFunction.h"
#include "MemoryManager.h"
#include <dbghelp.h>


//------------------------------------------------------------------------------------------------------------------
#if defined( _DEBUG ) | defined ( DEBUG )
#define DETECT_MEMORY_LEAK 0
#endif


//------------------------------------------------------------------------------------------------------------------
//disable warning c4996
#pragma  warning ( disable : 4996 )


//------------------------------------------------------------------------------------------------------------------
#pragma comment(lib,"dbghelp.lib")


//------------------------------------------------------------------------------------------------------------------
// Get the RtlCaptureContext function at runtime so we can support older SDKs
// and machines without it
typedef VOID (WINAPI *LPRtlCaptureContext)(PCONTEXT ContextRecord);


//------------------------------------------------------------------------------------------------------------------
//copied from msdn
/*
* For diagnostic purpose, blocks are allocated with extra information and
* stored in a doubly-linked list. This makes all blocks registered with
* how big they are, when they were allocated, and what they are used for.
*/
#define nNoMansLandSize 4


//------------------------------------------------------------------------------------------------------------------
typedef struct _CrtMemBlockHeader
{
struct _CrtMemBlockHeader * pBlockHeaderNext;
struct _CrtMemBlockHeader * pBlockHeaderPrev;
char * szFileName;
int nLine;
#ifdef _WIN64
/* These items are reversed on Win64 to eliminate gaps in the struct
* and ensure that sizeof(struct)%16 == 0, so 16-byte alignment is
* maintained in the debug heap.
*/
int nBlockUse;
size_t nDataSize;
#else /* _WIN64 */
size_t nDataSize;
int nBlockUse;
#endif /* _WIN64 */
long lRequest;
unsigned char gap[nNoMansLandSize];
/* followed by:
* unsigned char data[nDataSize];
* unsigned char anotherGap[nNoMansLandSize];
*/
} _CrtMemBlockHeader;


//------------------------------------------------------------------------------------------------------------------
#define pbData(pblock) ((unsigned char *)((_CrtMemBlockHeader *)pblock + 1))
#define pHdr(pbData) (((_CrtMemBlockHeader *)pbData)-1)


//------------------------------------------------------------------------------------------------------------------
cMemoryManager g_MemoryManager;


//------------------------------------------------------------------------------------------------------------------
cMemoryManager::cMemoryManager()
{
#if DETECT_MEMORY_LEAK
//  _CrtMemCheckpoint( &m_InitalMemState ) ; //take the memory snapshot
	m_EnableTrack = true;
	m_NumberOfAlloc = 0;
	m_TotalAllocSize = 0;
	m_OriginalHookFunction = _CrtSetAllocHook( cMemoryManager::StaticAllocHook );
	SymInitialize( GetCurrentProcess(), NULL, TRUE );
#endif
}


//------------------------------------------------------------------------------------------------------------------
cMemoryManager::~cMemoryManager()
{
	m_EnableTrack = false;
	unsigned int UnfreedNum = m_AllocList.size();
	if( UnfreedNum > 0 )
	{
		DumpUnfreed();
	}
	else
	{
		Log( " No Memory Leak detected\n" );
	}
	
	//_CrtMemState FinalMemState; // holds the memory states
	//_CrtMemCheckpoint( &m_InitalMemState1 ) ; //take the memory snapshot
	Assert( UnfreedNum <= 0, " Memory Leak detected\n"  );
	_CrtSetAllocHook( m_OriginalHookFunction );
	SymCleanup( GetCurrentProcess() );
}


//------------------------------------------------------------------------------------------------------------------
void cMemoryManager::AddTrack( long pRequestNum, size_t pAllocSize )
{
	m_EnableTrack = false;
	sAlloc_Info * memInfo = NULL;
	memInfo = new ( sAlloc_Info );
	memInfo->m_RequestID = pRequestNum;
	memInfo->m_AllocSize = pAllocSize;
	m_TotalAllocSize += pAllocSize;
	m_AllocList.insert( m_AllocList.begin(), memInfo );
	StatckTrace( memInfo );
	m_EnableTrack = true;
	m_NumberOfAlloc++;
}


//------------------------------------------------------------------------------------------------------------------
void cMemoryManager::RemoveTrack( long pRequestNum )
{
	m_EnableTrack = false;
	for( tAllocList::iterator memIt = m_AllocList.begin(); memIt != m_AllocList.end(); memIt++ )
	{
		sAlloc_Info * allocMem = * memIt;
		if( allocMem->m_RequestID == pRequestNum )
		{
			m_AllocList.remove( allocMem );
			break;
		}
	}
	m_EnableTrack = true;
}


//------------------------------------------------------------------------------------------------------------------
void cMemoryManager::DumpUnfreed()
{
  Log( "====================================================================================\n" );
  Log( " Memory Leak detected\n" );
  Log( " NumOfAllocation    AllocationSize \t\t\n" );
  Log( "   %d                 %d \n", m_NumberOfAlloc, m_TotalAllocSize );
  Log( "------------------------------------------------------------------------------------\n" );

  for( tAllocList::iterator It = m_AllocList.begin(); It != m_AllocList.end(); It++ )
  {
	  sAlloc_Info * memInfo = *It;
	  const char * strMemInfo = GetMemoryAllocatorInfo( memInfo );
	  Log( " %d bytes is not freed in File: %s \n", memInfo->m_AllocSize, strMemInfo  );
  }

  Log( "====================================================================================\n" );
}


//------------------------------------------------------------------------------------------------------------------
int __cdecl cMemoryManager::StaticAllocHook( int pAllocType, void *pUsrdata, size_t pSize, int pBlockUse, 
									long pRequest, const unsigned char *pSzFileName, int pLine )
{
    if ( pBlockUse == _CRT_BLOCK )
        return( TRUE );

	if( g_MemoryManager.m_EnableTrack  == false )
		return TRUE;

	switch( pAllocType )
	{
		case _HOOK_ALLOC:
			g_MemoryManager.AddTrack( pRequest, pSize );
			//Log( " Allocation is called \n" );
			break;

		case _HOOK_REALLOC:
		case _HOOK_FREE:
			_CrtMemBlockHeader* memHeader = pHdr(pUsrdata);
			size_t  size = memHeader->nDataSize;
			long requestId = memHeader->lRequest;
			g_MemoryManager.RemoveTrack( requestId );
			//Log( "FreeAllocation is called\n" );
			break;
	}

	return TRUE;
}


//------------------------------------------------------------------------------------------------------------------
void cMemoryManager::StatckTrace( sAlloc_Info * memAlloc )
{
	// See if we can get RtlCaptureContext
	LPRtlCaptureContext pfnRtlCaptureContext = (LPRtlCaptureContext)
		GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "RtlCaptureContext");
	if(!pfnRtlCaptureContext)
	{
		// RtlCaptureContext not supported
		return;
	}

	// Capture context
	CONTEXT ctx;
	pfnRtlCaptureContext( &ctx );

	// Init the stack frame for this function
	STACKFRAME64 theStackFrame;
	memset( &theStackFrame, 0, sizeof( theStackFrame ) );
	#ifdef _M_IX86
		DWORD dwMachineType = IMAGE_FILE_MACHINE_I386;
		theStackFrame.AddrPC.Offset = ctx.Eip;
		theStackFrame.AddrPC.Mode = AddrModeFlat;
		theStackFrame.AddrFrame.Offset = ctx.Ebp;
		theStackFrame.AddrFrame.Mode = AddrModeFlat;
		theStackFrame.AddrStack.Offset = ctx.Esp;
		theStackFrame.AddrStack.Mode = AddrModeFlat;
	#elif _M_X64
		DWORD dwMachineType = IMAGE_FILE_MACHINE_AMD64;
		theStackFrame.AddrPC.Offset = ctx.Rip;
		theStackFrame.AddrPC.Mode = AddrModeFlat;
		theStackFrame.AddrFrame.Offset = ctx.Rsp;
		theStackFrame.AddrFrame.Mode = AddrModeFlat;
		theStackFrame.AddrStack.Offset = ctx.Rsp;
		theStackFrame.AddrStack.Mode = AddrModeFlat;
	#elif _M_IA64
		DWORD dwMachineType = IMAGE_FILE_MACHINE_IA64;
		theStackFrame.AddrPC.Offset = ctx.StIIP;
		theStackFrame.AddrPC.Mode = AddrModeFlat;
		theStackFrame.AddrFrame.Offset = ctx.IntSp;
		theStackFrame.AddrFrame.Mode = AddrModeFlat;
		theStackFrame.AddrBStore.Offset = ctx.RsBSP;
		theStackFrame.AddrBStore.Mode = AddrModeFlat;
		theStackFrame.AddrStack.Offset = ctx.IntSp;
		theStackFrame.AddrStack.Mode = AddrModeFlat;
	#else
	#	error "Platform not supported!"
	#endif

	// Walk up the stack
	memset( memAlloc->m_AddrPC, 0, sizeof( memAlloc->m_AddrPC ));
	for( int idx = 0; idx< sAlloc_Info::MaxStackFrames; ++idx )
	{
		memAlloc->m_AddrPC[idx] = theStackFrame.AddrPC.Offset;
		if( !StackWalk64( dwMachineType, GetCurrentProcess(), GetCurrentThread(), &theStackFrame,
						  &ctx, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL))
		{
			break;
		}
	}

	 //tricky to surpress the W4 warning "unreferenced parameter"
	//UNREFERENCED_PARAMETER( memAlloc );
}


//------------------------------------------------------------------------------------------------------------------
const char * cMemoryManager::GetMemoryAllocatorInfo( sAlloc_Info * memAlloc )
{
	const size_t cnBufferSize = 512;
	char szFile[ cnBufferSize ];
	char szFunc[ cnBufferSize ];
	unsigned int nLine;
	static char szBuff[ cnBufferSize*3 ];

	// Initialise allocation source
	strcpy( szFile, "??" );
	nLine = 0;

	// Resolve PC to function names
	DWORD64 nPC;
	for( int idx = 0; idx < sAlloc_Info::MaxStackFrames; ++idx )
	{
		// Check for end of stack walk
		nPC = memAlloc->m_AddrPC[idx];
		if( nPC == 0 )
			break;

		// Get function name
		unsigned char byBuffer[ sizeof( IMAGEHLP_SYMBOL64 ) + cnBufferSize ];
		IMAGEHLP_SYMBOL64 * pSymbol = ( IMAGEHLP_SYMBOL64* ) byBuffer;
		DWORD64 dwDisplacement;
		memset( pSymbol, 0, sizeof( IMAGEHLP_SYMBOL64 ) + cnBufferSize );
		pSymbol->SizeOfStruct = sizeof( IMAGEHLP_SYMBOL64 );
		pSymbol->MaxNameLength = cnBufferSize;
		if( !SymGetSymFromAddr64( GetCurrentProcess(), nPC, &dwDisplacement, pSymbol ) )
		{
			//function name not found
			strcpy(szFunc, "??");
		}
		else
		{
			pSymbol->Name[cnBufferSize-1] = '\0';
			// See if we need to go further up the stack
			if( strncmp( pSymbol->Name, "cMemoryManager::", 16 ) == 0)
			{
				// In MemoryManager, keep going...
			}
			else if( strncmp( pSymbol->Name, "operator new", 12 ) == 0)
			{
				// In operator new or new[], keep going...
			}
			else if( strncmp( pSymbol->Name, "malloc_deb", 10 ) == 0 )
			{
				//malloc called
			}
			else if( strncmp( pSymbol->Name, "malloc", 6 ) == 0 )
			{
				// keep going
			}
			else if( strncmp( pSymbol->Name, "std::", 5 ) == 0)
			{
				// In STL code, keep going...
			}
			else
			{
				// Found the allocator (Or near to it)
				strcpy( szFunc, pSymbol->Name );
				break;
			}
		}
	}

	// Get file/line number
	if( nPC != 0 )
	{
		IMAGEHLP_LINE64 theLine;
		DWORD dwDisplacement;
		memset( &theLine, 0, sizeof( theLine ) );
		theLine.SizeOfStruct = sizeof( theLine );
		if(!SymGetLineFromAddr64( GetCurrentProcess(), nPC, &dwDisplacement, &theLine ) )
		{
			strcpy( szFile, "??" );
			nLine = 0;
		}
		else
		{
			const char* pszFile = strrchr( theLine.FileName, '\\');
			if( !pszFile )
			{
				pszFile = theLine.FileName;
			}
			else 
			{ 
				++pszFile;
			}

			strncpy( szFile, pszFile, cnBufferSize );
			nLine = theLine.LineNumber;
		}
	}

	// Format into buffer and return
	sprintf( szBuff, "%s:%d (%s)", szFile, nLine, szFunc );
	return szBuff;
}