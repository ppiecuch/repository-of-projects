#include <stdio.h>
#include <stdlib.h>

//refer to:http://wenku.baidu.com/view/99557c6127d3240c8447ef20.html

//function��ֻ�Ǹ��������⸲��wrapper�������������õ�����ƫ�ػ��汾��
//function��Ϊһ���º����࣬��ôû������operator()�����������Ϊ�º����ı�־����
//�𼱣�function����Щ���˵����񶼶��������Ļ���functionN
template<typename Signature>
class function;

union any_pointer;

template<typename FunctionPtr,typename R> // ע�������ģ���������������
struct function_invoker0
{
	static R invoke(any_pointer function_ptr)
	{
		FunctionPtr f = reinterpret_cast<FunctionPtr>(function_ptr.func_ptr);
		return f();
	}

	//���⣬����û������������Ƿº��������Ǻ���ָ�룬����function_obj_invoker1������Ӧ��
	//����Ҳ��һ�����Ƶ�ģ�壬����invoke��̬��Ա��������ʽҲ�ǣ�
	static R invoke(any_pointer function_obj_ptr)
	{
		FunctionObj* f = (FunctionObj*)(function_obj_ptr.obj_ptr);
		return (*f)(); // �����û����������ķº���

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
		this->assign_to(f);   //����������и�ֵ��assign_to�Ĵ����������г�
	}

	template<typename Functor>
	void assign_to(Functor f) // ���еĹ��캯���������������ж�����ذ汾��
	{
		// ��һ��get_function_tag��ȡ��Functor�����(category)!
		typedef typename detail::function::get_function_tag<Functor>::type tag;

		// ���ݲ�ͬ����Functor��ȡ��ͬ��assign���ԣ�
		this->assign_to(f, tag()); // ת�������������assign_to�汾�������ġ�
	}

	/*
	����ֵ��ע����ǣ�
	get_function_tag<>����ȡ��Functor�����(category)�������漸�����

	struct function_ptr_tag {}; // ����ָ��
	struct function_obj_tag {}; // �º�������
	struct member_ptr_tag {}; // ��Ա����
	struct function_obj_ref_tag {}; // ��ref(obj)���Է�װ����𣬾�����������
	struct stateless_function_obj_tag {}; // ��״̬��������

	���⣬������������������

	has_trivial_constructor
	has_trivial_copy
	has_trivial_destructor
	is_empty

	�ķº��������Ϊstateless�ġ�
	*/

	//���ڲ�ͬ�ĺ������assign_to�и�����ͬ�����ذ汾
	template<typename FunctionPtr> // ����Ǻ���ָ��͵�������汾
	void assign_to(FunctionPtr f, function_ptr_tag) // ����汾��Ժ���ָ��
	{
		clear();

		if(f){
			typedef typename get_function_invoker0<FunctionPtr,R>::type invoker_type; // ��ȡ��Ӧ��invoker
			//::type�����ͽ��ᱻ�Ƶ�Ϊfunction_invoker0<R(*)()>

			invoker = &invoker_type::invoke; // invoke��һ��static��Ա����

			// ������ԣ�һ������ָ��
			function_base::manager = &functor_manager<FunctionPtr, Allocator>::manage;

			// ����function�ĺ���ָ���º�������ָ���������������
			function_base::functor = function_base::manager(make_any_pointer((void (*)())(f)),clone_functor_tag); // ����һ��
		}
	}


	result_type operator()() const // ��Ϊ�º����ı�־��
	{

		// ������������ĺ������ã�ʹ��invoker��
		// invoker��functionN�ĳ�Ա�������������assign_to�б���ֵ��
		// functorΪʵ�ʱ����õĺ�����º�����a0��Ȼ���������
		internal_result_type result = invoker(function_base::functor); 

		// ���õ��Ľ��cast�����շ��س�ȥ������
		return static_cast<result_type>(result);
	}

	// any_pointer�����Ľ���
	typedef internal_result_type(*invoker_type)(any_pointer);

	invoker_type invoker; // ��Ҫ��Ա��������ú�����


};

//��Ҫ˵�����ǣ����ں���ָ�룬function_base����֪��Ҳ��������Ҫ����ĺ���ָ����ʲôȷ�е����ͣ�
//ֻҪ�Ǻ���ָ����У���Ϊ���ܻ�Ѹú���ָ��fת��Ϊ��void (*)()�����ͣ�Ȼ�󱣴���functor��Ա�У�functor��Ա��һ��union����:
union any_pointer
{
	// ����º�������ָ�붼������static_castת��Ϊvoid*��
	void* obj_ptr;

	// Ϊconst�º���׼����
	const void* const_obj_ptr;

	// ���⺯��ָ�붼������reinterpret_castת��Ϊvoid(*)()��
	void (*func_ptr)();

	char data[1];
};
//���any_pointer����ͨ����ȫת�ͱ���������ʽ�ķº����ͺ���ָ�룬�����ڵײ㱣�����ݵ�����

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