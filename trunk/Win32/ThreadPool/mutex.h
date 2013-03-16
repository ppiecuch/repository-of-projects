#include <Windows.h>
class mutex{

	typedef CRITICAL_SECTION sem_t;

public:
	mutex(){
		//ÿ���߳��з����ٽ���Դ���Ƕγ����Ϊ�ٽ�����Critical Section�����ٽ���Դ��һ�ν�����һ���߳�ʹ�õĹ�����Դ����
		//ÿ��ֻ׼��һ���߳̽����ٽ�������������������߳̽��롣
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