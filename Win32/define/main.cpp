#include<stdlib.h>
#include <stdio.h>


inline unsigned __int64 GetCycleCount()   
{
	__asm _emit 0x0F   
	__asm _emit 0x31   
}

#define _EXECUTE_AS_NEEDED_(func,cah,req) \
	if(cah!=req) \
	{ \
		func(req); \
		cah=req; \
	}

void test(int a)
{
	printf("test %d\n",a);
}

#define _TEST(a) printf("%d\n",a)
#define _TEST(a,b) printf("%d,%d\n",a,b)

int a;
int b;

int main(int argc,char ** argv)
{

	_TEST(1);
	_EXECUTE_AS_NEEDED_(test,a,1);
	_EXECUTE_AS_NEEDED_(test,a,1);
	printf("-------------\r\n");
	_EXECUTE_AS_NEEDED_(test,b,1);
	_EXECUTE_AS_NEEDED_(test,b,1);
	_EXECUTE_AS_NEEDED_(test,b,2);
	system("pause");
	return 0;
}