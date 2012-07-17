#include <stdio.h>
#include <list>
using namespace std;

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

class Test{
public:
	bool changed;
	template<typename T>
	class AccessClass
	{
	public:
		void operator=(const T& newValue)
		{
			value=newValue;
			//changed=true;//��ȻǶ��������Χ���ڲ����壬������һ���������࣬����������Χ�಻��ء����ĳ�Ա��������Χ�࣬ͬ������Χ��ĳ�ԱҲ�����ڸ�Ƕ���ࡣ
			printf("changed:%d\n",value);
		}
		operator T()
		{
			return value;
		}
	private:
		T value;
	};

	AccessClass<bool> a;
	AccessClass<int> b;
};

int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

	Test t;
	t.a=true;
	t.b=10;
	printf("%d\n",t.b);
	getchar();
	return 0;
}