#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

long GetCPUFreq()//获取CPU频率,单位: MHZ
{
	unsigned int start1,start2;
	_asm rdtsc
	_asm mov start1,eax
	Sleep(1000);
	_asm rdtsc
	_asm mov start2,eax
	return (start2-start1)/1000000;
}


int main(int argc, char* argv[])
{
	long freq=GetCPUFreq();
	printf("%ld\n",freq);
	system("pause");
	return 0;
}