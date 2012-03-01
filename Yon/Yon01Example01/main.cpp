#include "stdio.h"

#include <crtdbg.h>
#include <typeinfo>

inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

#include <locale.h>

#include "yon.h"
using namespace yon;

#pragma comment(lib, "Yon.lib")

int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

	SYonEngineParameters params;
	IYonEngine* pYE=CreateEngine(params);

	
	//setlocale(LC_ALL,"zh_CN.utf8");
	setlocale(LC_ALL,"chs");

	core::stringc str;
	printf("%d,%s\n",str.length(),str);

	str+="test";
	printf("%d,%s\n",str.length(),str);

	core::stringc str1=str+"test";
	printf("%d,%s\n",str1.length(),str1);

	core::stringw wstr;
	wprintf(L"%d,%s\n",wstr.length(),wstr);

	wstr+=L"中华人民共和国";
	wprintf(L"%d,%s\n",wstr.length(),wstr);

	core::stringw wstr1=wstr+L"test";
	wprintf(L"%d,%s\n",wstr1.length(),wstr1);

	core::stringc tstr("你好");
	printf("%s\n\n\n",tstr);

	getchar();
	return 0;
}