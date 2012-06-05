#include <stdio.h>

int a=1;

int main(int argc, char* argv[])
{
	int b=*(&a);
	b=2;
	printf("%d\n",a);//1
	*(&a)=2;
	printf("%d\n",a);//2
	getchar();
}