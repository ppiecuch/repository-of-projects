#include <string>

#include <stdio.h>
#include <windows.h>
#include <dbghelp.h>

#pragma comment(lib, "Dbghelp.lib")

#define _INNER_TEST 1

#if _DEBUG & !defined( _INNER_TEST )
#define GS_EXCEPTION_BEGIN()
#define    GS_EXCEPTION_END()
#else
#define EXCEPTION_BEGIN() __try {
#define    EXCEPTION_END() } __except( MyUnhandledFilter( GetExceptionInformation() ) ) {}
#endif




LONG WINAPI MyUnhandledFilter(struct _EXCEPTION_POINTERS * lpExceptionInfo)
{
	LONG ret = EXCEPTION_EXECUTE_HANDLER;

	TCHAR szFileName[64];
	SYSTEMTIME st;
	::GetLocalTime(&st);
	wsprintf(szFileName, TEXT("%04d-%02d-%02d-%02d-%02d-%02d-%02d-%02d.dmp"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, /*rand()*/100);

	HANDLE hFile = ::CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if (hFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION ExInfo;

		ExInfo.ThreadId = ::GetCurrentThreadId();
		ExInfo.ExceptionPointers = lpExceptionInfo;
		ExInfo.ClientPointers = false;

		BOOL bOK = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL );

		if (bOK)
		{
			printf("Create Dump File Success!\n");
		}
		else
		{
			printf("MiniDumpWriteDump Failed: %d\n", GetLastError());
		}

		::CloseHandle(hFile);
	}
	else
	{
		printf("Create File %s Failed %d\n", szFileName, GetLastError());
	}

	std::string strTip;
	switch( lpExceptionInfo->ExceptionRecord->ExceptionCode )
	{
	case STATUS_ACCESS_VIOLATION:
		{
			char sz[200] = {0};
			sprintf( sz, "内存访问冲突 Access violation, Attempt to %s data at address %p",  lpExceptionInfo->ExceptionRecord->ExceptionInformation[0] ? "write" : "read", lpExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
			strTip = sz;
		}
		break;

	case STATUS_BREAKPOINT:
		strTip = "用户断点 Breakpoint";
		break;

	case STATUS_DATATYPE_MISALIGNMENT:
		strTip = "内存地址对齐错误 Address misaligned";
		break;
#ifdef STATUS_FLOATING_DIVIDE_BY_ZERO
	case STATUS_FLOATING_DIVIDE_BY_ZERO:
		strTip = L"浮点数除数为0 Floating point divide by 0";
		break;

	case STATUS_FLOATING_OVERFLOW:
		strTip = L"浮点数溢出 Floating point overflow" ;
		break;

	case STATUS_FLOATING_UNDERFLOW:
		strTip = L"浮点数溢出 Floating point underflow";
		break;

	case STATUS_FLOATING_RESEVERED_OPERAND:
		strTip = L"保留的浮点数格式 Reserved Floating point format" ;
		break;
#endif

	case STATUS_ILLEGAL_INSTRUCTION:
		strTip = "非法指令 Illegal instruction" ;
		break;

	case STATUS_PRIVILEGED_INSTRUCTION:
		strTip = "特权指令 Priviledged instruction" ;
		break;

	case STATUS_INTEGER_DIVIDE_BY_ZERO:
		strTip = "0除数错误 Integer divide by 0" ;
		break;

	case STATUS_INTEGER_OVERFLOW:
		strTip = "整数溢出 Integer overflow" ;
		break;

	case STATUS_SINGLE_STEP:
		strTip = "单步执行 Single step" ;
		break;

	default:
		strTip = "未定义错误 Undefined error.";
	}

	printf( "发生错误的位置是 %p \n %s\n", lpExceptionInfo->ExceptionRecord->ExceptionAddress, strTip.c_str() );

	return ret;
}

//refer to::http://www.cppblog.com/zgysx/archive/2008/07/23/56926.html

int main(int argc, char* argv[])
{
	::SetUnhandledExceptionFilter(MyUnhandledFilter);

	EXCEPTION_BEGIN()
		int a = 0;
	int b = 2;
	//int c = b/a;
	int * p = (int *)1;
	*p = 2;
	strcpy( NULL, NULL );

	EXCEPTION_END()
		return 0;
}