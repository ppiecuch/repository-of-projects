
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

template<typename T>
class Typename{
public:
	static const char* get(){ return T::getTypeName(); }
};

typedef int s32;

// add more specialisations for the build-in types
template<> const char* Typename<s32>::get() { static const char* p="int"; return p; }
template<> const char* Typename<s32*>::get() { static const char* p="int*"; return p; }

template<typename T>
class Test{
	T t;
public:
	static const char* getTypeName(){
		static std::string p=std::string("Test<")+Typename<T>::get()+">";
		return p.c_str();
	}
};

class Object{
public:
	static const char* getTypeName(){
		static const char* p="Object";
		return p;
	}
};

int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

	Test<int> i;
	printf("%s\r\n",i.getTypeName());

	Test<int*> ip;
	printf("%s\r\n",ip.getTypeName());

	Test<Object> o;
	printf("%s\r\n",o.getTypeName());

	system("pause");
	return 0;
}