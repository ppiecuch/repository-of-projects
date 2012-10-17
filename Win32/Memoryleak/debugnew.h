#ifndef _DEBUG

#else

#ifndef _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#endif

#include <crtdbg.h>

#define CHECKMEMORY()     do{ \
	int nFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG ); \
	nFlag |= _CRTDBG_LEAK_CHECK_DF; \
	_CrtSetDbgFlag( nFlag ); \
						} while(0);

inline void * __cdecl operator new(unsigned int size,
								   const char *file, int line)
{
	CHECKMEMORY()
	return ::operator new(size, _NORMAL_BLOCK, file, line);
};

#define DEBUG_NEW new(__FILE__, __LINE__)
#define new DEBUG_NEW

#endif
