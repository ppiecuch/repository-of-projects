#include <stdio.h>
#include <windows.h>
#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

#include <stdio.h>
#include <wtypes.h>
#include <stdarg.h>
#include <tchar.h>

inline void TRACE(const char * pszFormat, ...)
{
	va_list pArgs;
	char szMessageBuffer[16380]={0};
	va_start( pArgs, pszFormat );
	vsnprintf_s( szMessageBuffer, 16380,16380-1,pszFormat, pArgs );
	va_end( pArgs );   
	OutputDebugStringA(szMessageBuffer);   
}

int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); // ��ȡ��׼����豸���
	SetConsoleTitleA("Bank System"); // ��ȡ���ڱ���
	//SetConsoleTextAttribute(hOut,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_BLUE );//���ô�������
	SetConsoleTextAttribute(hOut,BACKGROUND_BLUE);
	char fpath[_MAX_PATH];
	_fullpath(fpath,"./", _MAX_PATH);
	printf(fpath);
	TRACE("test\n");
	printf("\ntest\n");
	getchar();
	return 0;
}