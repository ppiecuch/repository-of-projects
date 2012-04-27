// demo.cpp : 定义控制台应用程序的入口点。
//
#include "log.h"
#include "stdafx.h"
#include <crtdbg.h>
#include <typeinfo>
#include "Thread.h"
#include <iostream>
#include <string>
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

int _tmain(int argc, _TCHAR* argv[])
{
	/*Trace("%d\r\n",argc);
	cout<<argc<<endl;
	for(int i=0;i<argc;i++)
	{
		Trace("%s\r\n",argv[i]);
		cout<<argv[i]<<endl;
	}*/
	EnableMemLeakCheck();

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


	getchar();
	return 0;
}

