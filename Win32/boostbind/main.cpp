#include <stdio.h>
#include <stdlib.h>

namespace  
{
	//通过在匿名名字空间中包含某种类型的变量来创建一个占位符
	class placeholder {};  
	placeholder _1;  
}  

template <typename R,typename T, typename Arg>  
class simple_bind_t  
{  
public:  
	typedef R (T::*fn)(Arg);
	simple_bind_t(fn f,const T& t):fn_(f),t_(t)  
	{}  
	R operator()(Arg a)  
	{  
		return (t_.*fn_)(a);  
	}  
private:  
	fn fn_;  
	T t_;  
};

template <typename R, typename T, typename Arg>  
simple_bind_t<R,T,Arg> mybind(R (T::*fn)(Arg),const T& t,const placeholder&)  
{
	return simple_bind_t<R,T,Arg>(fn,t);
}  

class Test{
public:
	Test(){
		printf("construct:%08X\n",this);
	}
	Test(const Test& t){
		printf("copy construct:%08X\n",this);
	}

	void test(int a){
		printf("Test::test:%08X,%d\n",this,a);
	}
};

void test(int a){
	printf("test:%d\n",a);
}

int main(int argc,char ** argv)
{
	Test t;
	mybind(&Test::test,t,_1)(10);
	system("pause");
	return 0;
}