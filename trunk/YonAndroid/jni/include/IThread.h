#ifndef _YON_ITHREAD_H_
#define _YON_ITHREAD_H_

#include "IReferencable.h"

namespace yon{

	enum ENUM_THREAD_STATE{
		ENUM_THREAD_STATE_NONE,
		ENUM_THREAD_STATE_READY,
		ENUM_THREAD_STATE_RUNNING,
		ENUM_THREAD_STATE_SUSPENDED,
		ENUM_THREAD_STATE_STOPPED,
		//TODO need?
		ENUM_THREAD_STATE_DETACHED,
		ENUM_THREAD_STATE_COUNT
	};

	enum ENUM_THRAD_PRIORITY{
		ENUM_THRAD_PRIORITY_LOWEST,
		ENUM_THRAD_PRIORITY_BELOW_NORMAL,
		ENUM_THRAD_PRIORITY_NORMAL,
		ENUM_THRAD_PRIORITY_ABOVE_NORMAL,
		ENUM_THRAD_PRIORITY_HIGHEST,
		ENUM_THRAD_PRIORITY_IDLE,
		ENUM_THRAD_PRIORITY_URGENT,
		ENUM_THRAD_PRIORITY_COUNT
	};

	class IRunnable : public virtual core::IReferencable{
	public:
		virtual void run() = 0;
	};

	template<typename Func>
	struct SRunnable : public IRunnable{
		const Func& func;
		SRunnable(const Func& f):func(f){}
		virtual void run(){
			func();
		}
	};

	class IThread : public virtual core::IReferencable{
	protected:
		//ENUM_THRAD_PRIORITY m_ePriority;
		ENUM_THREAD_STATE m_eState;
	public:
		IThread():m_eState(ENUM_THREAD_STATE_NONE){}
		virtual ~IThread(){}
		//virtual void setPriority(ENUM_THRAD_PRIORITY m_ePriority) = 0;

		//ENUM_THRAD_PRIORITY getPriority() const { return m_ePriority; }
		ENUM_THREAD_STATE getState() const { return m_eState; }

		//virtual bool join(u32 millisec = 0) = 0;
		virtual void suspend() = 0;
		//virtual void resume() = 0;
		//virtual bool run() = 0;
		virtual void start() = 0;
		//virtual bool yield() = 0;
		//virtual void detach() = 0;

	};

#if 0
	//refer to:http://bbs.csdn.net/topics/60302254
	//关于模板函数/类的DLL导出
	//这个在我的 VS2003 下编译器好像是这样的处理方法：
	//1. 如果该模板函数／类在 Dll 内使用过，则将该函数实例导出。
	//2. 如果该模板函数在 Dll 中没有被使用，即未产生实例，则在使用该模板的程序中产生实例，也就是说该 Dll 根本就没有这个模板函数的任何信息，而是通过导入 Dll 时包含的头文件生成模板实例的。
	//3. 如果该模板函数在 Dll 和客户程序中都未使用～～彻底放弃它～～
	template<typename Func> YON_API_TPL IThread* createThread(const Func& f);
#else
	extern "C" YON_API IThread* createThread(IRunnable* r);
#endif
}
#endif