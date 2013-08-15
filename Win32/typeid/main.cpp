
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "MyGUI_RTTI.h"

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

class Base{
	MYGUI_RTTI_BASE(Base)
};

class Extend : public Base{
	MYGUI_RTTI_DERIVED(Extend)
};

class Derived : public Extend{
	MYGUI_RTTI_DERIVED(Derived)
};

class Other{
public:
	virtual ~Other(){}
};

class A{

};


int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

	Base a;
	Extend b;
	Derived c;
	Other d;
	bool result;

	printf("%d\r\n",a.isType<Base>());
	printf("%d\r\n",a.isType<Extend>());
	printf("%d\r\n",a.isType<Derived>());
	printf("%d\r\n",b.isType<Base>());
	printf("%d\r\n",b.isType<Extend>());
	printf("%d\r\n",b.isType<Derived>());
	printf("%d\r\n",c.isType<Base>());
	printf("%d\r\n",c.isType<Extend>());
	printf("%d\r\n",c.isType<Derived>());

	printf("%d\r\n",typeid(float)==typeid(0.f));

	printf("%d\r\n",typeid(Base)==typeid(Extend));
	printf("%d\r\n",typeid(a)==typeid(Base));
	printf("%d\r\n",typeid(a)==typeid(b));
	printf("%d\r\n",typeid(&a)==typeid(&b));

	result=typeid(Base*)==typeid(static_cast<Base*>(&b));
	printf("%08X,%08X,dynamic extend:%d\r\n",dynamic_cast<Base*>(&b),typeid(Base*),result);

	result=typeid(dynamic_cast<Base*>(&b))==typeid(Base*);
	printf("dynamic inherit:%d\r\n",result);
	printf("%08X,%08X,dynamic other:%d\r\n",dynamic_cast<Base*>(&c),typeid(Base*),typeid(Base*)==typeid(dynamic_cast<Base*>(&c)));
	//printf("%08X,%08X,dynamic other:%d\r\n",dynamic_cast<Base*>(&c),typeid(Base*),typeid(Base*)==typeid(static_cast<Base*>(&c)));

	system("pause");
	return 0;
}