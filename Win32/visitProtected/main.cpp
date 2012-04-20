#include <stdio.h>

class A{
protected:
	int a;
public:
	A(int v):a(v){}
	virtual void test(A* a){
		printf("A\n");
	}
};

class B : public A{
public:
	virtual void test(A* a){
		printf("B:%d\n",a->a);
	}
};

int main(int argc, char* argv[]){
	B b(0);
	b.test(&b);

	getchar();
}