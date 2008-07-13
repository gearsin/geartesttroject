//------------------------------------------------------------------------------------------------------------------
#ifndef MEM_LEAK_H
#define MEM_LEAK_H


//------------------------------------------------------------------------------------------------------------------
class cMemoryManager
{

public:
	cMemoryManager();
	~cMemoryManager();

	static void FindMemoryLeak( int pBreakAlloc );
	static void GenerateMemoryLeakReport();

	void AddTrack( DWORD pAddr,  DWORD pSize,  const char *pFname, DWORD pLineNum );
	void RemoveTrack( DWORD pAddr);
	void DumpUnfreed();

private:

      typedef struct 
	  {
	      DWORD	m_Address;
	      DWORD	m_Size;
	      char	m_File[64];
	      DWORD	m_LineNum;
      } ALLOC_INFO;
	 
	  typedef std::list< ALLOC_INFO* > tAllocList;
	  tAllocList * m_AllocList;
};


//------------------------------------------------------------------------------------------------------------------
extern cMemoryManager g_MemoryManager;



//------------------------------------------------------------------------------------------------------------------
void SetNewOwner( const char * pFileName, const char * pFunctionName, int pLineNum );


//------------------------------------------------------------------------------------------------------------------
#if defined ( DEBUG ) | defined( _DEBUG )


//------------------------------------------------------------------------------------------------------------------
void * operator new( size_t pReportedSize );
//void * operator new[]( size_t reportedSize );
void * operator new( size_t pReportedSize, const char *pSourceFile, const char * pFunctionName, int pSourceLine );
//void * operator new[]( size_t pReportedSize, const char *pSourceFile, int pSourceLine );


//------------------------------------------------------------------------------------------------------------------ 
void operator delete( void *pAddr );
void operator delete[](void *reportedAddress);


//------------------------------------------------------------------------------------------------------------------
#ifdef	new
#undef	new
#endif

#ifdef	delete
#undef	delete
#endif

#ifdef	malloc
#undef	malloc
#endif

#ifdef	calloc
#undef	calloc
#endif

#ifdef	realloc
#undef	realloc
#endif

#ifdef	free
#undef	free
#endif

#define	new		( SetNewOwner ( __FILE__, __FUNCTION__, __LINE__ ),false ) ? NULL : new
#endif

//#define new DEBUG_NEW


//------------------------------------------------------------------------------------------------------------------
#endif