#include <stdio.h>
#include <stdlib.h>

//Boost::Utility�� ֮enable_if
//refer to:http://hi.baidu.com/hins_pan/item/b156890ba81b3ad873e6762d

//enable_if  
//refer to:http://blog.163.com/hao_dsliu/blog/static/1315789082011427103659365/

// ���ã��Ƿ񽫺����߳����غ�ѡ�б�
//�����һ��������true,����Ƕ�ͱ�type���ǵڶ�������,����,û��type�Ķ���.
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

	invoker_type invoker; // ��Ҫ��Ա��������ú�����

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
	//TODO ����֮
	bool m_bEmpty;
public:
	function():m_bEmpty(true){
		printf("function:default,empty:%d\r\n",m_bEmpty);
	}

	//���û����죺  function<int(int)> f(0);  ��ʱ�򣬽��ã�����enable_if�ģ����캯�������ؾ���ĺ�ѡ�����ߵ���
	template<typename Functor>
	function(Functor f,typename enable_if_c< ice_not<is_integral<Functor>::value>::value, int>::type)
		: m_bEmpty(false), base_type(static_cast<const base_type&>(f)){
			//TODOΪ�β������õ�
			printf("function:%08X,empty:%d\r\n",f,m_bEmpty);
	}

	function(clear_type*) : base_type(),m_bEmpty(true) {
		printf("function:clear_type,empty:%d\r\n",m_bEmpty);
	}

	function(const self_type& f) : base_type(static_cast<const base_type&>(f)){}

	function(const base_type& f) : base_type(static_cast<const base_type&>(f)){}

	//��˶��壬���Ա�תΪint���ⲻ��������Ҫ��
	//operator bool(){
	//	return true;
	//}

	bool empty() const{
		return m_bEmpty;
	}

	//����������дif(f)��ʱ�򣬱��������ҵ�operator safe_bool()���Ӷ���fת��Ϊsafe_bool�����Ǹ�ָ�����ͣ�if������ȷ�ж����Ƿ�Ϊ��ָ�롣
	//��������ͼ��f�����������ͱ�����ʱ������⵽�����ڵľܾ�������Ϊsafe_bool��һ����Աָ�����ͣ��޷�����������ת����
	struct dummy { void nonnull(){}}; 
	typedef void (dummy::*safe_bool)(); // ȷ��safebool����ת��Ϊ�κ��������ͣ� 
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