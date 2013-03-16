#ifndef _YON_CORE_THREADPOOL_H_
#define _YON_CORE_THREADPOOL_H_

#include "IReferencable.h"

namespace yon{
namespace core{

	class IWork : public virtual IReferencable{
	public:
		virtual ~IWork(){}
		virtual void process() = 0;
	};

	class threadpool : public virtual IReferencable{
	public:
		virtual ~threadpool(){}
		virtual void push(IWork* work) = 0;
		virtual void run() = 0;
		virtual void stop() = 0;
	};

	extern "C" YON_API threadpool* createThreadPool(u32 num);
}
}
#endif