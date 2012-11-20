#include <stdio.h>
#include <stdlib.h>

#include <sys/timeb.h>
#include <time.h>

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

#include "yonArray.h"
#include "yonList.h"
#include "memorypool.h"
using namespace yon;
using namespace yon::core;

enum {_ALIGN = 8};//小块区域的上界

//将Byte调到8的倍数
static size_t
_S_round_up(size_t __bytes) 
{ return (((__bytes) + (size_t) _ALIGN-1) & ~((size_t) _ALIGN - 1)); }


int main(int argc, char* argv[]){

	EnableMemLeakCheck();
	
#define COUNT 10000000
	struct _timeb start;
	struct _timeb end;
	long long s;
	int ms;

	//for(size_t i=0;i<8;++i)
	//	printf("%d\n",_S_round_up(i));
	_ftime64_s( &start ); 
	//试验结果两者在毫秒级别上是一样的
	//core::array<s32> arr;
	//core::array<s32,yonAllocatorMalloc<s32> > arr;
	//core::array<s32,yonAllocatorMemoryPool<s32,100> > arr;
	//for(int i=0;i<COUNT;++i){
	//	arr.push_back(i);
	//}
	//core::list<s32> l;
	core::list<s32,yonAllocatorMemoryPool<YonListNode<s32> > > l;
	for(int i=0;i<COUNT;++i){
		l.push_back(i);
		l.erase(l.begin());
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