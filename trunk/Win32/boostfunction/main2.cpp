#include <stdio.h>
#include <stdlib.h>

//Boost::Utility库 之enable_if
//refer to:http://hi.baidu.com/hins_pan/item/b156890ba81b3ad873e6762d

//enable_if  
//refer to:http://blog.163.com/hao_dsliu/blog/static/1315789082011427103659365/

// 作用，是否将函数踢出重载候选列表。
//如果第一个参数是true,则内嵌型别type就是第二个参数,否则,没有type的定义.
template <bool B, class T = void>
struct enable_if_c {
	typedef T type;
};

template <class T>
struct enable_if_c<false, T> {};

template <class Cond, class T = void>
struct enable_if : public enable_if_c<Cond::value, T> {};

template <bool B, class T>
struct lazy_enable_if_c {
	typedef typename T::type type;
};

template <class T>
struct lazy_enable_if_c<false, T> {};

template <class Cond, class T>
struct lazy_enable_if : public lazy_enable_if_c<Cond::value, T> {};


template <bool B, class T = void>
struct disable_if_c {
	typedef T type;
};

template <class T>
struct disable_if_c<true, T> {};

template <class Cond, class T = void>
struct disable_if : public disable_if_c<Cond::value, T> {};

template <bool B, class T>
struct lazy_disable_if_c {
	typedef typename T::type type;
};

template <class T>
struct lazy_disable_if_c<true, T> {};

template <class Cond, class T>
struct lazy_disable_if : public lazy_disable_if_c<Cond::value, T> {};


template <bool b>
struct ice_not
{
	static const bool value = true;
};

template <>
struct ice_not<true>
{
	static const bool value = false;
};


template <class T, T val>
struct integral_constant
{
	typedef integral_constant<T, val>  type;
	typedef T                          value_type;
	static const T value = val;
};

typedef integral_constant<bool, true>  true_type;
typedef integral_constant<bool, false> false_type;

template <class T>
struct is_integral : public false_type {};

template <>
struct is_integral<bool> : public true_type {};
template <>
struct is_integral<char> : public true_type {};
template <>
struct is_integral<signed char> : public true_type {};
template <>
struct is_integral<unsigned char> : public true_type {};
template <>
struct is_integral<signed short> : public true_type {};
template <>
struct is_integral<unsigned short> : public true_type {};
template <>
struct is_integral<signed int> : public true_type {};
template <>
struct is_integral<unsigned int> : public true_type {};
template <>
struct is_integral<signed long> : public true_type {};
template <>
struct is_integral<unsigned long> : public true_type {};
template <>
struct is_integral<float> : public true_type {};
template <>
struct is_integral<double> : public true_type {};

template<typename T> struct is_pointer{enum{value=0};};
template<typename T> struct is_pointer<T*>{enum{value=1};};

template<typename Signature>
class function{};

union any_pointer
{
	void (*func_ptr)();
};

template<typename R>
class function0
{
public:
	typedef R return_type;
	//typedef function0 self_type;

	typedef R(*invoker_type)(any_pointer);

	invoker_type invoker; // 重要成员，负责调用函数！

	function0(){}

	template<typename Functor>
	function0(Functor const & f)
		: invoker(0)
	{
		printf("function0:%08X\r\n",f);
	}
};

template<typename R>
class function<R(void)> : public function0<R>
{
	typedef function0<R> base_type;
	typedef function self_type;

	struct clear_type{};
	//TODO 不用之
	bool m_bEmpty;
public:
	function():m_bEmpty(true){
		printf("function:default,empty:%d\r\n",m_bEmpty);
	}

	//当用户构造：  function<int(int)> f(0);  的时候，将该（带有enable_if的）构造函数从重载决议的候选集中踢掉。
	template<typename Functor>
	function(Functor f,typename enable_if_c< ice_not<is_integral<Functor>::value>::value, int>::type)
		: m_bEmpty(false), base_type(static_cast<const base_type&>(f)){
			//TODO为何不被调用到
			printf("function:%08X,empty:%d\r\n",f,m_bEmpty);
	}

	function(clear_type*) : base_type(),m_bEmpty(true) {
		printf("function:clear_type,empty:%d\r\n",m_bEmpty);
	}

	function(const self_type& f) : base_type(static_cast<const base_type&>(f)){}

	function(const base_type& f) : base_type(static_cast<const base_type&>(f)){}

	//如此定义，可以被转为int，这不是我们想要的
	//operator bool(){
	//	return true;
	//}

	bool empty() const{
		return m_bEmpty;
	}

	//这样，当你写if(f)的时候，编译器会找到operator safe_bool()，从而将f转型为safe_bool，这是个指针类型，if语句会正确判定它是否为空指针。
	//而当你试图把f赋给其它整型变量的时候则会遭到编译期的拒绝——因为safe_bool是一个成员指针类型，无法向其它整型转换。
	struct dummy { void nonnull(){}}; 
	typedef void (dummy::*safe_bool)(); // 确保safebool不能转型为任何其它类型！ 
	operator safe_bool () const  { return (this->empty())? 0 : &dummy::nonnull; }

};

void test0()
{
	printf("test0\r\n");
}

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

class obj{};

int main(int argc,char ** argv)
{
	EnableMemLeakCheck();

	printf("is_integral<char>:%d\r\n",is_integral<char>::value);
	printf("is_integral<unsigned char>:%d\r\n",is_integral<unsigned char>::value);
	printf("is_integral<short>:%d\r\n",is_integral<short>::value);
	printf("is_integral<unsigned short>:%d\r\n",is_integral<unsigned short>::value);
	printf("is_integral<int>:%d\r\n",is_integral<int>::value);
	printf("is_integral<unsigned int>:%d\r\n",is_integral<unsigned int>::value);
	printf("is_integral<float>:%d\r\n",is_integral<float>::value);
	printf("is_integral<double>:%d\r\n",is_integral<double>::value);
	printf("is_integral<long>:%d\r\n",is_integral<long>::value);
	printf("is_integral<unsigned long>:%d\r\n",is_integral<unsigned long>::value);

	printf("is_integral<void>:%d\r\n",is_integral<void>::value);
	printf("is_integral<void*>:%d\r\n",is_integral<void*>::value);
	printf("is_integral<obj>:%d\r\n",is_integral<obj>::value);

	printf("\r\n\r\n");

	function<void()> f0(0);

	if(f0)
		printf("f0==true\r\n");

	function<void()> f1;

	if(f1)
		printf("f1==true\r\n");

	function<void()> f2(test0);

	if(f2)
		printf("f2==true\r\n");

	//TODO
	//f2();
	
	system("pause");
	return 0;
}