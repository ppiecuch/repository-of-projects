
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
#include <stdio.h>
#include "yonAllocator.h"

typedef int s32;

//0 1 2 3 4
template<class T, typename TAlloc = yonAllocator<T> >
class loop{
private:
	TAlloc allocator;

	s32 front;
	s32 rear;
	s32 current;

	T* elements;
	s32 diff;
	s32 total;

	inline void lock(){}
	inline void unlock(){}
	inline void update(){
		diff=rear-front;
		if(diff<0)
			diff+=total;
	}
public:
	loop(s32 capacity)
		:total(0),elements(NULL),front(0),rear(0),current(0),diff(0)
	{
		elements=allocator.allocate(capacity);
		total=capacity;
	}

	~loop(){
		allocator.deallocate(elements);
		elements=total=front=rear=diff=current=0;
	}

	inline s32 capacity() const {return total;}

	inline s32 size() const{return diff;}

	inline s32 available() const{return total-diff-1;}

	// Return the max num for using pointer incrementment
	inline s32 nextable() const{
		lock();
		if(rear>=front)
		{
			if(front>0)
				return total-rear;
			else
				return total-rear-1;
		}
		s32 result=front-rear-1;
		unlock();
		return result;
	}

	T* pointer(){return elements;}

	const T* const_pointer() const{return elements;}

	void consume(s32 len){
		lock();
		YON_DEBUG_BREAK_IF(len<0||len>size())
		front+=len;
		if(front>=total)
			front-=total;
		update();
		unlock();
	}

	void supply(s32 len){
		lock();
		YON_DEBUG_BREAK_IF(len<0||len>available())
		rear+=len;
		if(rear>=total)
			rear-=total;
		update();
		unlock();
	}

	void clear(){
		memset(elements,0x0,capacity);
		front=rear=diff=current=0;
	}
};

int main(int argc,char ** argv)
{

	system("pause");
	return 0;
}

#endif