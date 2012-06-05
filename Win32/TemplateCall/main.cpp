#include <stdio.h>

template<typename T>
void test(T a,float b){

}

int main(int argc, char* argv[])
{
	test(1.0f,1);
	test(1,10);
	getchar();
	return 0;
}