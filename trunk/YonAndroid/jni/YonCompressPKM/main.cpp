#include "stdio.h"
#include<windows.h>

#include <crtdbg.h>
#include <typeinfo>

//#include <vld.h>

inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) |  _CRTDBG_LEAK_CHECK_DF);
}

#include <locale.h>

#ifdef YON_HIDE_CONSOLE
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif//YON_HIDE_CONSOLE

#include "framework.h"

int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

	//setlocale(LC_ALL,"chs");
	setlocale(LC_CTYPE,"UTF-8");

	try{
		init(NULL,800,480);
	}catch(...){
		Logger->error(YON_LOG_FAILED_FORMAT,"init engine failed!");
	}
	try{
		while(getEngine()->run()){
			drawFrame();
		}
	}catch(...){
		Logger->error(YON_LOG_FAILED_FORMAT,"run/draw engine failed!");
	}
	destroy();
}