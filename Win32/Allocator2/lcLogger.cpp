#include "lcLogger.h"

namespace lc{

	const c8* LC_LOG_LEVELS[]={"[DEBG]","[INFO]","[WARN]","[EROR]"};
	const s32 LC_LOG_TRACE_BUFFER_LEN=16380;
	const s32 LC_LOG_LEVELS_LEN=6;

#if defined(LC_OS_WIN32)
	void trace(s32 level,const c8 * pszFormat, ...)
	{
		//TODO
		//clamp(level)
		va_list pArgs;
		c8 szMessageBuffer[LC_LOG_TRACE_BUFFER_LEN]={0};
		strcat_s(szMessageBuffer,LC_LOG_TRACE_BUFFER_LEN,LC_LOG_LEVELS[level]);
		va_start(pArgs, pszFormat);
		vsnprintf_s(szMessageBuffer+LC_LOG_LEVELS_LEN, LC_LOG_TRACE_BUFFER_LEN-LC_LOG_LEVELS_LEN,LC_LOG_TRACE_BUFFER_LEN-1-LC_LOG_LEVELS_LEN,pszFormat, pArgs);
		va_end( pArgs );   
		OutputDebugStringA(szMessageBuffer);   
	}
#endif
}