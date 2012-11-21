#include <stdio.h>
#include <stdlib.h>
#include <list>
using namespace std;

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

#if 0
	float a=1.01f;
	//int a=58880;
	unsigned char c[4];
	memcpy(c,&a,4);
	printf("%02X,%02X,%02X,%02X\n",c[0],c[1],c[2],c[3]);
	//int b;
	float b;
	memcpy(&b,c,4);
	//printf("%d\n",b);
	printf("%.4f\n",b);
#else
	int* p=new int[2];
	printf("int:%08X,%08X\n",p,p+1);//+1跳过4个单位
	char* c=(char*)p;
	printf("int:%08X,%08X\n",c,c+1);//+1跳过1个单位
	delete[] p;
#endif
	system("pause");
	return 0;
}