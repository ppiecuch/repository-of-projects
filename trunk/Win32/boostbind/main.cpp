#include <stdio.h>
#include <stdlib.h>

//refer to:http://hi.baidu.com/hj11yc/item/107ee5350392f2392e0f818f
//refer to:http://xanpeng.github.com/coding/2012/07/26/boost-bind.html
//refer to:http://www.oschina.net/code/snippet_54334_10775

template <typename R, typename Arg> class invoker_base {
public:
	virtual R operator()(Arg arg)=0;
};


template <typename R, typename Arg> class function_ptr_invoker 
: public invoker_base<R,Arg> {
	R (*func_)(Arg);
public:
	function_ptr_invoker(R (*func)(Arg)):func_(func) {}

	R operator()(Arg arg) {
		return (func_)(arg);
	}
};


template <typename R, typename Arg, typename T> 
class member_ptr_invoker : 
	public invoker_base<R,Arg> {
		R (T::*func_)(Arg);
		T* t_;
public:
	member_ptr_invoker(R (T::*func)(Arg),T* t)
		:func_(func),t_(t) {}

	R operator()(Arg arg) {
		return (t_->*func_)(arg);
	}
};


template <typename R, typename Arg, typename T> 
class function_object_invoker : 
	public invoker_base<R,Arg> {
		T t_;
public:
	function_object_invoker(T t):t_(t) {}

	R operator()(Arg arg) {
		return t_(arg);
	}
};

template <typename R, typename Arg> class function1 {
	invoker_base<R,Arg>* invoker_;
public:
	function1(R (*func)(Arg)) : 
	  invoker_(new function_ptr_invoker<R,Arg>(func)) {}

	  template <typename T> function1(R (T::*func)(Arg),T* p) : 
	  invoker_(new member_ptr_invoker<R,Arg,T>(func,p)) {}

	  template <typename T> function1(T t) : 
	  invoker_(new function_object_invoker<R,Arg,T>(t)) {}

	  R operator()(Arg arg) {
		  return (*invoker_)(arg);
	  }

	  ~function1() {
		  delete invoker_;
	  }
};




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
	typedef R (T::*F)(Arg);
	simple_bind_t(F f,const T& t):f_(f),t_(t)  
	{}  
	R operator()(Arg a)  
	{  
		return (t_.*f_)(a);  
	}  
private:  
	F f_;  
	T t_;  
};

template <typename R, typename T, typename Arg>  
simple_bind_t<R,T,Arg> mybind(R (T::*fn)(Arg),const T& t,const placeholder&)  
{
	return simple_bind_t<R,T,Arg>(fn,t);
}  

template <typename R, typename T, typename Arg>
class simple_bind_t2
{
private:
	typedef R (T::*F)(Arg);
	F f_;
	T* t_;
	Arg& a_;

public:
	simple_bind_t2(F f, T* t, Arg &a)
		: f_(f), t_(t), a_(a)
	{}

	R operator()()
	{
		return (t_->*f_)(a_);
	}
};


template <typename R, typename T, typename Arg>
simple_bind_t2<R, T, Arg> mybind(R (T::*f)(Arg), T* t, Arg& a)
{
	return simple_bind_t2<R, T, Arg>(f, t, a);
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

	function1<void,int> f(mybind(&Test::test, t, _1));
	f(2);

	system("pause");
	return 0;
}