
#ifndef _YON_CORE_YONALLOCATOR_H_
#define _YON_CORE_YONALLOCATOR_H_

#include "yonTypes.h"
#include <new>
#include <memory.h>
#include "memorypool.h"
#include "alignof.h"
#include "alignmentpoolgroup.h"

namespace yon
{
namespace core
{

#ifdef YON_CHECK_MEMORY
#ifdef new
#undef new
#endif
#define new YON_ORIGIN_NEW
#endif

//! Very simple allocator implementation, containers using it can be used across dll boundaries
template<typename T>
class yonAllocator
{
public:

	//! Destructor
	virtual ~yonAllocator() {}

	//! Allocate memory for an array of objects
	T* allocate(size_t cnt)
	{
		return (T*)internal_new(cnt* sizeof(T));
	}

	//! Deallocate memory for an array of objects
	void deallocate(T* ptr)
	{
		internal_delete(ptr);
	}

	//! Construct an element
	void construct(T* ptr, const T&e)
	{
		new ((void*)ptr) T(e);
	}

	//! Destruct an element
	void destruct(T* ptr)
	{
		ptr->~T();
	}

protected:

	virtual void* internal_new(size_t cnt)
	{
		return operator new(cnt);
	}

	virtual void internal_delete(void* ptr)
	{
		operator delete(ptr);
	}

};

template<typename T,size_t S=sizeof(T)>
class yonAllocatorMemoryPool
{
	static memorypool<S> pool;
public:
	yonAllocatorMemoryPool(){}

	//! Destructor
	virtual ~yonAllocatorMemoryPool() {}

	//! Allocate memory for an array of objects
	T* allocate(size_t cnt)
	{
		return (T*)pool.allocate(sizeof(T),cnt);
	}

	//! Deallocate memory for an array of objects
	void deallocate(T* ptr)
	{
		if(ptr==NULL)
			return;
		pool.deallocate(ptr);
	}

	//! Construct an element
	void construct(T* ptr, const T&e)
	{
		new ((void*)ptr) T(e);
	}

	//! Destruct an element
	void destruct(T* ptr)
	{
		ptr->~T();
	}

};

template<typename T,size_t S> 
memorypool<S> yonAllocatorMemoryPool<T,S>::pool;

/*
template<typename T,size_t ChunkSize=256,size_t Align=core::AlignOf<T>::ALIGNMENT>
class yonAllocatorAlign
{
	enum{BLOCK_SIZE=1024};
	enum{CHUNK_COUNT_PER_BLOCK=ChunkSize>BLOCK_SIZE?1:BLOCK_SIZE/ChunkSize};
	static alignmentpool<ChunkSize,CHUNK_COUNT_PER_BLOCK,Align> pool;
public:

	//! Destructor
	virtual ~yonAllocatorAlign() {}

	//! Allocate memory for an array of objects
	T* allocate(size_t cnt)
	{
		//return (T*)internal_new(cnt* sizeof(T));
		//return NULL;
		return (T*)pool.allocate(sizeof(T),cnt);
	}

	//! Deallocate memory for an array of objects
	void deallocate(T* ptr)
	{
		//internal_delete(ptr);
		pool.deallocate(ptr);
	}

	//! Construct an element
	void construct(T* ptr, const T&e)
	{
		new ((void*)ptr) T(e);
	}

	//! Destruct an element
	void destruct(T* ptr)
	{
		ptr->~T();
	}
};

template<typename T,size_t ChunkSize,size_t Align>
alignmentpool<ChunkSize,yonAllocatorAlign<T,ChunkSize,Align>::CHUNK_COUNT_PER_BLOCK,Align> yonAllocatorAlign<T,ChunkSize,Align>::pool;
*/

template<typename T,size_t Align=core::AlignOf<T>::ALIGNMENT>
class yonAllocatorAlign
{
	static alignmentpoolgroup* poolgroup;
public:

	//! Destructor
	virtual ~yonAllocatorAlign() {}

	//! Allocate memory for an array of objects
	T* allocate(size_t cnt)
	{
		//return (T*)internal_new(cnt* sizeof(T));
		//return NULL;
		//return (T*)pool.allocate(sizeof(T),cnt);
		return (T*)poolgroup->allocate(Align,sizeof(T),cnt);
	}

	//! Deallocate memory for an array of objects
	void deallocate(T* ptr)
	{
		//internal_delete(ptr);
		poolgroup->deallocate(Align,ptr);
	}

	//! Construct an element
	void construct(T* ptr, const T&e)
	{
		new ((void*)ptr) T(e);
	}

	//! Destruct an element
	void destruct(T* ptr)
	{
		ptr->~T();
	}
};

template<typename T,size_t Align>
alignmentpoolgroup* yonAllocatorAlign<T,Align>::poolgroup=alignmentpoolgroup::GetInstance();

template<typename T>
class yonAllocatorMalloc
{
public:

	//! Destructor
	virtual ~yonAllocatorMalloc() {}

	//! Allocate memory for an array of objects
	T* allocate(size_t cnt)
	{
		return (T*)internal_new(cnt* sizeof(T));
	}

	//! Deallocate memory for an array of objects
	void deallocate(T* ptr)
	{
		internal_delete(ptr);
	}

	//! Construct an element
	void construct(T* ptr, const T&e)
	{
		new ((void*)ptr) T(e);
	}

	//! Destruct an element
	void destruct(T* ptr)
	{
		ptr->~T();
	}

protected:

	virtual void* internal_new(size_t cnt)
	{
		return malloc(cnt);
	}

	virtual void internal_delete(void* ptr)
	{
		free(ptr);
	}

};


//! Fast allocator, only to be used in containers inside the same memory heap.
/** Containers using it are NOT able to be used it across dll boundaries. Use this
when using in an internal class or function or when compiled into a static lib */
template<typename T>
class yonAllocatorFast
{
public:

	//! Allocate memory for an array of objects
	T* allocate(size_t cnt)
	{
		return (T*)operator new(cnt* sizeof(T));
	}

	//! Deallocate memory for an array of objects
	void deallocate(T* ptr)
	{
		operator delete(ptr);
	}

	//! Construct an element
	void construct(T* ptr, const T&e)
	{
		new ((void*)ptr) T(e);
	}

	//! Destruct an element
	void destruct(T* ptr)
	{
		ptr->~T();
	}
};


//! defines an allocation strategy
enum eAllocStrategy
{
	ALLOC_STRATEGY_SAFE    = 0,
	ALLOC_STRATEGY_DOUBLE  = 1,
	ALLOC_STRATEGY_SQRT    = 2
};

#ifdef YON_CHECK_MEMORY
#ifdef new
#undef new
#endif
#define new YON_DEBUG_NEW
#endif
} // end namespace core
} // end namespace yon

#endif

