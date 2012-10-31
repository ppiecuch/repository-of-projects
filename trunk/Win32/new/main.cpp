#include<stdlib.h>
#include <stdio.h>
#include <sys/timeb.h>
#include <time.h>

//дицК
inline unsigned long long GetNTime() 
{ 
	__asm _emit 0x0F
	__asm _emit 0x31
}

struct A{
	int* tmp;
public:
	A():tmp(NULL){
		tmp=new int;
	}
	~A(){
		delete tmp;
	}
};

struct B{
	int tmp;
};

int main(int argc,char ** argv)
{

	long long start = GetNTime();
	

	for(long i=0;i<90000;++i){
		A a;
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