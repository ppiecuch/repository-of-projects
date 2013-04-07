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
//�ź����뻥���� 
//refer to:http://www.cnblogs.com/diyingyun/archive/2011/12/04/2275229.html
//refer to:http://blog.163.com/iam_zhangyong/blog/static/18234533820112147419797/

//����Ľ��̶��嶼����������Դ����ִ�������֣����г���ͨ��ָ���룬��Դ�ڲ���ϵͳ������ͨ�������ڴ���Դ��IO��Դ���źŴ���Ȳ��֣�
//�������ִ��ͨ�����Ϊִ�������ģ�������cpu��ռ�ã�������չΪ�̡߳����̸߳��������ǰ��Ϊ�˼�С�����л��Ŀ���������ϵͳ��������������̵ĸ��
//������������ռ�е���Դ��������������������ĳЩ���̹���һ������Դ�������ļ����źţ������ڴ棬�������룬��ͷ�չ���������̵ĸ��
//Linux �߳��ں����������������̵���ʽ���ڵģ�ӵ�ж����Ľ��̱�������еĴ�����ͬ����ɾ���Ȳ������ں���pthread���н���, �����Ƚ����ں˴���
//���������̺���ͨ���̵��������ڣ�ǰ��û�ж������û��ռ�(�ں�̬�߳����û��ռ䣬�û�̬�̹߳����û��ռ�)������ͨ�����ж������ڴ�ռ� 

//POSIX.1 ��׼������һϵ���̺߳����Ľӿڣ��� POSIX threads(Pthreads)��
//Linux C ���ṩ�����ֹ����̵߳�ʵ�֣�LinuxThreads �� NPTL(Native POSIX Threads Library)��LinuxThreads �Ѿ���ʱ��һЩ������ʵ�ֲ���ѭPOSIX.1 �淶��
//NPTL ���� Linux 2.6 �ںˣ�������ѭ POSIX..1 �淶����Ҳ������ȫ��ѭ��

//Unixϵͳ��ÿһ�����̶�����һ��task_struct���ݽṹ������ָ����Щ���ݽṹ��ָ�����һ�������������飬ϵͳȱʡ�Ľ����������ݴ�С��512����ʾϵͳ�п�ͬʱ����512�����̡�
//���̵�task_struct���ݽṹ�����˽��̵�״̬��������Ϣ�����̱�ʶ������Ϣ��
//����UNIXϵͳ��һ������̵Ĳ���ϵͳ������ÿһ�����̶��Ƕ����ģ������Լ���Ȩ�޼��������Ե�ĳһ����ʧ��ʱ�����ᵼ�±�Ľ���ʧ�ܡ�
//ϵͳͨ�����̱�ʶ�������ֲ�ͬ�Ľ��̣����̱�ʶ����һ���Ǹ������������κ�ʱ�̶���Ψһ�ģ���ĳ�����̽���ʱ�����Ľ��̱�ʶ�����Է��������һ���½��̡�
//ϵͳ����ʶ�� 0��������Ƚ��̣���ʶ��1�������ʼ�����̡�
//Linux �ں˲���֧�����������ϵ��̣߳�LinuxThreads��������ͨ���̾���ͬ���ں˵�����ͼ��������������ʵ���߳�֧�ֵġ�
//��Щ����������ӵ�ж����Ľ���id���ڽ��̵��ȡ��źŴ���IO�ȷ�����������ͨ����һ����������

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

//TODO����μ���ǰ�߳���LinuxThread����NPTL

//TODO
//refer to:A Fast Mostly User Mode Inter-Process Mutex
//http://www.codeproject.com/Articles/3271/A-Fast-Mostly-User-Mode-Inter-Process-Mutex

//TODO m_iPid�ƺ������ã����÷�����������

