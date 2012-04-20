#pragma once
#include <process.h>


class Thread
{
private:
	static void __cdecl _Proc(void* pArguments)
	{
		Thread* p=(Thread*)pArguments;
		p->run();
	}
protected:
	uintptr_t m_thread;
public:
	Thread(void):m_thread(0){}
	Thread(const Thread&){}
	virtual ~Thread(void){}
	void start(){m_thread=_beginthread(_Proc,4096,this);}
	virtual void run() = NULL;
};
