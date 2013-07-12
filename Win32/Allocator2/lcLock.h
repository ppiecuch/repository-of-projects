#ifndef _LUCID_LOCK_H_
#define _LUCID_LOCK_H_

//同步---CriticalSection,Mutex,Event,Semaphores  
//refer to:http://kulong0105.blog.163.com/blog/static/174406191201182133344113/
//关于线程的同步对象可分为内核对象与非内核对象，最大区别在于内核对象能跨越进程，而非内核对象不能跨越进程，只能同步单个进程中的线程。
//内核对象：（非内核对象： CriticalSection）
//1. 进程，Processe         
//2. 线程，Threads         
//3. 文件，Files                  
//4. 控制台输入，Console input  
//5. 文件变化通知，File change notifications         
//6. 互斥量，Mutexes          
//7. 信号量，Semaphores
//8. 事件Events              
//9. 可等的计时器Waitable timers         
//10.Jobs
//每一个上面这些类型的对象都可以处于两种状态之一：有信号（signaled）和无信号（nonsignaled）。可用就是有信号状态，被占用就是无信号状态。
//比如进程和线程在终结时其内核对象变为有信号，而在它们处于创建和正在运行时，其内核对象是无信号的。
//内核对象同步应用：
//1.某线程获得某进程的内核对象句柄时，可以改变进程优先级、获得进程的退出码；使本线程与某进程的终结取得同步等。
//2.当获得某线程的内核对象句柄时，可以改变该线程运行状态、与该线程的终结取得同步等。
//3.当获得文件句柄时，本线程可与某一个异步文件的I/O操作获得同步等。
//4.控制台输入对象可用来使线程在有输入进入时被唤醒以执行相关任务等。
//5.其它内核对象―――文件改变通知、互斥量、信号量、事件、可等计时器等―――都只是为了同步对象而存在


//WaitForMultipleObjects（）与WaitForSingleObject（）类似，同时监视位于句柄数组中的所有句柄。
//这些被监视对象的句柄享有平等的优先权，任何一个句柄都不可能比其他句柄具有更高的优先权。WaitForMultipleObjects（）的函数原型为：
//DWORD WaitForMultipleObjects(
//		DWORD nCount, // 等待句柄数
//		CONST HANDLE *lpHandles, // 句柄数组首地址
//		BOOL fWaitAll, // 等待标志
//		DWORD dwMilliseconds // 等待时间间隔
//);
//参数nCount指定了要等待的内核对象的数目，存放这些内核对象的数组由lpHandles来指向。
//fWaitAll对指定的这nCount 个内核对象的两种等待方式进行了指定，为TRUE时当所有对象都被通知时函数才会返回，为FALSE则只要其中任何一个得到通知就可以返回。 
//dwMilliseconds在这里的作用与在WaitForSingleObject（）中的作用是完全一致的。如果等待超时，函数将返回 WAIT_TIMEOUT。
//如果返回WAIT_OBJECT_0到WAIT_OBJECT_0+nCount-1中的某个值，则说明所有指定对象的状态均为已通知状态（当fWaitAll为TRUE时）
//或是用返回值减去WAIT_OBJECT_0可得到发生通知的对象的索引（当fWaitAll为FALSE 时）。
//如果返回值在WAIT_ABANDONED_0与WAIT_ABANDONED_0+nCount-1之间，则表示所有指定对象的状态均为已通知，且其中至少有一个对象是被丢弃的互斥对象（当fWaitAll为TRUE时），
//或是用返回值减去WAIT_OBJECT_0可得到表示一个等待正常结束的互斥对象的索引（当fWaitAll为FALSE时）。


//CritiaclSection：
//临界区是保证在某一时刻只有一个线程能访问数据的简便办法。在任意时刻只允许一个线程对共享资源进行访问，如果有多个线程试图同时访问临界区，
//那么在有一个线程进入后其他所有试图访问此临界区的线程将被挂起，并一直持续到进入临界区的线程离开。临界区在被释放后，其他线程可以继续抢占，
//并以此达到用原子方式操作共享资源的目的。
//在所有同步对象中，临界区是最容易使用的，但它只能用于同步单个进程中的线程，并且不是内核对象，它不由操作系统的低级部件管理，
//而且不能使用句柄来操纵，由于不是内核对象，使得它作为一种轻量级的同步机制，同步速度比较快。

