#ifndef _YON_CORE_YONSEMAPHORE_H_
#define _YON_CORE_YONSEMAPHORE_H_

#include "IReferencable.h"
#include "yonString.h"
#include "ILogger.h"

#ifdef YON_COMPILE_WITH_WIN32
#include <Windows.h>
#else
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#endif

//refer to:http://blog.sina.com.cn/s/blog_759dc36b0100p8y4.html
//refer to:http://wenx05124561.blog.163.com/blog/static/1240008052011717114011994/
//refer to:http://m.oschina.net/blog/56927
//refer to:http://en.usenet.digipedia.org/thread/1485/4413/
//refer to:http://rednaxelafx.iteye.com/blog/716918
//refer to:http://msdn.microsoft.com/en-us/library/ms685129%28VS.85%29.aspx
//信号量与互斥锁 
//refer to:http://www.cnblogs.com/diyingyun/archive/2011/12/04/2275229.html
//refer to:http://blog.163.com/iam_zhangyong/blog/static/18234533820112147419797/

//最初的进程定义都包含程序、资源及其执行三部分，其中程序通常指代码，资源在操作系统层面上通常包括内存资源、IO资源、信号处理等部分，
//而程序的执行通常理解为执行上下文，包括对cpu的占用，后来发展为线程。在线程概念出现以前，为了减小进程切换的开销，操作系统设计者逐渐修正进程的概念，
//逐渐允许将进程所占有的资源从其主体剥离出来，允许某些进程共享一部分资源，例如文件、信号，数据内存，甚至代码，这就发展出轻量进程的概念。
//Linux 线程在核内是以轻量级进程的形式存在的，拥有独立的进程表项，而所有的创建、同步、删除等操作都在核外pthread库中进行, 而调度交给内核处理。
//轻量级进程和普通进程的区别在于：前者没有独立的用户空间(内核态线程无用户空间，用户态线程共享用户空间)，而普通进程有独立的内存空间 

//POSIX.1 标准定义了一系列线程函数的接口，即 POSIX threads(Pthreads)。
//Linux C 库提供了两种关于线程的实现：LinuxThreads 和 NPTL(Native POSIX Threads Library)。LinuxThreads 已经过时，一些函数的实现不遵循POSIX.1 规范。
//NPTL 依赖 Linux 2.6 内核，更加遵循 POSIX..1 规范，但也不是完全遵循。

//Unix系统中每一个进程都包含一个task_struct数据结构，所有指向这些数据结构的指针组成一个进程向量数组，系统缺省的进程向量数据大小是512，表示系统中可同时容纳512个进程。
//进程的task_struct数据结构包括了进程的状态、调度信息、进程标识符等信息。
//由于UNIX系统是一个多进程的操作系统，所以每一个进程都是独立的，都有自己的权限及任务，所以当某一进程失败时并不会导致别的进程失败。
//系统通过进程标识符来区分不同的进程，进程标识符是一个非负正数，他在任何时刻都是唯一的，当某个进程结束时，他的进程标识符可以分配给另外一个新进程。
//系统将标识符 0分配给调度进程，标识符1分配给初始化进程。
//Linux 内核并不支持真正意义上的线程，LinuxThreads是用与普通进程具有同样内核调度视图的轻量级进程来实现线程支持的。
//这些轻量级进程拥有独立的进程id，在进程调度、信号处理、IO等方面享有与普通进程一样的能力。

// Under the old linux thread library, linux gives each thread  
// its own process id. Because of this each thread will return  
// a different pid if this method were to return the result  
// of getpid(2). Linux provides no api that returns the pid  
// of the launcher thread for the vm. This implementation  
// returns a unique pid, the pid of the launcher thread  
// that starts the vm 'process'.  

// Under the NPTL, getpid() returns the same pid as the  
// launcher thread rather than a unique pid per thread.  
// Use gettid() if you want the old pre NPTL behaviour.  

//TODO：如何鉴别当前线程是LinuxThread还是NPTL

//TODO
//refer to:A Fast Mostly User Mode Inter-Process Mutex
//http://www.codeproject.com/Articles/3271/A-Fast-Mostly-User-Mode-Inter-Process-Mutex

//TODO m_iPid似乎不能用，不用反而是正常的

//Mutex在计算机中是互斥也就是排他持有的一种方式，和信号量-Semaphore有可以对比之处。有人做过如下类比： 
//Mutex是一把钥匙，一个人拿了就可进入一个房间，出来的时候把钥匙交给队列的第一个。一般的用法是用于串行化对critical section代码的访问，保证这段代码不会被并行的运行。
//Semaphore是一件可以容纳N人的房间，如果人不满就可以进去，如果人满了，就要等待有人出来。对于N=1的情况，称为binary semaphore。一般的用法是，用于限制对于某一资源的同时访问。
//对于Binary semaphore与Mutex，这两者之间就存在了很多相似之处：
//在有的系统中Binary semaphore与Mutex是没有差异的。在有的系统上，主要的差异是mutex一定要由获得锁的进程来释放。
//而semaphore可以由其它进程释放（这时的semaphore实际就是个原子的变量，大家可以加或减），因此semaphore可以用于进程间同步。
//Semaphore的同步功能是所有系统都支持的，而Mutex能否由其他进程释放则未定，因此建议mutex只用于保护critical section。而semaphore则用于保护某变量，或者同步。

namespace yon{
namespace core{

