#ifndef _YON_CORE_YONMUTEX_H_
#define _YON_CORE_YONMUTEX_H_

#include "IReferencable.h"

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

namespace yon{
namespace core{

	class mutex{

#ifdef YON_COMPILE_WITH_WIN32
	typedef CRITICAL_SECTION sem_t;
#endif

	public:
		mutex(){
#ifdef YON_COMPILE_WITH_WIN32
			//每个线程中访问临界资源的那段程序称为临界区（Critical Section）（临界资源是一次仅允许一个线程使用的共享资源）。
			//每次只准许一个线程进入临界区，进入后不允许其他线程进入。
			InitializeCriticalSection( &m_semaphore );
#else
			//函数sem_init（）用来初始化一个信号量。
			//它的原型为：　extern int sem_init __P ((sem_t *__sem, int __pshared, unsigned int __value));
			//sem为指向信号量结构的一个指针；
			//pshared不为0时此信号量在进程间共享，否则只能为当前进程的所有线程共享；
			//value给出了信号量的初始值。
			//Linux线程目前不支持进程间共享信号量，pshared传递一个非零将会使函数返回ENOSYS错误。
			sem_init( &m_semaphore, 0, 1 );
#endif
		}
		~mutex(){
#ifdef YON_COMPILE_WITH_WIN32
			DeleteCriticalSection( &m_semaphore );
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
			EnterCriticalSection( &m_semaphore );
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
			LeaveCriticalSection( &m_semaphore );
#else
			//函数sem_post( sem_t *sem )用来增加信号量的值当有线程阻塞在这个信号量上时，调用这个函数会使其中的一个线程不再阻塞，选择机制同样是由线程的调度策略决定的。
			sem_post( &m_semaphore );
#endif
		}
		bool tryWait(){
#ifdef YON_COMPILE_WITH_WIN32
			if(TryEnterCriticalSection( &m_semaphore ) != 0) // _WIN32_WINNT >= 0x400
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