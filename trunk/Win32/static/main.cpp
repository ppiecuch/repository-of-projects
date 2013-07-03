#include <stdio.h>
#include <stdlib.h>

struct base{
	static int s;
};
int base::s=0;
struct derA : public base{
};
struct derB : public base{
};
int main(int argc, char* argv[])
{

	derA a;
	derB b;

	a.s+=10;
	printf("%d\r\n",b.s);

	system("pause");
	return 0;
}