#include<stdlib.h>
#include <stdio.h>


inline unsigned __int64 GetCycleCount()   
{
	__asm _emit 0x0F   
	__asm _emit 0x31   
}

struct Outter{
	struct Inner{
		int a;
	};

	static Inner inner;
};
Outter::Inner inner;//无法解析的外部命令

int main(int argc,char ** argv)
{
	Outter a;
	Outter b;
	printf("%08X,%08X",&a.inner,&b.inner);
	system("pause");
	return 0;
}