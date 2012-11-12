#include <stdio.h>
#include <stdlib.h>

//调用操作符
class CallOperator{
public:
	void operator()(void){
		printf("operator()()\n");
	}
	void operator()(int a,int b){
		printf("operator()(%d,%d)\n",a,b);
	}
};

//递增递减操作符
class IncDecOperator{
public:
	IncDecOperator& operator++(){
		printf("++operator()\n");
		return *this;
	}

	IncDecOperator operator++(int){
		IncDecOperator tmp(*this);
		printf("operator++()\n");
		return tmp;
	}
};

class SubOperator{
public:
	SubOperator operator-(const SubOperator& s) const{
		printf("this-s\n");
		return *this;
	}
};

int main(int argc,char ** argv)
{
#if 0
	CallOperator a;
	a.operator()();
	a();
	a.operator()(1,2);
	a(1,2);
#elif 0
	IncDecOperator a;
	a++;
	++a;
#elif 1
	SubOperator a;
	SubOperator b;
	a-b;
#endif;
	system("pause");
	return 0;
}