
#if 0
#include <iostream>
#include "SCArray.h"

using namespace std;
using namespace SCARRAY;


void main()
{
	SCArray buf(100);
	char* cbuf = buf.Buffer();
	char cc[20];
	int i;
	for( i=0;i<50;i++)
	{
		if(i%10==8)
			cbuf[i]='\r';
		else if(i%10==9)
			cbuf[i]='\n';
		else
			cbuf[i]='0'+i%10;
	}
	for( i=50;i<90;i++)
	{
		if(i%10==6)
			cbuf[i]='\r';
		else if(i%10==7)
			cbuf[i]='\n';
		else
			cbuf[i]='0'+i%10;
	}
	for( i=0;i<10;i++)
	{
		cbuf[90+i]='a'+i%26;
	}

		cout<<cbuf;

	cout<<"\n=============================================\n";
	buf.SetWritePosition(60);


	cout<<"\n=============================================\n";
	buf.SetReadPosition(50);
	for( i=0;i<20;i++)
	{
		if(buf.GetData(10,cc))
			cout<<cc<<"\n----"<<i<<"------\n";
		else
			cout<<cc<<"\n###-"<<i<<"------\n";
	}
	cout<<"\n=============================================\n";
	buf.SetReadPosition(6);
	for( i=0;i<20;i++)
	{
		if(buf.GetLine(cc,20,"567\r\n"))
			cout<<cc<<"\n----"<<i<<"------\n";
		else
			cout<<cc<<"\n###-"<<i<<"------\n";
	}
	cout<<"\n=============================================\n";
	buf.SetReadPosition(49);
	for( i=0;i<20;i++)
	{
		if(buf.GetLine(cc,20,"\r\n"))
			cout<<cc<<"\n----"<<i<<"------\n";
		else
			cout<<cc<<"\n###-"<<i<<"------\n";
	}

	system("pause");



}

#elif 1

#include<time.h>
#include<stdlib.h>

#include <stdio.h>
#include "yonAllocator.h"
#include "yonString.h"

#include <windows.h>

DWORD WINAPI consumeProc(LPVOID lpParameter);
DWORD WINAPI supplyProc(LPVOID lpParameter);

typedef int s32;

//0 1 2 3 4
template<class T, typename TAlloc = yon::core::yonAllocator<T> >
class circular{
private:
	TAlloc allocator;

	s32 front;
	s32 rear;

	T* elements;
	s32 total;

public:
	circular(s32 capacity=1024)
		:total(0),elements(NULL),front(0),rear(0)
	{
		elements=allocator.allocate(capacity);
		total=capacity;
	}

	~circular(){
		allocator.deallocate(elements);
	}

	inline s32 capacity() const {return total;}

	inline s32 size() const{
		s32 len=rear-front;
		if(len<0)
			len+=total;
		return len;
	}

	inline s32 available() const{
		s32 len=rear-front;
		if(len>=0)
			return total-1-len;
		else
			return -len-1;
	}

	// Return the max num for using pointer increment
	inline s32 seqailable() const{
		s32 tmpFront=front;
		if(rear<tmpFront)
			return tmpFront-rear-1;
		else if(tmpFront==0)
			return total-rear-1;
		else 
			return total-rear;
	}

	// Return the max num for using pointer decrement
	inline s32 sequpied() const{
		s32 tmpRear=rear;
		if(tmpRear>=front)
			return tmpRear-front;
		else
			return total-front;
	}

	T* supply_pointer(){return elements+rear;}

	T* consume_pointer(){return elements+front;}

	void consume(s32 len){
		YON_DEBUG_BREAK_IF(len<0||len>size())
		if(front+len>=total)
			front+=len-total;
		else
			front+=len;
	}

	void supply(s32 len){
		YON_DEBUG_BREAK_IF(len<0||len>available())
		if(rear+len>=total)
			rear+=len-total;
		else
			rear+=len;
	}

	void clear(){
		memset(elements,0x0,capacity);
		front=rear=0;
	}
};

