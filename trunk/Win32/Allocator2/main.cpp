#include <stdio.h>
#include <stdlib.h>

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

#include "lcAllocator.h"
using namespace lc;

void outOfMemory(){
	printf("outOfMemory\r\n");
}


int main()
{
	EnableMemLeakCheck();

#if 1
	MemoryTracer::create();
	int* p=LC_NEW int;
	delete p;
	double* d=LC_NEW double;
	float* f=LC_NEW float;
	short* s=LC_NEW short;
	delete f;
	long* l= new  long;
	char* c=new char;
	delete c;
	MemoryTracer::getInstance().testDeprecated1();
	MemoryTracer::testDeprecated2(NULL);
	MemoryTracer::getInstance().destroy();
#else
	Allocator<PRIMITIVE> allocator;
	allocator.setOutOfMemHandler(&outOfMemory);
	//allocator.setMaxSize(1);
	
	lcTry{
		s16* p1=LC_ALLOCATE(allocator,s16,sizeof(s16));
		printf("%d\r\n",*p1);
		allocator.construct<s16>(p1,10);
		printf("%d\r\n",*p1);
		allocator.destruct<s16>(p1);
		printf("%d\r\n",*p1);
		LC_DEALLOCATE(allocator,p1);
		s32* p2=LC_ALLOCATE(allocator,s32,sizeof(s32));
		//s32* p3=LUCID_NEW s32();
	}lcCatch(...){
		printf("Nothing!\r\n");
	}

	allocator.destroy();
#endif

	//int* p=new int;


	system("pause");
	return 0;
}