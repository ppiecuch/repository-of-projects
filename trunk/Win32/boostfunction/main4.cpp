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

/**
* vtable for a specific boost::function instance. This
* structure must be an aggregate so that we can use static
* initialization in boost::function's assign_to and assign_to_a
* members. It therefore cannot have any constructors,
* destructors, base classes, etc.
*/
template<typename R>
struct basic_vtable0
{
	typedef R result_type;
	typedef result_type (*invoker_type)(function_buffer&);
}

/**
* The function_base class contains the basic elements needed for the
* function1, function2, function3, etc. classes. It is common to all
* functions (and as such can be used to tell if we have one of the
* functionN objects).
*/
class function_base
{
public:
	any_pointer functor;

	function_base()
	{
		//functor.obj_ptr = 0;
		functor.func_ptr=NULL;
	}

	// Is this function empty?
	bool empty() const{return functor.func_ptr==NULL;}
};

template<typename R>
class function0 : public function_base
{
public:
	typedef R return_type;
	//typedef function0 self_type;

	typedef R(*invoker_type)(any_pointer);

	invoker_type invoker; // ��Ҫ��Ա��������ú�����

	function0(){}

	template<typename Functor>
	function0(Functor const & f)
	{
		functor.func_ptr=reinterpret_cast<void(*)()>(f);
		printf("function0:%08X\r\n",f);
	}

	//result_type operator()() const
	//{
		//if (this->empty())
		//	boost::throw_exception(bad_function_call());

		//return get_vtable()->invoker
		//	(this->functor  BOOST_FUNCTION_ARGS);
	//}

	// Clear out a target, if there is one
	void clear()
	{
		//if (vtable) {
		//	if (!this->has_trivial_copy_and_destroy())
		//		get_vtable()->clear(this->functor);
		//	vtable = 0;
		//}
		functor.func_ptr=NULL;
	}

	// Assignment from another function0
	function0& operator=(const function0& f)
	{
		if (&f == this)
			return *this;

		this->clear();
		/*BOOST_TRY {
			this->assign_to_own(f);
		} BOOST_CATCH (...) {
			vtable = 0;
			BOOST_RETHROW;
		}
		BOOST_CATCH_END*/
		
		return *this;
	}

	void swap(function0& other)
	{
		if (&other == this)
			return;

		/*function0 tmp;
		tmp.move_assign(*this);
		this->move_assign(other);
		other.move_assign(tmp);*/
		any_pointer tmp;
		tmp=this->functor;
		this->functor=other.functor;
		other.functor=tmp;
	}

	template<typename R >
	inline void swap(function0<R>& f1,function0<R>& f2)
	{
		f1.swap(f2);
	}
private:
	void assign_to_own(const function0& f)
	{
		/*if (!f.empty()) {
			this->vtable = f.vtable;
			if (this->has_trivial_copy_and_destroy())
				this->functor = f.functor;
			else
				get_vtable()->base.manager(f.functor, this->functor,
				boost::detail::function::clone_functor_tag);
		}*/
		this->functor=f.functor;
	}
};

template<typename R>
class function<R(void)> : public function0<R>
{
	typedef function0<R> base_type;
	typedef function self_type;

	struct clear_type{};
public:
	function(){
		printf("function:default,empty:%d\r\n",empty());
	}

	//���û����죺  function<int(int)> f(0);  ��ʱ�򣬽��ã�����enable_if�ģ����캯�������ؾ���ĺ�ѡ�����ߵ���
	template<typename Functor>
	function(Functor f,typename enable_if_c< ice_not<is_integral<Functor>::value>::value, int>::type)
		: base_type(static_cast<const base_type&>(f)){
			//TODOΪ�β������õ�
			printf("function:%08X,empty:%d\r\n",f,empty());
	}

	function(clear_type*) : base_type() {
		printf("function:clear_type,empty:%d\r\n",empty());
	}

	function(const self_type& f) : base_type(static_cast<const base_type&>(f)){}

	function(const base_type& f) : base_type(static_cast<const base_type&>(f)){}


	self_type& operator=(const self_type& f)
	{
		self_type(f).swap(*this);
		return *this;
	}

	template<typename Functor>
	typename enable_if_c<(ice_not<(is_integral<Functor>::value)>::value),self_type&>::type
		operator=(Functor f)
	{
		self_type(f).swap(*this);
		return *this;
	}

	//��˶��壬���Ա�תΪint���ⲻ��������Ҫ��
	//operator bool(){
	//	return true;
	//}

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

	f0=test0;
	if(f0)
		printf("f0==true\r\n");

	//TODO
	//f2();
	
	system("pause");
	return 0;
}