//Mutex�ڼ�������ǻ���Ҳ�����������е�һ�ַ�ʽ�����ź���-Semaphore�п��ԶԱ�֮������������������ȣ� 
//Mutex��һ��Կ�ף�һ�������˾Ϳɽ���һ�����䣬������ʱ���Կ�׽������еĵ�һ����һ����÷������ڴ��л���critical section����ķ��ʣ���֤��δ��벻�ᱻ���е����С�
//Semaphore��һ����������N�˵ķ��䣬����˲����Ϳ��Խ�ȥ����������ˣ���Ҫ�ȴ����˳���������N=1���������Ϊbinary semaphore��һ����÷��ǣ��������ƶ���ĳһ��Դ��ͬʱ���ʡ�
//����Binary semaphore��Mutex��������֮��ʹ����˺ܶ�����֮����
//���е�ϵͳ��Binary semaphore��Mutex��û�в���ġ����е�ϵͳ�ϣ���Ҫ�Ĳ�����mutexһ��Ҫ�ɻ�����Ľ������ͷš�
//��semaphore���������������ͷţ���ʱ��semaphoreʵ�ʾ��Ǹ�ԭ�ӵı�������ҿ��Լӻ���������semaphore�������ڽ��̼�ͬ����
//Semaphore��ͬ������������ϵͳ��֧�ֵģ���Mutex�ܷ������������ͷ���δ������˽���mutexֻ���ڱ���critical section����semaphore�����ڱ���ĳ����������ͬ����

namespace yon{
namespace core{

	/**
	 * �ź������ǿ��������������������ؼ�����Σ���Щ�ؼ�����β��ܲ������á�
	 * �ڽ���һ���ؼ������֮ǰ���̱߳����ȡһ���ź�����
	 * ����ؼ��������û���κ��̣߳���ô�̻߳���������ÿ�ͼ�е��Ǹ����֡�
	 * һ���ùؼ����������ˣ���ô���̱߳����ͷ��ź��������������ùؼ�����ε��̱߳���ȴ�ֱ����һ���߳��ͷ��ź�����
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
			//�����ź���
			core::stringc name("%08X",this);
			m_semaphore=CreateSemaphoreA(NULL,maxNum,maxNum,name.c_str());
			if(m_semaphore==NULL)
				YON_ERROR(YON_LOG_FAILED_FORMAT,core::stringc("CreateSemaphore failed for %d",GetLastError()).c_str());
#else
			//����sem_init����������ʼ��һ���ź�����
			//����ԭ��Ϊ����extern int sem_init __P ((sem_t *__sem, int __pshared, unsigned int __value));
			//semΪָ���ź����ṹ��һ��ָ�룻
			//pshared��Ϊ0ʱ���ź����ڽ��̼乲������ֻ��Ϊ��ǰ���̵������̹߳���
			//value�������ź����ĳ�ʼֵ��
			//Linux�߳�Ŀǰ��֧�ֽ��̼乲���ź�����pshared����һ�����㽫��ʹ��������ENOSYS����
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
			//�ͷ��ź���
			//int sem_destroy (sem_t *sem);
			//�������Ҳʹ��һ���ź���ָ�����������黹�Լ�ռ�ݵ�һ����Դ���������ź�����ʱ����������߳��ڵȴ������û��ͻ��յ�һ������
			//Ȼ����linux���߳��У���ʵ��û���κ���Դ�������ź���������Ҫ�ͷŵģ������linux�У������ź�����������ý����ǲ����Ƿ����߳���Ϊ���ź����ڵȴ���
			//�����������0˵��û�У�����ע���ź������������EBUSY��˵�������߳����ڵȴ����ź������źš�
			//�������ĺ���һ������Щ�����ڳɹ�ʱ�����ء�0����
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
			//sem_wait����Ҳ��һ��ԭ�Ӳ��������������Ǵ��ź�����ֵ��ȥһ����1����������Զ���ȵȴ����ź���Ϊһ������ֵ�ſ�ʼ��������
			//Ҳ����˵��������һ��ֵΪ2���ź�������sem_wait(),�߳̽������ִ�У����ź�����ֵ������1��
			//�����һ��ֵΪ0���ź�������sem_wait()����������� ��صȴ�ֱ���������߳����������ֵʹ��������0Ϊֹ��
			//����sem_wait( sem_t *sem )������������ǰ�߳�ֱ���ź���sem��ֵ����0�����������sem��ֵ��һ������������Դ��ʹ�ú���١�
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
			//����sem_post( sem_t *sem )���������ź�����ֵ�����߳�����������ź�����ʱ���������������ʹ���е�һ���̲߳���������ѡ�����ͬ�������̵߳ĵ��Ȳ��Ծ����ġ�
			sem_post( &m_semaphore );
#endif
		}
		//TODO Win32������
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
			//����sem_trywait ( sem_t *sem )�Ǻ���sem_wait�����ķ������汾����ֱ�ӽ��ź���sem��ֵ��һ��
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