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

	static int __cdecl StaticAllocHook( int pAllocType, void *pUdata, size_t pSize, int pBlockUse, 
										long pRequest, const unsigned char * pSzFileName, int pLine );

	void AddTrack( long pRequestNum, size_t pAllocSize );
	void RemoveTrack( long pRequestNum );
	void DumpUnfreed();

private:

      typedef struct 
	  {
		  long	m_RequestID;
		  size_t m_AllocSize;
		  static const size_t MaxStackFrames = 16;
		  DWORD64 m_AddrPC[ MaxStackFrames ]; //specify the program counter
      } sAlloc_Info;
	 
	  typedef std::list< sAlloc_Info* > tAllocList;
	  bool			m_EnableTrack;
	  unsigned int	m_NumberOfAlloc;
	  size_t  m_TotalAllocSize;
	  tAllocList	m_AllocList;
	  _CRT_ALLOC_HOOK m_OriginalHookFunction;
	 //_CrtMemState m_InitalMemState; // holds the memory states

private:
	void StatckTrace( sAlloc_Info * memAlloc );
	const char * GetMemoryAllocatorInfo( sAlloc_Info * memAlloc ); 

};


//------------------------------------------------------------------------------------------------------------------
extern cMemoryManager g_MemoryManager;


//------------------------------------------------------------------------------------------------------------------
#endif