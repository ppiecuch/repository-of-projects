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
	//1�����������OT��������Ӧ�ó��򲻻ᴴ��Obj������Ȼ����ģ������б�����������
	//2�����ʹ��static OT t�������;һ�����ᴴ��������ʹ��������API��t.tetst();
	//ʹ��static���������ǵ�t�ͻ��ڶ��д�����������ͬ��sizeof(T)��ʹ����ͬ�Ķ���
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

		//�˴�static��һ���¶�����Contain�е�tͬ��
		//static Obj<int> t;

		//ʹ�õ������������
		static Obj<int>& t=Obj<int>::getInstance();
		t.test();
	}
	test(1.0f,1);
	test(1,10);*/

	system("pause");
	return 0;
}