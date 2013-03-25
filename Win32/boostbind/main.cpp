#include <stdio.h>
#include <stdlib.h>

//refer to:http://greyforest.blog.sohu.com/152084205.html
//refer to:http://hi.baidu.com/hj11yc/item/107ee5350392f2392e0f818f
//refer to:http://xanpeng.github.com/coding/2012/07/26/boost-bind.html
//refer to:http://www.oschina.net/code/snippet_54334_10775


template <typename R, typename Arg> class invoker_base {
public:
	virtual R operator()(Arg arg)=0;
};


template <typename R, typename Arg> 
class function_ptr_invoker 
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

template <typename R, typename Arg> 
class function1 {
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

/*
template<typename R,>

class function<R()>:public function0<R>
{};*/



namespace  
{
	//通过在匿名名字空间中包含某种类型的变量来创建一个占位符
	class placeholder {};  
	placeholder _1;  
}  

template <typename R,typename T, typename Arg>  
class simple_bind_t1  
{  
public:  
	typedef R (T::*F)(Arg);
	simple_bind_t1(F f,const T& t):f_(f),t_(t)  
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
simple_bind_t1<R,T,Arg> mybind(R (T::*fn)(Arg),const T& t,const placeholder&)  
{
	return simple_bind_t1<R,T,Arg>(fn,t);
}

template <typename R, typename T, typename Arg>
class simple_bind_tp1
{
private:
	typedef R (T::*F)(Arg);
	F f_;
	T* t_;
	Arg& a_;

public:
	simple_bind_tp1(F f, T* t, Arg &a)
		: f_(f), t_(t), a_(a)
	{}

