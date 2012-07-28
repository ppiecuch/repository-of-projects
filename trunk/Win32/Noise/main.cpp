#include "noise.h"
#include <time.h>
#include <stdio.h>

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

	InitNoise();
	for(int i=0;i<10;++i)
		printf("%.2f\r\n",Noise(0,0,0,i));

	getchar();
	return 0;
}