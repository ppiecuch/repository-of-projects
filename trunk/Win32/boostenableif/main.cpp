#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define FIXED

//Boost::Utility库 之enable_if
//refer to:http://hi.baidu.com/hins_pan/item/b156890ba81b3ad873e6762d

class some_class {
public:
	typedef int type;
};


void some_func(int i) {
	std::cout << "void some_func(" << i << ")\n";
}

#ifdef FIXED

//如果你没有学过 SFINAE (匹配失败不是错误), 很可能现在你的脸上会有困惑的表情。
//编译修改过的代码，我们的例子会通过编译。
//short 被提升为 int, 并且第一个版本被调用。这种令人惊奇的行为的原因是模板版本的 some_func 不再包含在重载决议的集合内了。
//它被排除在内是因为，编译器看到了这个函数的返回类型要求模板类型T 要有一个嵌套类型type ，而它知道 short 不满足这个要求，
//所以它把这个函数模板从重载决议集合中删掉了。这就是 Daveed Vandevorde 和 Nicolai Josuttis 教给我们的 SFINAE, 
//它意味着宁可对有问题的类型不考虑函数的重载，也不要产生一个编译器错误。如果类型有一个符合条件的嵌套类型，那么它就是重载决议集合的一部分。

template <typename T> 
//typename T::type* some_func(T t) {			//第一种方法
void some_func(T t,typename T::type* p=0) {		//第二种方法
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
	some_func(s); //error C2825: 'T': 当后面跟“::”时必须为类或命名空间
	some_func(some_class());

	
	system("pause");
	return 0;
}