//注意，拥有该临界区的线程，每一次针对此临界区的EnterCriticalSection调用都会成功（这里指的是重复调用也会立即返回，也就是支持嵌套调用），
//且会使得临界区标志（即一个CRITICAL_SECTION全局变量）的引用计数增加1。在另一个线程能够拥有该临界区之前，
//拥有它的线程必须调用LeaveCriticalSection足够多次，在引用计数降为零后，另一线程才有可能拥有该临界区。
//换言之，在一个正常使用临界区的线程中，EnterCriticalSection和LeaveCriticalSection应该成对使用。


//Mutex：（互斥对象包含一个使用数量，一个线程ID和一个引用计数器）
//互斥量与CRITICAL_SECTION类似。拥有该互斥量的线程，每次调用WaitForSingleObject都会立即成功返回，但互斥量的使用计数将增加，
//同样的，也要多次调用ReleaseMutex以使引用计数变为零，方可供别的线程使用。

//TODO:正常释放与非正常释放返回值WAIT_ABANDONED与WAIT_OBJECT_0的区别
//TODO:WaitForSingleObject与WaitForMultipleObjects的不同


//Event：（分为自动重置事件和人工重置事件）
//使用临界区只能同步同一进程中的线程，而使用事件内核对象则可以对进程外的线程进行同步，其前提是得到对此事件对象的访问权。


//Semaphores：（允许多个线程同时访问一个资源）
//信号量对象对线程的同步方式与前面几种方法不同，信号允许多个线程同时使用共享资源。
//它允许多个线程在同一时刻访问同一资源，但是需要限制在同一时刻访问此资源的最大线程数目。

//用CreateSemaphore（）创建信号量时即要同时指出允许的最大资源计数和当前可用资源计数。
//一般是将当前可用资源计数设置为最大资源计数，每增加一个线程对共享资源的访问，当前可用资源计数就会减1，只要当前可用资源计数是大于0的，就可以发出信号量信号。
//但是当前可用计数减小到0时则说明当前占用资源的线程数已经达到了所允许的最大数目，不能在允许其他线程的进入，此时的信号量信号将无法发出。
//线程在处理完共享资源后，应在离开的同时通过ReleaseSemaphore（）函数将当前可用资源计数加1。
//在任何时候当前可用资源计数决不可能大于最大资源计数。 信号量是通过计数来对线程访问资源进行控制的，而实际上信号量确实也被称作Dijkstra计数器。 

//CreateEvent跟CreateMutex区别
//refer to:http://hi.baidu.com/zhaoyong200518/item/e8ce09020182171aebfe38fc
//CreateMutex比CreateEvnet
//1 多了个递归计数器。每WaitforsingleObject一下，计数器加一；
//2 Mutex 的WaitForSingleObject后，被设置为无信息状态，当线程运行结束前，没用ReleaseMutex，系统会线程结束后自动帮做这步操作，以使其他线程能正常使用mutex，不至阻塞死。
//而Event不会，必须用SetEvent()设置为有信号状态；
//3 同一个线程函数，第一个WaitforSingleObject调用后，状态为无信号，不过遇到第二个，第三个时，还是会继续运行，因为是同一线程，
//即使设置了无信号也是对于其他线程而言，对本身还是有信号的，同时没WaitForSingleObject一下，递归计数器加一。Event不会这样。
//这个也带来了Mutex的另一个问题，线程1多次调用WaitForSingleObject后，其他线程如果想要有信息，那线程1必须ReleaseMutex对应Wait的次数或使线程1结束。
//还有就是同一线程多次OpenMutex，而没及时CloseHandle容易导致句柄泄漏。

enum ENUM_LOCK{
	NIL = 0,
	CRITICAL_SECTION,
	MUTEX,
	EVENT,
	ENUM_LOCK_COUNT
};

template<ENUM_LOCK type>
class Lock{
public:
	lock(){}
	unlock(){}
	virtual bool canCrossProcess() const = 0;
};
#endif