	/**
	 * 信号量，是可以用来保护两个或多个关键代码段，这些关键代码段不能并发调用。
	 * 在进入一个关键代码段之前，线程必须获取一个信号量。
	 * 如果关键代码段中没有任何线程，那么线程会立即进入该框图中的那个部分。
	 * 一旦该关键代码段完成了，那么该线程必须释放信号量。其它想进入该关键代码段的线程必须等待直到第一个线程释放信号量。
	 */
	class semaphore{

#ifdef YON_COMPILE_WITH_WIN32
	typedef HANDLE sem_t;
#endif

	public:
		semaphore(s32 maxNum=1):m_semaphore(NULL){
#ifdef YON_COMPILE_WITH_WIN32
			//HANDLE WINAPI CreateSemaphore(
			//	_In_opt_  LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
			//	_In_      LONG lInitialCount,
			//	_In_      LONG lMaximumCount,
			//	_In_opt_  LPCTSTR lpName
			//	);
			//创建信号量
			core::stringc name("%08X",this);
			m_semaphore=CreateSemaphoreA(NULL,maxNum,maxNum,name.c_str());
			if(m_semaphore==NULL)
				YON_ERROR(YON_LOG_FAILED_FORMAT,core::stringc("CreateSemaphore failed for %d",GetLastError()).c_str());
#else
			//函数sem_init（）用来初始化一个信号量。
			//它的原型为：　extern int sem_init __P ((sem_t *__sem, int __pshared, unsigned int __value));
			//sem为指向信号量结构的一个指针；
			//pshared不为0时此信号量在进程间共享，否则只能为当前进程的所有线程共享；
			//value给出了信号量的初始值。
			//Linux线程目前不支持进程间共享信号量，pshared传递一个非零将会使函数返回ENOSYS错误。
			sem_init( &m_semaphore, 0, maxNum );
#endif
		}
		~semaphore(){
#ifdef YON_COMPILE_WITH_WIN32
			//Closes an open object handle.
			//BOOL WINAPI CloseHandle(
			//_In_  HANDLE hObject
			//);
			if(m_semaphore&&!CloseHandle(m_semaphore))
				YON_ERROR(YON_LOG_FAILED_FORMAT,core::stringc("CloseHandle failed for %d",GetLastError()).c_str());
#else
			//释放信号量
			//int sem_destroy (sem_t *sem);
			//这个函数也使用一个信号量指针做参数，归还自己占据的一切资源。在清理信号量的时候如果还有线程在等待它，用户就会收到一个错误。
			//然而在linux的线程中，其实是没有任何资源关联到信号量对象需要释放的，因此在linux中，销毁信号量对象的作用仅仅是测试是否有线程因为该信号量在等待。
			//如果函数返回0说明没有，正常注销信号量，如果返回EBUSY，说明还有线程正在等待该信号量的信号。
			//与其它的函数一样，这些函数在成功时都返回“0”。
			sem_destroy( &m_semaphore );
#endif
		}
		void wait(){
#ifdef YON_COMPILE_WITH_WIN32
			//Waits until the specified object is in the signaled state or the time-out interval elapses.
			//To enter an alertable wait state, use the WaitForSingleObjectEx function. To wait for multiple objects, use the WaitForMultipleObjects.
			//DWORD WINAPI WaitForSingleObject(
			//	_In_  HANDLE hHandle,
			//	_In_  DWORD dwMilliseconds
			//	);
			YON_DEBUG_BREAK_IF(m_semaphore==NULL);
			WaitForSingleObject(m_semaphore,INFINITE);
#else
			//sem_wait函数也是一个原子操作，它的作用是从信号量的值减去一个“1”，但它永远会先等待该信号量为一个非零值才开始做减法。
			//也就是说，如果你对一个值为2的信号量调用sem_wait(),线程将会继续执行，这信号量的值将减到1。
			//如果对一个值为0的信号量调用sem_wait()，这个函数就 会地等待直到有其它线程增加了这个值使它不再是0为止。
			//函数sem_wait( sem_t *sem )被用来阻塞当前线程直到信号量sem的值大于0，解除阻塞后将sem的值减一，表明公共资源经使用后减少。
			sem_wait( &m_semaphore );
#endif
		}
		void notify(){
#ifdef YON_COMPILE_WITH_WIN32
			//Increases the count of the specified semaphore object by a specified amount.
			//BOOL WINAPI ReleaseSemaphore(
			//	_In_       HANDLE hSemaphore,
			//	_In_       LONG lReleaseCount,
			//	_Out_opt_  LPLONG lpPreviousCount
			//	);
			YON_DEBUG_BREAK_IF(m_semaphore==NULL);
			ReleaseSemaphore(m_semaphore,1,NULL);
#else
			//函数sem_post( sem_t *sem )用来增加信号量的值当有线程阻塞在这个信号量上时，调用这个函数会使其中的一个线程不再阻塞，选择机制同样是由线程的调度策略决定的。
			sem_post( &m_semaphore );
#endif
		}
		//TODO Win32待测试
		bool tryWait(){
#ifdef YON_COMPILE_WITH_WIN32
			if(WaitForSingleObject(m_semaphore,0) == WAIT_OBJECT_0)
			{
				return true;
			}	
			else
			{
				return false;	
			}
#else
			//函数sem_trywait ( sem_t *sem )是函数sem_wait（）的非阻塞版本，它直接将信号量sem的值减一。
			if(sem_trywait( &m_semaphore )==0)
			{
				return true;
			}	
			else
			{
				return false;	
			}
#endif
		}
	private:
		sem_t m_semaphore;
	};
}
}
#endif