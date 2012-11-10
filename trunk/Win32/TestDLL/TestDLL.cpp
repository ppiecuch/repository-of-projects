// TestDLL.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "test.h"
#pragma comment( lib, "DLLDemo.lib" )

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}




int _tmain(int argc, _TCHAR* argv[])
{
	EnableMemLeakCheck();
	testexam(NULL);
	int* p=testnew();
	delete p;
	Test t;
	t.test();
	getchar();
	return 0;
}

