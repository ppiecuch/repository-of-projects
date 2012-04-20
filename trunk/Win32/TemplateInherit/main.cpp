#include <stdio.h>
#include <iostream>

using namespace std;

class A{
public:
	template<typename T> void test(){
		printf("%d",sizeof(T));
	}

	A& operator<<(bool value){
		printf("%s","<<");
		return *this;
	}
};

template<>
void A::test<char>(){
	printf("%d",2);
}
class B : public A{
};

#if 0
template<typename T>
class class1
{
public:
	class1(T t):m_val(t){}
	virtual T getVal(){
		cout<<"in class1,val =="<< m_val <<endl;
		return m_val;
	}
private:
	 T m_val;
};
template<typename T>
class derived2: public class1<T>
{
public:
	derived2(T val):class1<T>(val){}
	T getVal()
	{
		cout<<"in derived2"<<endl;
		return class1<T>::getVal();
	}
};
#endif

int main(int argc, char* argv[])
{
	B b;
	b.test<bool>();
	b<<true;
#if 0
	derived2<char> d('a');
	d.getVal();
#endif
	getchar();
	return 0;
}