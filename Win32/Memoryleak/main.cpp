#include <stdio.h>
#include <Windows.h>

#define SIMPLE_CHECK

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}



//#define DEBUG_NEW new(_CLIENT_BLOCK ,__FILE__, __LINE__)  
//#define new DEBUG_NEW 
#include "debugnew.h"

int main(int argc, char* argv[])
{
#ifdef SIMPLE_CHECK
	EnableMemLeakCheck();
#endif
	_CrtSetBreakAlloc(61);
	new int;
	new short[5];
	
	getchar();
	return 0;
}
