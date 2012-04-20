// logdemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}



#include "Log.h"




int _tmain(int argc, _TCHAR* argv[])
{
	EnableMemLeakCheck();

	Logger* logger=Logger::getInstance("test");

	logger->info("test:%s","helloworld");
	Logger::getInstance("test2")->debug("test:%s","helloworld");
	logger->warn("test:%s","helloworld");
	logger->error("test:%s","helloworld");
	logger->debug(L"test:%s",L"中国");
	Logger::getInstance("test2");


	//Log::Out(Log.OFile,"helloworld%s\r\n","test");
	getchar();
	return 0;
}

