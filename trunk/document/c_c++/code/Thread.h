#pragma once
#include <process.h>
#include <windows.h>


#define THREAD_DEFAULT_INTERVAL 1000
class Thread
{
private:
	static void __cdecl _Proc(void* pArguments)
	{
		Thread* p=(Thread*)pArguments;
		bool loop=false;
		p->m_now=GetTickCount();
		do{
			if(p->m_now-p->m_last<p->m_interval)
			{
				Sleep(p->m_interval-(p->m_now-p->m_last));
				printf("sleep:%ld\r\n",p->m_interval-(p->m_now-p->m_last));
			}
			if(!p->runnable)break;
			p->m_last=GetTickCount();
			printf("time:%ld\r\n",GetTickCount());
			loop=p->run();
			p->m_now=GetTickCount();
		}while(p->runnable&&loop);
		//_endthread();
	}
	unsigned long m_last,m_now;
	bool runnable;
protected:
	uintptr_t m_thread;
	unsigned long m_interval;
public:
	Thread(void):m_thread(0),m_interval(THREAD_DEFAULT_INTERVAL),m_last(0),m_now(0),runnable(true){}
	Thread(const Thread&){}
	virtual ~Thread(void){}
	void start(){m_thread=_beginthread(_Proc,4096,this);}
	void stop(){runnable=false;}
	virtual bool run() = NULL;
};
