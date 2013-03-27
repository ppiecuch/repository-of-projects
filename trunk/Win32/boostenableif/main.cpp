#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define FIXED

//Boost::Utility�� ֮enable_if
//refer to:http://hi.baidu.com/hins_pan/item/b156890ba81b3ad873e6762d

class some_class {
public:
	typedef int type;
};


void some_func(int i) {
	std::cout << "void some_func(" << i << ")\n";
}

#ifdef FIXED

//�����û��ѧ�� SFINAE (ƥ��ʧ�ܲ��Ǵ���), �ܿ�������������ϻ�������ı��顣
//�����޸Ĺ��Ĵ��룬���ǵ����ӻ�ͨ�����롣
//short ������Ϊ int, ���ҵ�һ���汾�����á��������˾������Ϊ��ԭ����ģ��汾�� some_func ���ٰ��������ؾ���ļ������ˡ�
//�����ų���������Ϊ����������������������ķ�������Ҫ��ģ������T Ҫ��һ��Ƕ������type ������֪�� short ���������Ҫ��
//���������������ģ������ؾ��鼯����ɾ���ˡ������ Daveed Vandevorde �� Nicolai Josuttis �̸����ǵ� SFINAE, 
//����ζ�����ɶ�����������Ͳ����Ǻ��������أ�Ҳ��Ҫ����һ���������������������һ������������Ƕ�����ͣ���ô���������ؾ��鼯�ϵ�һ���֡�

template <typename T> 
//typename T::type* some_func(T t) {			//��һ�ַ���
void some_func(T t,typename T::type* p=0) {		//�ڶ��ַ���
	//typename T::type variable_of_nested_type;
	std::cout <<"template <typename T> void some_func()\n";
	return;
}

#else

template <typename T> 
void some_func(T t) {
	typename T::type variable_of_nested_type;
	std::cout << "template <typename T> void some_func(" << t << ")\n";
}

#endif

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

int main(int argc,char ** argv)
{
	EnableMemLeakCheck();

	int i=12;
	short s=12;

	some_func(i);
	some_func(s); //error C2825: 'T': ���������::��ʱ����Ϊ��������ռ�
	some_func(some_class());

	
	system("pause");
	return 0;
}