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

//{return malloc(sz);}
//void operator delete(LC_ALLOC_PARAMS(void* ptr));

//refer to:http://www.parashift.com/c++-faq-lite/templates-defn-vs-decl.html
//Why can't I separate the definition of my templates class from its declaration and put it inside a .cpp file? 
//1、A template is not a class or a function. A template is a "pattern" that the compiler uses to generate a family of classes or functions. 
//2、In order for the compiler to generate the code, it must see both the template definition (not just declaration) and 
//the specific types/whatever used to "fill in" the template. For example, if you're trying to use a Foo<int>, 
//the compiler must see both the Foo template and the fact that you're trying to make a specific Foo<int>. 
//3、Your compiler probably doesn't remember the details of one .cpp file while it is compiling another .cpp file. 
//It could, but most do not and if you are reading this FAQ, it almost definitely does not. BTW this is called the "separate compilation model." 

/*

class BaseAllocator{
protected:
	lcDefineException(OverflowException);
	lcDefineException(BadAllocException);

	struct MemRecord{
		MemRecord():Ptr(NULL),File(NULL),Func(NULL),Line(-1),Size(0){}
		MemRecord(void* ptr,u32 size):Ptr(ptr),File(NULL),Func(NULL),Line(-1),Size(size){}
		MemRecord(void* ptr,u32 size,const c8* file,const c8* func,s32 line):Ptr(ptr),File(file),Func(func),Line(line),Size(size){}

		void* Ptr;
		const c8* File;
		const c8* Func;
		s32 Line;
		u32 Size;

	};
	//specify you own out-of-memory handler as set_new_handler()
	typedef void (*pOutOfMemHandler)();
	typedef core::map<void*,MemRecord> MemRecordMap;
	pOutOfMemHandler m_pOutOfMemHandler;
	MemRecordMap m_recordMap;
	u32 m_uAllocatedSize,m_uMaxSize;

	virtual void* oomMalloc(LC_ALLOC_PARAMS(size_t sz)) = 0;
	virtual void internalDeallocate(const MemRecord& r) = 0;
	virtual void* internalAllocate(LC_ALLOC_PARAMS(size_t sz)) = 0;
	virtual void internalDeallocate(LC_ALLOC_PARAMS(void *ptr)) = 0;
public:
	BaseAllocator();
	virtual ~BaseAllocator();

	template<typename T>
	T* allocate(LC_ALLOC_PARAMS(size_t sz))
	{
		return (T*)internalAllocate(LC_ALLOC_ARGS_SL(sz));
	}
	template<typename T>
	void deallocate(LC_ALLOC_PARAMS(T *ptr))
	{
		internalDeallocate(LC_ALLOC_ARGS_SL((void*)ptr));
	}

	void destroy();

	template<typename T>
	void construct(T* ptr, const T&e)
	{
		new ((void*)ptr) T(e);
	}

	template<typename T>
	void destruct(T* ptr)
	{
		ptr->~T();
	}

	void setOutOfMemHandler(pOutOfMemHandler pFunc);

	void setMaxSize(u32 size);
	u32 getMaxSize();

	void setAllocatedSize(u32 size);
	u32 getAllocatedSize();
};


template<ENUM_ALLOC_STRATEGY stgy=PRIMITIVE>
class Allocator : public BaseAllocator{
protected:
	virtual void* oomMalloc(LC_ALLOC_PARAMS(size_t sz))
	{
		pOutOfMemHandler pHandler = NULL;
		void* pResult = NULL;

		//do it repeatedly
		for( ; ; )
		{
			pHandler = m_pOutOfMemHandler;

			if( NULL == pHandler )
				lcThrow(BadAllocException);

			//call process
			(*pHandler)();

			//try again
			pResult = operator new(LC_ALLOC_ARGS_SL(sz));
			if( pResult )
			{
#ifdef LC_TRACK_DETAIL
				MemRecord r(pResult,LC_ALLOC_ARGS_SL(sz));
				m_recordMap.set(pResult,r);
#endif
				return pResult;
			}
		}
	}

	virtual void internalDeallocate(const MemRecord& r)
	{
		//operator delete(LC_ALLOC_ARGS_MR(r));
		operator delete(r.Ptr);
		m_uAllocatedSize-=r.Size;
	}
public:

	virtual void* internalAllocate(LC_ALLOC_PARAMS(size_t sz))
	{
		if(m_uAllocatedSize+sz>m_uMaxSize)
			lcThrow(OverflowException);

		void *pResult = operator new(LC_ALLOC_ARGS_SL(sz));

		if( NULL == pResult )
			pResult = oomMalloc(LC_ALLOC_ARGS_SL(sz));

#ifdef LC_TRACK_DETAIL
		MemRecord r(pResult,LC_ALLOC_ARGS_SL(sz));
		m_recordMap.set(pResult,r);
#endif

		return pResult;
	}

	virtual void internalDeallocate(LC_ALLOC_PARAMS(void *ptr))
	{
		if(ptr==NULL)
			return;

		MemRecord r(LC_ALLOC_ARGS_SL(ptr,0));
#ifdef LC_TRACK_DETAIL
		if(m_recordMap.remove(ptr,&r))
#endif
		{
			internalDeallocate(r);
		}
	}
};



	
class Allocator{
public:
	//specify you own out-of-memory handler as set_new_handler()
	typedef void (*pOutOfMemHandler)();
private:
	static pOutOfMemHandler s_pOutOfMemHandler;

	static void* OOMMalloc(size_t sz)
	{
		pOutOfMemHandler pHandler = NULL;
		void* pResult = NULL;

		//do it repeatedly
		for( ; ; )
		{
			pHandler = s_pOutOfMemHandler;

			if( NULL == pHandler )
				throw std::bad_alloc();

			//call process
			(*pHandler)();

			//try again
			pResult = std::malloc(sz);
			if( pResult )
				return pResult;
		}
	}
public:

	virtual ~Allocator(){}

	static void* allocate(size_t sz)
	{
		void *pResult = std::malloc(sz);

		if( NULL == pResult )
			pResult = OOMMalloc(sz);

		return pResult;
	}

	static void deallocate(void *p, size_t)
	{
		// 直接使用free
		std::free(p);
	}

	
	static void (*Allocator::setOutOfMemHandler(pOutOfMemHandler pFunc))()
	{
		pOutOfMemHandler pOldFunc = s_pOutOfMemHandler;
		s_pOutOfMemHandler = pFunc;

		return pOldFunc;
	}
};

LC_SELECTANY Allocator::pOutOfMemHandler Allocator::s_pOutOfMemHandler = NULL;
*/


}

#endif