boolean finishConsume=false;
boolean finishSupply=false;

circular<yon::s32> loop(10);

s32 seed;
int randIn10(){
	const s32 m = 2147483399;	// a non-Mersenne prime
	const s32 a = 40692;		// another spectral success story
	const s32 q = m/a;
	const s32 r = m%a;		// again less than q

	seed = a * (seed%q) - r* (seed/q);
	if (seed<0) seed += m;

	return seed%10+1;
}

int main(int argc,char ** argv)
{

	seed=(int)time(0);
	/*int i,j;
	
	for (i=0; i<10; i++) {
		j = 1 + (int) (10.0*rand() / (RAND_MAX + 1.0));
		printf("%d\n",j);
	}*/
	HANDLE consumeThread;
	HANDLE supplyThread;
	//创建线程

	consumeThread=CreateThread(NULL,0,consumeProc,NULL,0,NULL);
	supplyThread=CreateThread(NULL,0,supplyProc,NULL,0,NULL);

	while(!finishConsume||!finishSupply);

	CloseHandle(consumeThread);
	CloseHandle(supplyThread);

	system("pause");
	return 0;
}

DWORD WINAPI consumeProc(LPVOID lpParameter){
	for(int i=0;i<20;++i){
		int num=randIn10();
		if(loop.size()<num)
			printf("no ready for consume(%d):%d<%d!\n",num,loop.size(),num);
		else if(loop.sequpied()<num)
		{
			//printf("no seq fro consume(%d):%d<%d!\n",num,loop.sequpied(),num);
			yon::core::stringc str("consume(%d):",num);
			int seq=loop.sequpied();
			s32* p=loop.consume_pointer();
			for(int j=0;j<seq;++j){
				int tmp=*p++;
				str.append(yon::core::stringc("%d,",tmp));
			}
			loop.consume(seq);

			int remain=num-seq;
			p=loop.consume_pointer();
			for(int j=0;j<remain;++j){
				int tmp=*p++;
				str.append(yon::core::stringc("%d,",tmp));
			}
			loop.consume(remain);
			printf("%s\n",str.c_str());
		}
		else
		{
			yon::core::stringc str("consume(%d):",num);
			s32* p=loop.consume_pointer();
			for(int j=0;j<num;++j){
				int tmp=*p++;
				str.append(yon::core::stringc("%d,",tmp));
			}
			printf("%s\n",str.c_str());
			loop.consume(num);
		}
		Sleep(10);
	}
	finishConsume=true;
	return 0;
}
DWORD WINAPI supplyProc(LPVOID lpParameter){
	for(int i=0;i<20;++i){
		int num=randIn10();
		if(loop.available()<num)
			printf("no ready for supply(%d):%d<%d!\n",num,loop.available(),num);
		else if(loop.seqailable()<num)
		{
			//printf("no seq for supply(%d):%d<%d!\n",num,loop.seqailable(),num);
			yon::core::stringc str("supply(%d):",num);
			s32* p=loop.supply_pointer();
			int seq=loop.seqailable();
			for(int j=0;j<seq;++j){
				int tmp=randIn10();
				*p++=tmp;
				str.append(yon::core::stringc("%d,",tmp));
			}
			loop.supply(seq);

			int remain=num-seq;
			p=loop.supply_pointer();
			for(int j=0;j<remain;++j){
				int tmp=randIn10();
				*p++=tmp;
				str.append(yon::core::stringc("%d,",tmp));
			}
			loop.supply(remain);
			printf("%s\n",str.c_str());
		}
		else
		{
			yon::core::stringc str("supply(%d):",num);
			s32* p=loop.supply_pointer();
			for(int j=0;j<num;++j){
				int tmp=randIn10();
				*p++=tmp;
				str.append(yon::core::stringc("%d,",tmp));
			} 
			printf("%s\n",str.c_str());
			loop.supply(num);
		}
		Sleep(10);
	}
	finishSupply=true;
	return 0;
}

#endif