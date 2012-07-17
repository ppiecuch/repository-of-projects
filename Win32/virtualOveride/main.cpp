#include <stdio.h>

class A{
public:
	virtual void test(){
		printf("A:\n");
	}
	virtual void test(int a){
		printf("A:%d\n",a);
	}
};

class B : public A{
public:
	virtual void test(int a){
		printf("B:%d\n",a);
	}
};

int main(int argc, char* argv[]){
	B b;
	b.test();

	getchar();
}