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
#include "FBKernal_Memory.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned __int64 u64;

struct SS{
	u64 a;
	u16 b;
	u32 c;
	u8 d;
};

int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

#define COUNT 100000000
	struct _timeb start;
	struct _timeb end;
	long long s;
	int ms;


	//CFBKernal_Pool int4(32);
#if 1
	SS* p1=FBKernal_Memory_New<SS>(1,0,__FILE__,__LINE__);
	printf("%08X,%08X,%08X,%08X\n",&p1->a,&p1->b,&p1->c,&p1->d);
	//printf("%08X\n",&p1->a);
	SS* p2=FBKernal_Memory_New<SS>(1,0,__FILE__,__LINE__);
	printf("%08X,%08X,%08X,%08X\n",&p2->a,&p2->b,&p2->c,&p2->d);
	//printf("%08X\n",&p2->a);
	//FBKernal_Memory_Delete(p1);
	//FBKernal_Memory_Delete(p2);
#else
	SS* p1=(SS*)malloc(sizeof(SS));
	printf("%08X,%08X,%08X,%08X\n",&p1->a,&p1->b,&p1->c,&p1->d);
	//printf("%08X\n",&p1->a);
	SS* p2=(SS*)malloc(sizeof(SS));
	printf("%08X,%08X,%08X,%08X\n",&p2->a,&p2->b,&p2->c,&p2->d);
	//printf("%08X\n",&p2->a);
	//free(p1);
	//free(p2);
#endif

	_ftime64_s( &start ); 
	for(int i=0;i<COUNT;++i)
	{
		//0.687s
		//void* p=int4.Malloc(4,1,false);
		//int4.Free(p);

		p2->a=1;

		//11s
		//char* p=new char[4];
		//delete[] p;
	}
	_ftime64_s( &end );

	FBKernal_Memory_Delete(p1);
	FBKernal_Memory_Delete(p2);
	//free(p1);
	//free(p2);

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
