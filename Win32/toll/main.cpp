#include<stdlib.h>
#include <stdio.h>


inline unsigned __int64 GetCycleCount()   
{
	__asm _emit 0x0F   
	__asm _emit 0x31   
}


int main(int argc,char ** argv)
{
	int a,b=0;
	unsigned long long start;
	unsigned long long end;
	start=GetCycleCount();
	a=b;
	end=GetCycleCount(); 
	long long diff=end-start;
	long long s=diff/1000000000;
	long long ms=diff/1000000-s*1000;
	long long us=diff/1000-s*1000000-ms*1000;
	long long ns=diff-s*1000000000-ms*1000000-us*1000;
	printf("%ld",s);
	printf(".%03ld",ms);
	printf(".%03ld",us);
	printf(".%03ld\n",ns);
	system("pause");
	return 0;
}