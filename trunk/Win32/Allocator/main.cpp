#include <stdio.h>
#include <stdlib.h>

enum {_ALIGN = 8};//С��������Ͻ�


//��Byte����8�ı���
static size_t
_S_round_up(size_t __bytes) 
{ return (((__bytes) + (size_t) _ALIGN-1) & ~((size_t) _ALIGN - 1)); }
int main(){
	

	for(size_t i=0;i<8;++i)
		printf("%d\n",_S_round_up(i));
	system("pause");
	return 0;

}