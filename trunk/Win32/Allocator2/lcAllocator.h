#ifndef _LUCID_ALLOCATOR_H_
#define _LUCID_ALLOCATOR_H_

#include "LCDefine.h"

//refer to:SGIMallocPool.hhp

namespace lc{
	
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

	static void deallocate(void *p, size_t/* sz*/)
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

}

#endif