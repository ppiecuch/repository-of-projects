#include "lcAllocator.h"
#include <stdlib.h>



namespace lc{

	template<> MemoryTracer* core::Singleton<MemoryTracer>::s_pSingleton = NULL;

	MemoryTracer::MemoryTracer():m_pHead(NULL),m_pTail(NULL),m_uAllocatedSize(0),m_uAllocatedCount(0){}

	void MemoryTracer::addNode(TracerNode* node)
	{
		if(m_pTail)
		{
			node->Prev = m_pTail;
			node->Next = NULL;
			m_pTail->Next = node;
			m_pTail = node;
		}
		else
		{
			m_pHead = node;
			node->Prev = NULL;
			node->Next = NULL;
			m_pTail = node;
		}
		m_uAllocatedSize += node->Size;
		++m_uAllocatedCount;
	}

	void MemoryTracer::removeNode(TracerNode* node)
	{
		TracerNode* next = node->Next;
		TracerNode* prev = node->Prev;
		if(prev)
		{
			if(next)
			{
				prev->Next = next;
				next->Prev = prev;
			}
			else
			{
				prev->Next = NULL;
				m_pTail = prev;
			}
		}
		else
		{
			if (next)
			{
				m_pHead = next;
				next->Prev = NULL;
			}
			else
			{
				m_pHead = NULL;
				m_pTail = NULL;
			}
		}
		m_uAllocatedSize -= node->Size;
		--m_uAllocatedCount;
	}

	void MemoryTracer::deallocateNode(TracerNode* node)
	{
		m_mutex.lock();
		removeNode(node);
		m_mutex.unlock();
		free(node);
	}

	void MemoryTracer::allocate(LC_ALLOC_PARAMS(void* p,size_t sz))
	{
		TracerNode* n=(TracerNode*)malloc(sizeof(TracerNode));
		TracerNode tmp(LC_ALLOC_ARGS_SL(p,sz));
		new (n) TracerNode(tmp);
		m_mutex.lock();
		addNode(n);
		m_mutex.unlock();
	}

	void MemoryTracer::deallocate(void* ptr)
	{
		m_mutex.lock();
		TracerNode* n=m_pHead;
		while(n)
		{
			if(n->Addr==ptr)break;
			n=n->Next;
		}
		//LC_DEBUG_BREAK_IF(n==NULL);
		if(n)
			removeNode(n);
		m_mutex.unlock();
		free(n);
	}

	void MemoryTracer::dump()
	{

	}

	void MemoryTracer::destroy()
	{
		m_mutex.lock();
		TracerNode* n=m_pHead;
		while(n)
		{
			TracerNode* next=n->Next;
			TRACE("Check memory leak at 0x%08X(%u bytes) in %s within \"%s(...)\" at line:%d\r\n",n->Addr,n->Size,n->File,n->Func,n->Line);
			delete n->Addr;
			n=next;
		}
		m_mutex.unlock();
		core::Singleton<MemoryTracer>::destroy();
	}

	
	/*

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

	*/
}
