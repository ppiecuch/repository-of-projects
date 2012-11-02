#include<stdlib.h>
#include <stdio.h>
#include <sys/timeb.h>
#include <time.h>

//纳秒
inline unsigned long long GetNTime() 
{ 
	__asm _emit 0x0F
	__asm _emit 0x31
}

struct Inner{
	int a;
	int b;
	short c;
	char d[5];
};

struct A{
	Inner* tmp;
public:
	A():tmp(NULL){
		tmp=new Inner;
	}
	~A(){
		delete tmp;
	}
};

struct B{
	Inner tmp;
};

int main(int argc,char ** argv)
{

	long long start = GetNTime();
	

	//使用B比使用A快1000倍
	for(long i=0;i<900000000;++i){
		B a;
	}

	long long end = GetNTime();

	long long diff=end-start;
	long long s=diff/1000000000;
	long long ms=diff/1000000-s*1000;
	printf("%ld",s);
	printf(".%03ld\n",ms);

	system("pause");
	return 0;
}