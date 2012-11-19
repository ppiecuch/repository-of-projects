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
	//~Derived(){delete a;printf("non-virtual destruct Derived\r\n");}
	virtual ~Derived(){delete a;printf("virtual destruct Derived\r\n");}
};

int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

	Base* base=new Derived();
	delete base;

	system("pause");
	return 0;
}