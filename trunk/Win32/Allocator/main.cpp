#include <stdio.h>
#include <stdlib.h>

#include "alignof.h"
#include "yonAllocator.h"
#include "alignmentpool.h"
#include "yonString.h"

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


enum {_ALIGN = 8};//С��������Ͻ�

//��Byte����8�ı���
static size_t
_S_round_up(size_t __bytes) 
{ return (((__bytes) + (size_t) _ALIGN-1) & ~((size_t) _ALIGN - 1)); }

/*void* operator new( size_t nSize, char* pszFileName, int nLineNum )
{
	void *pResult;

	pResult = ::operator new( nSize );

	printf("fdafda\r\n");
	return pResult;
}

#define YON_DEBUG_NEW new( __FILE__, __LINE__)*/

int main(int argc, char* argv[]){

	EnableMemLeakCheck();
	
#define COUNT 10000000
	struct _timeb start;
	struct _timeb end;
	long long s;
	int ms;

	//core::stringc str("%d",1);
	core::stringc str1("");
	printf("%d\r\n",str1.length());
	core::stringc str2("		hello \n\t");
	printf("%d\r\n",str2[0]);
	printf("%d\r\n",str2.length());
	printf("%d\r\n",str2.trim().length());

	core::array<int> a;
	a.push_back(1);

	/*
	//alignmentpool
	core::alignmentpool<128,1,64> ali1;
	c8* p1=(c8*)ali1.allocate(129,1);
	c8* p2=(c8*)ali1.allocate(1,3);
	printf("%08X,%08X,%08X\n",p1,p1+1,p1+2);
	printf("%08X,%08X,%08X\n",p2,p2+1,p2+2);
	ali1.deallocate(p1);
	u64* p3=(u64*)ali1.allocate(8,16);
	printf("%08X,%08X\n",p3,p3+1);
	ali1.deallocate(p3);
	ali1.deallocate(p2);
	*/

	/*u32 align=core::alignOf<u64>();
	printf("align:%u\n",align);
	for(u32 addr=0;addr<15;++addr)
	{
		u32 offset=(addr+align-1)&~(align-1);
		printf("%u,%u\n",addr,offset);
	}*/

	/*
	#ifndef INTERNAL_SIZE_T  
	#define INTERNAL_SIZE_T size_t  
	#endif  
	#define SIZE_SZ                (sizeof(INTERNAL_SIZE_T))  
	#ifndef MALLOC_ALIGNMENT  
	#define MALLOC_ALIGNMENT       (2 * SIZE_SZ)  
	#endif  
	*/
	//refer to:http://www.cnblogs.com/Creator/archive/2012/04/05/2433386.html
	//���ݺ궨���������32λϵͳ��malloc��MINSIZEΪ16�ֽڣ���64λϵͳ��MINSIZEһ��Ϊ32�ֽڡ�
	/*void* p1=malloc(1);
	void* p2=malloc(8);
	printf("%08X,%08X\n",p1,p2);
	free(p1);
	free(p2);*/

#if 0
	//for(size_t i=0;i<8;++i)
	//	printf("%d\n",_S_round_up(i));
	_ftime64_s( &start ); 
	//�����������ں��뼶������һ����
	//core::array<s32> arr;
	//core::array<s32,yonAllocatorMalloc<s32> > arr;
	//core::array<s32,yonAllocatorMemoryPool<s32,100> > arr;
	core::array<s32,yonAllocatorAlign<s32> > arr;
	for(int i=0;i<COUNT;++i){
		arr.push_back(i);
	}

	/*
	//core::list<s32> l;
	core::list<s32,yonAllocatorMemoryPool<YonListNode<s32> > > l;
	for(int i=0;i<COUNT;++i){
		l.push_back(i);
		l.erase(l.begin());
	}
	*/
	_ftime64_s( &end );

	s=end.time-start.time;
	ms=end.millitm-start.millitm;
	if(ms<0){
		ms+=1000;
		--s;
	}
	printf("%d.",s);
	printf("%03d\n",ms);
#endif

	system("pause");
	return 0;

}