	R operator()()
	{
		return (t_->*f_)(a_);
	}
};


template <typename R, typename T, typename Arg>
simple_bind_tp1<R, T, Arg> mybind(R (T::*f)(Arg), T* t, Arg& a)
{
	return simple_bind_tp1<R, T, Arg>(f, t, a);
}

template <typename R, typename Arg>
simple_bind_tp1<R, Arg> mybind(R (*f)(Arg), Arg& a)
{
	return simple_bind_tp1<R, T, Arg>(f,a);
}

/*
template <typename R, typename Arg, typename Argo> class invoker_base2 {
public:
	virtual R operator()(Arg arg, Argo argo)=0;
};


template <typename R, typename Arg, typename Argo> class function_ptr_invoker2 
: public invoker_base2<R,Arg,Argo> {
	R (*func_)(Arg,Argo);
public:
	function_ptr_invoker2(R (*func)(Arg,Argo)):func_(func) {}

	R operator()(Arg arg, Argo argo) {
		return (func_)(arg,argo);
	}
};


template <typename R, typename Arg, typename Argo, typename T> 
class member_ptr_invoker2 : 
	public invoker_base2<R,Arg,Argo> {
		R (T::*func_)(Arg,Argo);
		T* t_;
public:
	member_ptr_invoker2(R (T::*func)(Arg,Argo),T* t)
		:func_(func),t_(t) {}

	R operator()(Arg arg, Argo argo) {
		return (t_->*func_)(arg,argo);
	}
};


template <typename R, typename Arg, typename Argo, typename T> 
class function_object_invoker2 : 
	public invoker_base2<R,Arg,Argo> {
		T t_;
public:
	function_object_invoker2(T t):t_(t) {}

	R operator()(Arg arg, Argo argo) {
		return t_(arg,argo);
	}
};

template <typename R, typename Arg, typename Argo> class function2 {
	invoker_base2<R,Arg,Argo>* invoker_;
public:
	function2(R (*func)(Arg,Argo)) : 
	  invoker_(new function_ptr_invoker2<R,Arg,Argo>(func)) {}

	  template <typename T> function2(R (T::*func)(Arg,Argo),T* p) : 
	  invoker_(new member_ptr_invoker2<R,Arg,Argo,T>(func,p)) {}

	  template <typename T> function2(T t) : 
	  invoker_(new function_object_invoker2<R,Arg,Argo,T>(t)) {}

	  R operator()(Arg arg,Argo argo) {
		  return (*invoker_)(arg,argo);
	  }

	  ~function2() {
		  delete invoker_;
	  }
};

template <typename R, typename Arg, typename Argo> class invoker_base2 {
public:
	virtual R operator()(Arg arg, Argo argo)=0;
};


template <typename R, typename Arg, typename Argo> class function_ptr_invoker2 
: public invoker_base2<R,Arg,Argo> {
	R (*func_)(Arg,Argo);
public:
	function_ptr_invoker2(R (*func)(Arg,Argo)):func_(func) {}

	R operator()(Arg arg, Argo argo) {
		return (func_)(arg,argo);
	}
};


template <typename R, typename Arg, typename Argo, typename T> 
class member_ptr_invoker2 : 
	public invoker_base2<R,Arg,Argo> {
		R (T::*func_)(Arg,Argo);
		T* t_;
public:
	member_ptr_invoker2(R (T::*func)(Arg,Argo),T* t)
		:func_(func),t_(t) {}

	R operator()(Arg arg, Argo argo) {
		return (t_->*func_)(arg,argo);
	}
};


template <typename R, typename Arg, typename Argo, typename T> 
class function_object_invoker2 : 
	public invoker_base2<R,Arg,Argo> {
		T t_;
public:
	function_object_invoker2(T t):t_(t) {}

	R operator()(Arg arg, Argo argo) {
		return t_(arg,argo);
	}
};

template <typename R, typename Arg, typename Argo> class function2 {
	invoker_base2<R,Arg,Argo>* invoker_;
public:
	function2(R (*func)(Arg,Argo)) : 
	  invoker_(new function_ptr_invoker2<R,Arg,Argo>(func)) {}

	  template <typename T> function2(R (T::*func)(Arg,Argo),T* p) : 
	  invoker_(new member_ptr_invoker2<R,Arg,Argo,T>(func,p)) {}

	  template <typename T> function2(T t) : 
	  invoker_(new function_object_invoker2<R,Arg,Argo,T>(t)) {}

	  R operator()(Arg arg,Argo argo) {
		  return (*invoker_)(arg,argo);
	  }

	  ~function2() {
		  delete invoker_;
	  }
};

template <typename R,typename T, typename Arg, typename Argo>  
class simple_bind_t2  
{  
public:  
	typedef R (T::*F)(Arg,Argo);
	simple_bind_t2(F f,const T& t):f_(f),t_(t)  
	{}  
	R operator()(Arg a,Argo o)  
	{  
		return (t_.*f_)(a,o);  
	}  
private:  
	F f_;  
	T t_;  
};

template <typename R, typename T, typename Arg, typename Argo>  
simple_bind_t2<R,T,Arg,Argo> mybind(R (T::*fn)(Arg,Argo),const T& t,const placeholder&)  
{
	return simple_bind_t2<R,T,Arg,Argo>(fn,t);
}

template <typename R, typename T, typename Arg, typename Argo>
class simple_bind_tp2
{
private:
	typedef R (T::*F)(Arg,Argo);
	F f_;
	T* t_;
	Arg& a_;
	Argo& b_;

public:
	simple_bind_tp2(F f, T* t, Arg &a, Argo& b)
		: f_(f), t_(t), a_(a), b_(b)
	{}

	R operator()()
	{
		return (t_->*f_)(a_,b_);
	}
};


template <typename R, typename T, typename Arg, typename Argo>
simple_bind_tp2<R, T, Arg,Argo> mybind(R (T::*f)(Arg,Argo), T* t, Arg& a, Argo& b)
{
	return simple_bind_tp2<R, T, Arg>(f, t, a, b);
}

//template<typename Signature> 
//class func;

//template<typename R,typename T0>
//class func:public function1<R,T0>
//{};

//template<typename R,typename T0,typename T1>
//class func:public function2<R,T0,T1>
//{};
*/
class Test{
public:
	Test(){
		printf("construct:%08X\n",this);
	}
	Test(const Test& t){
		printf("copy construct:%08X\n",this);
	}

	void test1(int a){
		printf("Test::test1:%08X,%d\n",this,a);
	}

	void test2(int a,float b){
		printf("Test::test2:%08X,%d,%.2f\n",this,a,b);
	}
};

void test3(int a){
	printf("test3:%d\n",a);
}
void test2(int a,float b){
	printf("test2:%d,%.2f\n",a,b);
}

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

int main(int argc,char ** argv)
{
	EnableMemLeakCheck();

	Test t;
	mybind(&Test::test1,t,_1)(10);

	//mybind(&Test::test2,t,_1)(11,12);


	function1<void,int> f1(mybind(&Test::test1, t, _1));
	f1(2);

	function1<void,int> f3(test3);
	f3(4);


	//function2<void,int,float> f2(mybind(&Test::test2, t, _1));
	//f2(10,20);

	
	system("pause");
	return 0;
}