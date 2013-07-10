#ifndef _LUCID_ALLOCATOR_H_
#define _LUCID_ALLOCATOR_H_

#include "lcMap.h"
#include "lcException.h"
//如果你想在预分配的内存上创建对象，用缺省的new操作符是行不通的。要解决这个问题，你可以用placement new构造。
#include <new>

//refer to:SGIMallocPool.hhp

#include <stdio.h>
#include <wtypes.h>
#include <stdarg.h>
#include <tchar.h>

inline void TRACE(const char * pszFormat, ...)
{
	va_list pArgs;
	char szMessageBuffer[16380]={0};
	va_start( pArgs, pszFormat );
	vsnprintf_s( szMessageBuffer, 16380,16380-1,pszFormat, pArgs );
	va_end( pArgs );   
	OutputDebugStringA(szMessageBuffer);   
}

namespace lc{

#ifdef LC_TRACK_DETAIL
#define LC_ALLOC_PARAMS(pr) pr,const c8* file,const c8* func,s32 line
#define LC_ALLOC_ARGS_MT(...) __VA_ARGS__,LC_FILE,LC_FUNC,LC_LINE
#define LC_ALLOC_ARGS_MR(r) r.Ptr,r.File,r.Func,r.Line
#define LC_ALLOC_ARGS_SL(...) __VA_ARGS__,file,func,line
#else
#define LC_ALLOC_PARAMS(pr) pr
#define LC_ALLOC_ARGS_MT(...) __VA_ARGS__
#define LC_ALLOC_ARGS_MR(r) r.Ptr
#define LC_ALLOC_ARGS_SL(...) __VA_ARGS__
#endif

#define LC_ALLOCATE(allocator,type,sz) allocator.allocate<type>(LC_ALLOC_ARGS_MT(sz))
#define LC_DEALLOCATE(allocator,ptr) allocator.deallocate<void>(LC_ALLOC_ARGS_MT(ptr))

enum ENUM_ALLOC_STRATEGY{
	PRIMITIVE = 0,
	ENUM_ALLOC_STRATEGY_COUNT
};

void* operator new(LC_ALLOC_PARAMS(size_t sz));
void operator delete(LC_ALLOC_PARAMS(void* ptr));

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
	BaseAllocator():m_uAllocatedSize(0),m_uMaxSize(LC_MEMORY_MAX_SIZE),m_pOutOfMemHandler(NULL){}
	virtual ~BaseAllocator(){}

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

	void destroy(){
#ifdef LC_TRACK_DETAIL
		MemRecordMap::Iterator it=m_recordMap.getIterator();
		for(;!it.atEnd();++it)
		{
			const MemRecord& r=it->getValue();
			TRACE("Check memory leak in 0x%08X(%u bytes) in %s at \"%s(...)\" line:%d\r\n",r.Ptr,r.Size,r.File,r.Func,r.Line);
			MemRecord m(LC_ALLOC_ARGS_MT(r.Ptr,r.Size));
			internalDeallocate(m);
		}
#endif
		m_recordMap.clear();
	}

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

	void setOutOfMemHandler(pOutOfMemHandler pFunc)
	{
		pOutOfMemHandler pOldFunc = m_pOutOfMemHandler;
		m_pOutOfMemHandler = pFunc;
	}

	void setMaxSize(u32 size)
	{
		m_uMaxSize=size;
	}

	u32 getMaxSize()
	{
		return m_uMaxSize;
	}

	void setAllocatedSize(u32 size)
	{
		m_uAllocatedSize=size;
	}

	u32 getAllocatedSize()
	{
		return m_uAllocatedSize;
	}
};


template<ENUM_ALLOC_STRATEGY stgy=PRIMITIVE>
class Allocator : public BaseAllocator{
protected:
	void* oomMalloc(LC_ALLOC_PARAMS(size_t sz))
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
		operator delete(LC_ALLOC_ARGS_MR(r));
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

/*
	
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