#include "lcLogger.h"

namespace lc{

#if defined(LC_OS_WINNT)||defined(LC_OS_WINRT)
	void trace(const lc::c8 * pszFormat, ...)
	{
		va_list pArgs;
		lc::c8 szMessageBuffer[16380]={0};
		va_start( pArgs, pszFormat );
		vsnprintf_s( szMessageBuffer, 16380,16380-1,pszFormat, pArgs);
		va_end( pArgs );   
		OutputDebugStringA(szMessageBuffer);   
	}
#endif
}