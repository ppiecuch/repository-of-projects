#include <stdio.h>

class A{
protected:
	virtual void g(){
		printf("A.g\n");
	}
public:
	virtual void test(){
		g();
		printf("A\n");
	}
};

class B : public A{
protected:
	virtual void g(){
		printf("B.g\n");
	}
public:
	virtual void test(){
		A::test();
		printf("B\n");
	}
};

int main(int argc, char* argv[]){
	B b;
	b.test();

	getchar();
}