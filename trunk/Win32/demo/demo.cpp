// demo.cpp : 定义控制台应用程序的入口点。
//
#include "log.h"
#include "stdafx.h"
#include <crtdbg.h>
#include <typeinfo>
#include "Thread.h"
#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

class Exception
{
protected:
	std::string msg;
public:
	Exception(){}
	Exception(const char* msg){this->msg=std::string(msg);}
	Exception(std::string msg){this->msg=msg;}
	std::string getMessage(){return msg;}
	virtual std::string getClassName()=0;
};
class MyException:public Exception
{
public:
	MyException(string msg):Exception(msg){}
	string getClassName(){return "MyException";}
};

class MyThread:public Thread{
static int counter;
public:
	void run()
	{
		counter++;
		Trace("run:%d\r\n",counter);
	}
};
int MyThread::counter=0;

inline int gcd(unsigned int a,unsigned int b){  
	int k=1,t;  
	while(~a&1 && ~b&1)k<<=1,a>>=1,b>>=1;  
	t=(a&1)?-b:a>>1;  
	do{  
		while(~t&1)t>>=1;  
		if(t>0)a=t;else b=-t;  
	}while(t=a-b);  
	return a*k;  
}  

class SColor
{
public:
	SColor()
		:color(0){}

	SColor(unsigned int clr)
		: color(clr) {}

	void print(){
		printf("%X\n",color);
	}

	unsigned int color;
};

class I{
public:
	int a;
	virtual void test(){}
};
class J : public I{
public:
	int b;
};

struct Desp{
	std::string Name;
	int Type;
};

int _tmain(int argc, _TCHAR* argv[])
{

#if 0
	/*Trace("%d\r\n",argc);
	cout<<argc<<endl;
	for(int i=0;i<argc;i++)
	{
		Trace("%s\r\n",argv[i]);
		cout<<argv[i]<<endl;
	}*/
	EnableMemLeakCheck();

	I* i=new I();
	try{
		J* j=dynamic_cast<J*>(i);
		printf("cast:%d\r\n",j);
	}catch(...){
		printf("can't cast\r\n");
	}

	SColor c1(0xFF132E47);

	SColor c2=c1;
	c2.print();

	/*
	MyThread t1;
	t1.start();
	MyThread t2;
	t2.start();

	t1.start();*/

	/*try{
		cout<<"test"<<endl;
		throw MyException("exp");
	}catch(Exception& e){
		cout<<e.getClassName()<<endl;
		cout<<e.getMessage().c_str()<<endl;
	}
	cout<<"outer"<<endl;*/

	cout<<gcd(60,1000)<<endl;
	cout<<gcd(5,1000)<<endl;
	cout<<gcd(7,1000)<<endl;
#else

	Desp ds[]={
		{"name1",0},
		{"name2",1},
		{"name3",2}
	};

	for(int i=0;i<3;++i)
		cout<<ds[i].Name<<','<<ds[i].Type<<endl;
#endif

	system("pause");
	return 0;
}

