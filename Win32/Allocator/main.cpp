#include <stdio.h>
#include <stdlib.h>

enum {_ALIGN = 8};//小块区域的上界


//将Byte调到8的倍数
static size_t
_S_round_up(size_t __bytes) 
{ return (((__bytes) + (size_t) _ALIGN-1) & ~((size_t) _ALIGN - 1)); }
int main(){
	

	for(size_t i=0;i<8;++i)
		printf("%d\n",_S_round_up(i));
	system("pause");
	return 0;

}