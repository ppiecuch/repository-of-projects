#ifndef _LUCID_THREAD_H_
#define _LUCID_THREAD_H_

namespace lc{

class IRunnable{
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


class thread{
public:
	enum State{
		NONE,
		READY,
		RUNNING,
		SUSPENDED,
		STOPPED,
		COUNT
	};
	virtual ~thread(){}
	virtual void start() = 0;
	virtual thread::State getState() = 0;
	static thread* createInstance(IRunnable* r);
};

}
#endif