
#ifndef _YON_CORE_YONALLOCATOR_H_
#define _YON_CORE_YONALLOCATOR_H_

#include "yonTypes.h"
#include <new>
#include <memory.h>

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

