#include <stdio.h>
#include <wtypes.h>
#include <stdarg.h>
#include <tchar.h>

inline void TRACE(const char * pszFormat, ...)
{
	va_list pArgs;
	char szMessageBuffer[16380]={0};
	va_start( pArgs, pszFormat );
	vsnprintf_s( szMessageBuffer, 16380,16380-1,pszFormat, pArgs );
	va_end( pArgs );   
	OutputDebugStringA(szMessageBuffer);   
}

//我们使用#把宏参数变为一个字符串,用##把两个宏参数贴合在一起. 
void test(){}

class Test{
public:
	static void test(){}
};

#define __FUNC_NAME(p) #p
#define _FUNC_NAME(p) __FUNC_NAME(p)

#define _REGISTER_FUNC(p) \
	do{char* c=_FUNC_NAME(p);printf("register:%s,0x%08X\n",c,p);}while(0);

#define _TEST Test::test

int main(int argc, char* argv[])
{
	_REGISTER_FUNC(test);
	_REGISTER_FUNC(Test::test);
	_REGISTER_FUNC(_TEST);

#if 0
	//当宏参数是另一个宏的时候
	//需要注意的是凡宏定义里有用'#'或'##'的地方宏参数是不会再展开.
#define A          (2)
#define STR(s)     #s
#define CONS(a,b)  int(a##e##b)

	printf("int max: %s\n",  STR(INT_MAX));    // INT_MAX ＃i nclude
	//这行会被展开为：
	//printf("int max: %s\n", "INT_MAX");

	printf("%s\n", CONS(A, A));               // compile error 
	//这一行则是：
	//printf("%s\n", int(AeA));

#else
	//INT_MAX和A都不会再被展开, 然而解决这个问题的方法很简单. 加多一层中间转换宏.
	//加这层宏的用意是把所有宏的参数在这层里全部展开, 那么在转换宏里的那一个宏(_STR)就能得到正确的宏参数.
#define A 2
#define MY_STR(s)     #s
#define STR(s)      MY_STR(s)
#define MY_CONS(a,b) int(a##e##b)
#define M_CONS(a,b) MY_CONS(a,b)

	printf("int max: %s\n",  STR(INT_MAX));    // INT_MAX ＃include
	printf("%d\n", M_CONS(A, A));               // compile error 
#endif
	system("pause");
	return 0;
}