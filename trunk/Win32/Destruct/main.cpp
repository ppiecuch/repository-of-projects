#include <stdio.h>
#include <stdlib.h>

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

/************************************************************************/
/* C++��ȷָ������derived class ������һ��classָ�뱻ɾ��������base class
   ����һ��non-virtual��������������δ�ж��塪��ʵ��ִ��ʱͨ���������Ƕ���
   ��derived�ɷ�û��������
*/
/************************************************************************/
class Base{
public:
	//~Base(){printf("non-virtual destruct Base\r\n");}
	virtual ~Base(){printf("virtual destruct Base\r\n");}
};
class Derived : public Base{
	int* a;
public:
	Derived(){
		a=new int;
	}
	~Derived(){delete a;printf("non-virtual destruct Derived\r\n");}
	//virtual ~Derived(){delete a;printf("virtual destruct Derived\r\n");}
};

class Super : public Derived{
	int* b;
public:
	Super(){
		b=new int;
	}
	~Super(){delete b;printf("non-virtual destruct Super\r\n");}
};

class mystring{
public:
	mystring(){
		printf("construct\r\n");
	}
	~mystring(){
		printf("destruct\r\n");
	}
	int c_str(){
		printf("c_str\r\n");
		return 1;
	}
};

void test(int num){
	printf("test num\r\n");
}

int main(int argc, char* argv[])
{
	EnableMemLeakCheck();


	//test(mystring().c_str());
	//int num=mystring().c_str();
	//printf("num\r\n");

	Base* base=new Super();
	delete base;

	system("pause");
	return 0;
}