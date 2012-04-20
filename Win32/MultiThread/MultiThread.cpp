// MultiThread.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "thread.h"
#include <windows.h>

class MyThread:public Thread
{
private:
	int id;
	int count;
public:
	MyThread(int id):count(0)
	{
		this->id=id;
		printf("construct:%d\r\n",this->id);
	}
	~MyThread()
	{
		printf("destruct:%d\r\n",this->id);
	}
	bool run()
	{
		printf("run,id:%d,count:%d\r\n",this->id,count);
		count++;
		return count<5;
	}
};
#define THREAD_NUM 5
MyThread* ts[THREAD_NUM];
int _tmain(int argc, _TCHAR* argv[])
{
	for(int i=0;i<THREAD_NUM;i++)
	{
		ts[i]=new MyThread(i);
		ts[i]->start();
	}
	Sleep(2000);
	for(int i=0;i<THREAD_NUM;i++)
	{
		ts[i]->stop();
		delete ts[i];
	}
	//delete[] &ts;
	return 0;
}

