#include <Windows.h>
class mutex{

	typedef CRITICAL_SECTION sem_t;

public:
	mutex(){
		//每个线程中访问临界资源的那段程序称为临界区（Critical Section）（临界资源是一次仅允许一个线程使用的共享资源）。
		//每次只准许一个线程进入临界区，进入后不允许其他线程进入。
		InitializeCriticalSection( &m_semaphore );
	}
	~mutex(){
		DeleteCriticalSection( &m_semaphore );
	}
	void wait(){
		EnterCriticalSection( &m_semaphore );
	}
	void notify(){
		LeaveCriticalSection( &m_semaphore );
	}
	bool tryWait(){
		if(TryEnterCriticalSection( &m_semaphore ) != 0) // _WIN32_WINNT >= 0x400
		{
			return true;
		}	
		else
		{
			return false;	
		}
	}
private:
	sem_t m_semaphore;
};