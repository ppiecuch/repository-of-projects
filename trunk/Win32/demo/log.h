#ifndef _DAECORE_LOG_H_
#define _DAECORE_LOG_H_

#include <stdio.h>
#include <wtypes.h>
#include <stdarg.h>
#include <tchar.h>


inline void Trace(LPCTSTR pszFormat, ...)
{
	va_list pArgs;
	char szMessageBuffer[16380]={0};
	va_start( pArgs, pszFormat );
	_vsntprintf_s( szMessageBuffer, 16380, pszFormat, pArgs );
	va_end( pArgs );   
	OutputDebugString(szMessageBuffer);   
}
#endif