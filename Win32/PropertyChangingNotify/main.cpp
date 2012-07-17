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
			//changed=true;//虽然嵌套类在外围类内部定义，但它是一个独立的类，基本上与外围类不相关。它的成员不属于外围类，同样，外围类的成员也不属于该嵌套类。
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