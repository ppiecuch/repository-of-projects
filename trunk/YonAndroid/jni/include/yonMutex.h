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

namespace yon{
namespace core{

	class mutex : public virtual IReferencable{

#ifdef YON_COMPILE_WITH_WIN32
	typedef CRITICAL_SECTION sem_t;
#endif

	public:
		mutex():m_uCount(0),m_iPid(0){
#ifdef YON_COMPILE_WITH_WIN32
			//ÿ���߳��з����ٽ���Դ���Ƕγ����Ϊ�ٽ�����Critical Section�����ٽ���Դ��һ�ν�����һ���߳�ʹ�õĹ�����Դ����
			//ÿ��ֻ׼��һ���߳̽����ٽ�������������������߳̽��롣
			InitializeCriticalSection( &m_semaphore );
#else
			//����sem_init����������ʼ��һ���ź�����
			//����ԭ��Ϊ����extern int sem_init __P ((sem_t *__sem, int __pshared, unsigned int __value));
			//semΪָ���ź����ṹ��һ��ָ�룻
			//pshared��Ϊ0ʱ���ź����ڽ��̼乲������ֻ��Ϊ��ǰ���̵������̹߳���
			//value�������ź����ĳ�ʼֵ��
			//Linux�߳�Ŀǰ��֧�ֽ��̼乲���ź�����pshared����һ�����㽫��ʹ��������ENOSYS����
			sem_init( &m_semaphore, 0, 1 );
#endif
		}
		~mutex(){
#ifdef YON_COMPILE_WITH_WIN32
			DeleteCriticalSection( &m_semaphore );
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
			//��ȡ��ǰ����һ��Ψһ�ı�ʶ��
			if(m_iPid != GetCurrentProcessId())
			{
				EnterCriticalSection( &m_semaphore );
				m_iPid = GetCurrentProcessId();
			}	
#else
			//����NPTL���߳̿⣬���߳�Ӧ���е�ÿ���߳����Լ����ص��߳� ID��������ͬһ������ID��
			//���� LinuxThreads ���߳̿⣬���߳�Ӧ���е�ÿ���߳�ӵ���Լ����صĽ��� ID��getpid���� �ڲ�ͬ���߳��е��û᷵�ز�ͬ��ֵ
			//NPTL �� LinuxThreads ����Ϊ��ʽ��ͬ��NPTL ���ں��и�ÿ�����̽ṹ�������ר�ŵ� tid ��ʶ����ʾ�߳� id��ͬʱһ�������е��̹߳���һ�����̺š�
			//����˵����getpid��������ȡ��Ŀǰ���̵Ľ���ʶ���룬����������ȡ���Ĵ�ֵ��������ʱ�ļ����Ա�����ʱ�ļ���ͬ���������⡣
			//����ֵ��Ŀǰ���̵Ľ���ʶ����
			if(m_iPid != getpid())
			{
				//����sem_wait( sem_t *sem )������������ǰ�߳�ֱ���ź���sem��ֵ����0�����������sem��ֵ��һ������������Դ��ʹ�ú���١�
				sem_wait( &m_semaphore );
				m_iPid = getpid();
			}	
#endif
			++m_uCount;
		}
		void notify(){
#ifdef YON_COMPILE_WITH_WIN32
			if(m_iPid == GetCurrentProcessId())
#else
			if(m_iPid == getpid())
#endif
			{
				if(m_uCount == 1)
				{
					m_iPid = 0 ;				
				}	
				if(m_uCount > 0)
				{
					--m_uCount;
				}			
				if(m_uCount == 0)
#ifdef YON_COMPILE_WITH_WIN32
					LeaveCriticalSection( &m_semaphore );
#else
					//����sem_post( sem_t *sem )���������ź�����ֵ�����߳�����������ź�����ʱ���������������ʹ���е�һ���̲߳���������ѡ�����ͬ�������̵߳ĵ��Ȳ��Ծ����ġ�
					sem_post( &m_semaphore );
#endif
			}	
		}
		bool tryWait(){
#ifdef YON_COMPILE_WITH_WIN32
			if(m_iPid != GetCurrentProcessId())
			{
				if(TryEnterCriticalSection( &m_semaphore ) != 0) // _WIN32_WINNT >= 0x400
				{
					m_iPid = GetCurrentProcessId();
					++m_uCount;
					return true;
				}	
				else
				{
					return false;	
				}
			}	
			else
			{
				++m_uCount;
				return true;
			}
#else
			if(m_iPid != getpid())
			{
				//����sem_trywait ( sem_t *sem )�Ǻ���sem_wait�����ķ������汾����ֱ�ӽ��ź���sem��ֵ��һ��
				if(sem_trywait( &m_semaphore )==0)
				{
					m_iPid = getpid();
					m_uCount++;
					return true;
				}	
				else
				{
					return false;	
				}
			}	
			else
			{
				++m_uCount;
				return true;
			}
#endif
		}
	private:
		sem_t m_semaphore;
		u32 m_uCount;	 
		s32 m_iPid;
	};
}
}
#endif