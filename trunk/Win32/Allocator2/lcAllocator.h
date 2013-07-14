#ifndef _LUCID_ALLOCATOR_H_
#define _LUCID_ALLOCATOR_H_

#include "lcException.h"
#include "lcSingleton.h"
#include "lcBackwardCompatibility.h"
#include "lcMutex.h"
//如果你想在预分配的内存上创建对象，用缺省的new操作符是行不通的。要解决这个问题，你可以用placement new构造。
#include <new>

//refer to:SGIMallocPool.hhp

enum ENUM_ALLOC_STRATEGY{
	PRIMITIVE = 0,
	ENUM_ALLOC_STRATEGY_COUNT
};

namespace lc{


//refer to:http://holos.googlecode.com/svn/trunk/src/h_MemTracer.h
class MemoryTracer : public core::Singleton<MemoryTracer>,public Obsolete<MemoryTracer>{
private:
	struct TracerNode{
		TracerNode():Prev(NULL),Next(NULL),Addr(NULL),Size(0),File(NULL),Func(NULL),Line(-1){}
		TracerNode(void* p,u32 size):Prev(NULL),Next(NULL),Addr(p),Size(size),File(NULL),Func(NULL),Line(-1){}
		TracerNode(void* p,u32 size,const c8* file,const c8* func,s32 line):Prev(NULL),Next(NULL),Addr(p),Size(size),File(file),Func(func),Line(line){}

		TracerNode* Prev;
		TracerNode* Next;

		void*		Addr;
		u32			Size;
		const c8*	File;
		const c8*	Func;
		s32			Line;
	};

	static mutex s_mutex;

	TracerNode* m_pHead;
	TracerNode* m_pTail;

	u32			m_uAllocatedSize;
	u32			m_uAllocatedCount;
	u32			m_uMaxSize;

	MemoryTracer();
	static void lock();
	static void unlock();
	bool canAllocate(size_t sz);
	void addNode(TracerNode* node);
	void removeNode(TracerNode* node);
	void deallocateNode(TracerNode* node);

	friend class core::Singleton<MemoryTracer>;
	friend void* ::operator new(LC_ALLOC_PARAMS(size_t size));
	friend void ::operator delete(void* ptr);
public:

	void allocate(LC_ALLOC_PARAMS(void* p,size_t sz));
	void deallocate(void* ptr);
	u32 getAllocatedSize() const{return m_uAllocatedSize;}
	u32 getAllocatedCount() const{return m_uAllocatedCount;}
	void setMaxSize(size_t sz){m_uMaxSize=sz;}
	u32 getMaxSize() const{return m_uMaxSize;}
	void dump();
	virtual void destroy();
};

}

#endif