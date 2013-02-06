#include <stdio.h>
#include <stdlib.h>

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

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}


template<typename T>
void test(T a,float b){

}

#define __FUNC_NAME(p) #p
#define _FUNC_NAME(p) __FUNC_NAME(p)

template<typename T,size_t S=sizeof(T)>
class Obj{
	Obj(){
		//new int;
	}
public:
	static Obj& getInstance(){
		static Obj o;
		return o;
	}
	virtual ~Obj(){
		TRACE("release %d\n",S);
		/*FILE* file;
		errno_t result=fopen_s(&file,"D:/Obj.txt","a+");
		if(result)
			return;
		fprintf(file,"release:%s\r\n",_FUNC_NAME(T));
		fclose(file);*/
	}
	void test(){printf("test\r\n");}
};

template<size_t Num>
class Test{
public:
	Test(){printf("%d\n",Num);}
};

template<typename T,typename OT=Obj<T> >
class Contain{
	//1、如果不定义OT变量，则应用程序不会创建Obj对象，虽然你在模板参数列表中声明了它
	//2、如果使用static OT t定义变量;一样不会创建，除非使用了它的API如t.tetst();
	//使用static，这样我们的t就会在堆中创建，并且相同的sizeof(T)会使用相同的对象
	static OT& t;
public:
	Contain(){
		t.test();
	}
	virtual ~Contain(){printf("release Contain\n");}
};
template <typename T,typename OT >
OT& Contain<T,OT>::t=OT::getInstance();

int i;

class NN{};
template <typename T>
class MM{
public:
	void test(const T& t){
	}
};

int main(int argc, char* argv[])
{
	EnableMemLeakCheck();
	//for(i=0;i<3;++i)
	//	Test<i>();

	MM<const NN> m;
	NN n;
	m.test(n);
	/*{
		Contain<int> c1;
		Contain<double> c2;
		Contain<int> c3;
		Contain<int> c4;

		//此处static了一个新对象不与Contain中的t同用
		//static Obj<int> t;

		//使用单例解决此问题
		static Obj<int>& t=Obj<int>::getInstance();
		t.test();
	}
	test(1.0f,1);
	test(1,10);*/

	system("pause");
	return 0;
}