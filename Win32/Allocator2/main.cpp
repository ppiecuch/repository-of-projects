#include <stdio.h>
#include <stdlib.h>

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

#include "ck_mem_mgr.h"

#include "LCAllocator.h"

int main()
{
	EnableMemLeakCheck();


	ckMemMgr::createFirst();

	int* p=(int*)ckMalloc(sizeof(int));
	ckFree(p);

	ckMemMgr::destroyLast();

	system("pause");
	return 0;
}