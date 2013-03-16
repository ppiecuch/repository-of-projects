
#include <stdio.h>
#include <wtypes.h>
#include <stdarg.h>
#include <tchar.h>

inline void TRACE(const char * pszFormat, ...)
{
	va_list pArgs;
	char szMessageBuffer[16380]={0};
	va_start( pArgs, pszFormat );
	vsnprintf_s( szMessageBuffer, 16380,16380-1,pszFormat, pArgs );
	va_end( pArgs );   
	OutputDebugStringA(szMessageBuffer);   
}

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

class IRunnable {
public:
	virtual void run() = 0;
};

#include <windows.h>
#include <list>
#include "mutex.h"

class Thread{
	HANDLE m_handle;
	DWORD m_threadId;
	IRunnable* m_pRunnable;
	static DWORD WINAPI threadProc(LPVOID data);
public:
	Thread(IRunnable* r);
	void start();
	void suspend();
};

DWORD Thread::threadProc(LPVOID data){
	Thread* thread = static_cast<Thread*>(data);
	thread->m_pRunnable->run();
	return 0;
}

Thread::Thread(IRunnable* r)
:m_handle(NULL),m_threadId(-1),m_pRunnable(r){
	DWORD dwCreationFlags = CREATE_SUSPENDED;
	m_handle = ::CreateThread(NULL, 0, threadProc, this, dwCreationFlags, &m_threadId);
	if(m_handle==NULL)
		TRACE("CreateThread failed,for error:%d!",GetLastError());
}
void Thread::start(){
	if(::ResumeThread(m_handle)==-1)
		TRACE("ResumeThread failed,for error:%d!",GetLastError());
}

void Thread::suspend()
{
	if(::SuspendThread(m_handle)==-1)
		TRACE("SuspendThread failed,for error:%d!",GetLastError());
}


struct SMyRunnable1 : public IRunnable{
	virtual void run(){
		TRACE("start SMyRunnable1()\r\n");
		for(int i=0;i<100;++i)
		{
			Sleep(100);
			TRACE("SMyRunnable1(%d)\r\n",i);
		}
		TRACE("end SMyRunnable1()\r\n");
	}
};

template<typename T> 
class delegate { 

public: 
	delegate(T* pT, void (T::*pFunc)()) 
		: m_pT(pT), m_pFunc(pFunc) { } 

	void invoke() { 
		(m_pT->*m_pFunc)(); 
	} 

private: 
	T* m_pT; 
	void (T::*m_pFunc)(); 
};

template<> 
class delegate<void> { 

public: 
	delegate(void (*pFunc)()) 
		: m_pFunc(pFunc) { } 

	void invoke() { 
		(*m_pFunc)(); 
	} 

private: 
	void (*m_pFunc)(); 
};

template<typename T>
class ThreadPool{
	struct Td{
		HANDLE Handle;
		DWORD ThreadId;
		int Index;
		bool Waiting;
		std::string Name;
		ThreadPool<T>* Owner;
	};
	std::list<delegate<T>*> m_works;
	Td* m_threads;
	int m_threadSize;
	mutex m_mutex;
	static DWORD WINAPI threadProc(LPVOID data);
	static void suspend(Td* thread);
	void wakeThread();
public:
	ThreadPool(int num);
	~ThreadPool();
	void add(delegate<T>* work);
	void start();
};

template<typename T>
DWORD ThreadPool<T>::threadProc(LPVOID data){
	Td* t = static_cast<Td*>(data);
	std::list<delegate<T>*>& works=t->Owner->m_works;
	mutex& mt=t->Owner->m_mutex;
	HANDLE handle=t->Handle;
	std::string& name=t->Name;
	delegate<T>* work;
	while(true)
	{
		mt.wait();
		if(works.empty())
			work=NULL;
		else
		{
			work=works.front();
			works.pop_front();
		}
		mt.notify();
		if(work)
			work->invoke();
		else
		{
			TRACE("%s wait\r\n",name.c_str());
			suspend(t);
		}

	}
	return 0;
}
template<typename T>
ThreadPool<T>::ThreadPool(int num)
:m_threads(NULL),m_threadSize(num)
{
	DWORD dwCreationFlags = CREATE_SUSPENDED;
	m_threads=new Td[num];
	for(int i=0;i<num;++i)
	{
		char buffer[1024];
		sprintf_s(buffer,1024,"Thread_%08X_%d",this,i);
		m_threads[i].Name.append(buffer);
		m_threads[i].Index=i;
		m_threads[i].Owner=this;
		m_threads[i].Waiting=true;
		m_threads[i].Handle = ::CreateThread(NULL, 0, threadProc, &m_threads[i], dwCreationFlags, &m_threads[i].ThreadId);
		if(m_threads[i].Handle==NULL)
			TRACE("CreateThread[i] failed,for error:%d!",i,GetLastError());
	}
}
template<typename T>
ThreadPool<T>::~ThreadPool(){
	if(m_threads)
	{
		delete[] m_threads;
		m_threads=NULL;
	}
}
template<typename T>
void ThreadPool<T>::start(){
	for(int i=0;i<m_threadSize;++i)
	{
		if(::ResumeThread(m_threads[i].Handle)==-1)
		{
			TRACE("ResumeThread[%d] failed,for error:%d!\r\n",i,GetLastError());
			return;
		}
		TRACE("ResumeThread[%s] success!\r\n",m_threads[i].Name.c_str());
		m_threads[i].Waiting=false;
	}
}
template<typename T>
void ThreadPool<T>::suspend(Td* thread)
{
	//TRACE("SuspendThread[%s] success!\r\n",thread->Name.c_str());
	thread->Waiting=true;
	if(::SuspendThread(thread->Handle)==-1)
	{
		TRACE("SuspendThread[%s] failed,for error:%d!\r\n",thread->Name.c_str(),GetLastError());
		return;
	}
	
}
template<typename T>
void ThreadPool<T>::add(delegate<T>* work)
{
	m_mutex.wait();
	m_works.push_back(work);
	TRACE("add work:%08X\r\n",work);
	m_mutex.notify();
	wakeThread();
}
template<typename T>
void ThreadPool<T>::wakeThread()
{
	for(int i=0;i<m_threadSize;++i)
	{
		if(m_threads[i].Waiting)
		{
			if(::ResumeThread(m_threads[i].Handle)==-1)
			{
				TRACE("ResumeThread[%s] failed,for error:%d!\r\n",m_threads[i].Name.c_str(),GetLastError());
				continue;
			}
			TRACE("ResumeThread[%s] success!\r\n",m_threads[i].Name.c_str());
			m_threads[i].Waiting=false;
		}
	}
}
int counter=1;
mutex mm;
void test(){
	mm.wait();
	counter<<=1;
	counter-=1;
	TRACE("test:%d\r\n",counter);
	mm.notify();
}
int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

	/*SMyRunnable1 r1;

	Thread t1(&r1);
	t1.start();

	TRACE("has start\r\n");*/

	delegate<void> d(test);

	ThreadPool<void> pool(10);
	pool.start();

	for(int i=0;i<100;++i)
	{
		//getchar();
		pool.add(&d);
	}

	system("pause");
	return 0;
}