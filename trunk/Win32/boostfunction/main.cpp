#include <stdio.h>
#include <stdlib.h>

//refer to:http://wenku.baidu.com/view/99557c6127d3240c8447ef20.html

//function类只是个薄薄的外覆（wrapper），真正起作用的是其偏特化版本。
//function作为一个仿函数类，怎么没有重载operator()——这可是身为仿函数的标志啊！
//别急，function把这些烦人的任务都丢给了它的基类functionN
template<typename Signature>
class function;

union any_pointer;

template<typename FunctionPtr,typename R> // 注意这里的模板参数，后面会解释
struct function_invoker0
{
	static R invoke(any_pointer function_ptr)
	{
		FunctionPtr f = reinterpret_cast<FunctionPtr>(function_ptr.func_ptr);
		return f();
	}

	//此外，如果用户当初给出的是仿函数而不是函数指针，则有function_obj_invoker1与它对应，
	//后者也是一个类似的模板，它的invoke静态成员函数的形式也是：
	static R invoke(any_pointer function_obj_ptr)
	{
		FunctionObj* f = (FunctionObj*)(function_obj_ptr.obj_ptr);
		return (*f)(); // 调用用户当初给出的仿函数

	}
};


template<typename R>
class function0
{
protected:
	typedef R return_type;
	typedef function0 self_type;

	template<typename Functor>
	function0(Functor const & f)
		: invoker(0)
	{
		this->assign_to(f);   //这儿真正进行赋值，assign_to的代码在下面列出
	}

	template<typename Functor>
	void assign_to(Functor f) // 所有的构造函数都调用它！具有多个重载版本。
	{
		// 以一个get_function_tag萃取出Functor的类别(category)!
		typedef typename detail::function::get_function_tag<Functor>::type tag;

		// 根据不同类别的Functor采取不同的assign策略！
		this->assign_to(f, tag()); // 转到真正做事情的assign_to版本，见下文。
	}

	/*
	其中值得注意的是：
	get_function_tag<>能萃取出Functor的类别(category)，有下面几种类别

	struct function_ptr_tag {}; // 函数指针
	struct function_obj_tag {}; // 仿函数对象
	struct member_ptr_tag {}; // 成员函数
	struct function_obj_ref_tag {}; // 以ref(obj)加以封装的类别，具有引用语义
	struct stateless_function_obj_tag {}; // 无状态函数对象

	此外，满足以下所有条件：

	has_trivial_constructor
	has_trivial_copy
	has_trivial_destructor
	is_empty

	的仿函数对象称为stateless的。
	*/

	//对于不同的函数类别，assign_to有各个不同的重载版本
	template<typename FunctionPtr> // 如果是函数指针就调用这个版本
	void assign_to(FunctionPtr f, function_ptr_tag) // 这个版本针对函数指针
	{
		clear();

		if(f){
			typedef typename get_function_invoker0<FunctionPtr,R>::type invoker_type; // 萃取相应的invoker
			//::type的类型将会被推导为function_invoker0<R(*)()>

			invoker = &invoker_type::invoke; // invoke是一个static成员函数

			// 管理策略，一个函数指针
			function_base::manager = &functor_manager<FunctionPtr, Allocator>::manage;

			// 交给function的函数指针或仿函数对象指针最终在这儿保存
			function_base::functor = function_base::manager(make_any_pointer((void (*)())(f)),clone_functor_tag); // 拷贝一份
		}
	}


	result_type operator()() const // 身为仿函数的标志！
	{

		// 这里进行真正的函数调用，使用invoker，
		// invoker是functionN的成员变量，在下面的assign_to中被赋值。
		// functor为实际被调用的函数或仿函数，a0当然是其参数。
		internal_result_type result = invoker(function_base::functor); 

		// 将得到的结果cast至最终返回出去的类型
		return static_cast<result_type>(result);
	}

	// any_pointer在下文解释
	typedef internal_result_type(*invoker_type)(any_pointer);

	invoker_type invoker; // 重要成员，负责调用函数！


};

//需要说明的是，对于函数指针，function_base并不知道也不关心它要保存的函数指针是什么确切的类型，
//只要是函数指针就行，因为它总会把该函数指针f转型为“void (*)()”类型，然后保存在functor成员中，functor成员是一个union类型:
union any_pointer
{
	// 任意仿函数对象指针都可以用static_cast转型为void*型
	void* obj_ptr;

	// 为const仿函数准备的
	const void* const_obj_ptr;

	// 任意函数指针都可以用reinterpret_cast转型为void(*)()型
	void (*func_ptr)();

	char data[1];
};
//这个any_pointer可以通过安全转型保存所有形式的仿函数和函数指针，承载在底层保存数据的任务

template<typename R>
class function<R> : function0<R>
{
	typedef function0<R> base_type;
	typedef function self_type;

	//struct clear_type{};
public:
	template<typename Functor> 
	function(Functor f):base_type(f){}

};


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

void test1(int a){
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
	
	system("pause");
	return 0;
}