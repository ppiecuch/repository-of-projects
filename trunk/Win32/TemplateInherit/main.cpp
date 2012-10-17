#include <stdio.h>
#include <iostream>

using namespace std;

//member templateΪʲô����Ϊvirtual?
//Ϊ�˲���C++̫���ӣ�����C++�ı�����̫������C++����ô�涨�ˡ�
//template�Ǿ�̬������ʱ����virtual�Ƕ�̬��(����ʱ),���߲��ɵü�
#if 0
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

#elif 0
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

#elif 0
template<class T>
class C{
public:
	virtual void test(){printf("testC\r\n");}
};

class D : public C<int>{
public:
	virtual void test(){
		C<int>::test();
		printf("testD\r\n");
	}
};

#else
//���μ̳�
class a{
public:
	virtual void demo() = 0;
};
class A : public virtual a{
public:
	virtual void test()= 0;
};

class b : public a{
public:
	virtual void demo(){printf("demob\r\n");}
};

class B : public A,b{
public:
	virtual void test(){printf("testB\r\n");}
	virtual void demo(){b::demo();printf("demoB\r\n");}
};

#endif
int main(int argc, char* argv[])
{
#if 0
	B b;
	b.test<bool>();
	b<<true;
#elif 0
	derived2<char> d('a');
	d.getVal();
#elif 0
	D d;
	d.test();
#else
	A* t=new B();
	t->demo();
	t->test();
	delete t;
#endif
	getchar();
	return 0;
}