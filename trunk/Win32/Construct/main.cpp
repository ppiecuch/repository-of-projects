#include <stdio.h>
#include <stdlib.h>

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

struct Test{
	//Test():p(NULL){}
	int* p;
};
int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

	Test tmp;

	Test* test=new Test();
	printf("%08x,%08x\r\n",test->p,tmp.p);
	delete test;

	system("pause");
	return 0;
}