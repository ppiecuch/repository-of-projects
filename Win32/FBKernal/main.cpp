#include <stdio.h>
#include <stdlib.h>

#include <sys/timeb.h>
#include <time.h>

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

#include "FBKernal_Pool.h"

int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

#define COUNT 10000000
	struct _timeb start;
	struct _timeb end;
	long long s;
	int ms;


	CFBKernal_Pool int4(32);
	_ftime64_s( &start ); 
	for(int i=0;i<COUNT;++i)
	{
		//0.687s
		void* p=int4.Malloc(4,1,false);
		int4.Free(p);
		//11s
		//char* p=new char[4];
		//delete[] p;
	}
	_ftime64_s( &end );

	s=end.time-start.time;
	ms=end.millitm-start.millitm;
	if(ms<0){
		ms+=1000;
		--s;
	}
	printf("%d.",s);
	printf("%03d\n",ms);

	system("pause");
	return 0;
}
