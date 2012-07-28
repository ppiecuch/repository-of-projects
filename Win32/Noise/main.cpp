#include <stdio.h>
#include <windows.h>
#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}
#include <sys/timeb.h>
#include <time.h>

#include "noise.h"
#include "PerlinNoise.h"

int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

	/*InitNoise();

	struct _timeb start,end;
	_ftime64_s( &start ); 

	for(int i=0;i<100;++i)
	{
		_ftime64_s( &end ); 
		float t=(end.millitm-start.millitm)/1000.f;
		printf("%.2f->%.2f\r\n",t,Noise(0,0,0,t));

		Sleep(10);
	}*/

	//InitNoise();
	for(int i=0;i<2000;++i)
	{
		double n=noise(0, i/20.f, 0);
		//double n=Noise(0,0,0,i/20.f);
		printf("%.2f,",n);
		if(i%10==9)
			printf("\r\n");
	}

	getchar();
	return 0;
}