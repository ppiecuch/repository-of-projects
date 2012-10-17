#include <stdio.h>

#include "Singleton.h"

class Test{
public:
	void test(){ printf("test\r\n");}

	DECLARE_SINGLETON_CLASS(Test);
};

int main(int argc, char* argv[])
{
	Test::getInstance().test();
	getchar();
}