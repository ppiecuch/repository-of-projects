#include <stdio.h>
#include <wtypes.h>
#include <stdarg.h>
#include <tchar.h>

#include <sys/timeb.h>
#include <time.h>

inline void TRACE(const char * pszFormat, ...)
{
	va_list pArgs;
	char szMessageBuffer[16380]={0};
	va_start( pArgs, pszFormat );
	vsnprintf_s( szMessageBuffer, 16380,16380-1,pszFormat, pArgs );
	va_end( pArgs );   
	OutputDebugStringA(szMessageBuffer);   
}

//����ʹ��#�Ѻ������Ϊһ���ַ���,��##�����������������һ��. 
void test(){}

class Test{
public:
	static void test(){}

	static int demo(int a,char* b){return 0;}

	void abcd(char c){}
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
	_REGISTER_FUNC(Test::demo);
	_REGISTER_FUNC(&Test::abcd);
	_REGISTER_FUNC(_TEST);

#if 0
	//�����������һ�����ʱ��
	//��Ҫע����Ƿ��궨��������'#'��'##'�ĵط�������ǲ�����չ��.
#define A          (2)
#define STR(s)     #s
#define CONS(a,b)  int(a##e##b)

	printf("int max: %s\n",  STR(INT_MAX));    // INT_MAX ��i nclude
	//���лᱻչ��Ϊ��
	//printf("int max: %s\n", "INT_MAX");

	printf("%s\n", CONS(A, A));               // compile error 
	//��һ�����ǣ�
	//printf("%s\n", int(AeA));

#elif 0

#define COUNT 100000000
	struct _timeb start;
	struct _timeb end;
	long long s;
	int ms;

	_ftime64_s( &start ); 
	//_FUNC_NAME 0.218ms,string 0.234ms
	for(int i=0;i<COUNT;++i){
		//_FUNC_NAME(test);
		char* c="test";
	}
	_ftime64_s( &end ); 

	s=end.time-start.time;
	ms=end.millitm-start.millitm;
	if(ms<0){
		ms+=1000;
		--s;
	}
	printf("%d.",s);
	printf("%03d\n",ms);

#elif 0
	//INT_MAX��A�������ٱ�չ��, Ȼ������������ķ����ܼ�. �Ӷ�һ���м�ת����.
	//������������ǰ����к�Ĳ����������ȫ��չ��, ��ô��ת���������һ����(_STR)���ܵõ���ȷ�ĺ����.
#define A 2
#define MY_STR(s)     #s
#define STR(s)      MY_STR(s)
#define MY_CONS(a,b) int(a##e##b)
#define M_CONS(a,b) MY_CONS(a,b)

	printf("int max: %s\n",  STR(INT_MAX));    // INT_MAX ��include
	printf("%d\n", M_CONS(A, A));               // compile error 
#endif
	system("pause");
	return 0;
}