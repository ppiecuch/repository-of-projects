#include "lcAllocator.h"
#include <stdlib.h>

void* operator new(LC_ALLOC_PARAMS(size_t size)){
	//void* operator new(size_t size, void* ptr, void*){
	//void* operator new(size_t size){
	return malloc(size);
}

//void operator delete(LC_ALLOC_PARAMS(void* ptr)){
void operator delete(void* ptr){
	free(ptr);
}

namespace lc{

	

	BaseAllocator::BaseAllocator():m_uAllocatedSize(0),m_uMaxSize(LC_MEMORY_MAX_SIZE),m_pOutOfMemHandler(NULL){}

	BaseAllocator::~BaseAllocator(){}

	void BaseAllocator::destroy(){
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

	void BaseAllocator::setOutOfMemHandler(pOutOfMemHandler pFunc)
	{
		pOutOfMemHandler pOldFunc = m_pOutOfMemHandler;
		m_pOutOfMemHandler = pFunc;
	}

	void BaseAllocator::setMaxSize(u32 size)
	{
		m_uMaxSize=size;
	}

	u32 BaseAllocator::getMaxSize()
	{
		return m_uMaxSize;
	}

	void BaseAllocator::setAllocatedSize(u32 size)
	{
		m_uAllocatedSize=size;
	}

	u32 BaseAllocator::getAllocatedSize()
	{
		return m_uAllocatedSize;
	}
}
