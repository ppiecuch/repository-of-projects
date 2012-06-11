#include "stdio.h"
#include<windows.h>

#include <crtdbg.h>
#include <typeinfo>

inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) |  _CRTDBG_LEAK_CHECK_DF);
}

#include <locale.h>

#ifdef XC_HIDE_CONSOLE
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif//XC_HIDE_CONSOLE

#include "framework.h"

int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

	//setlocale(LC_ALL,"chs");
	setlocale(LC_CTYPE,"UTF-8");

	init(NULL,400,400);
	while(getEngine()->run()){
		drawFrame();
	}
	destroy();
}