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

	Allocator<PRIMITIVE> allocator;
	allocator.setOutOfMemHandler(&outOfMemory);
	//allocator.setMaxSize(1);
	
	lcTry{
		s16* p1=(s16*)LC_ALLOCATE(allocator,sizeof(s16));
		printf("%d\r\n",*p1);
		allocator.construct<s16>(p1,10);
		printf("%d\r\n",*p1);
		allocator.destruct<s16>(p1);
		printf("%d\r\n",*p1);
		LC_DEALLOCATE(allocator,p1);
		s32* p2=(s32*)LC_ALLOCATE(allocator,sizeof(s32));
	}lcCatch(...){
		printf("Nothing!\r\n");
	}

	allocator.destroy();
	

	system("pause");
	return 0;
}