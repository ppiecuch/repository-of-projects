#include "lcAllocator.h"
#include "lcLogger.h"



namespace lc{

	template<> MemoryTracer* core::Singleton<MemoryTracer>::s_pSingleton = NULL;

	mutex MemoryTracer::s_mutex;

	MemoryTracer::MemoryTracer():m_pHead(NULL),m_pTail(NULL),m_uAllocatedSize(0),m_uAllocatedCount(0){
		setMaxSize(LC_MEMORY_MAX_SIZE);
	}

	void MemoryTracer::lock(){
		s_mutex.lock();
	}

	void MemoryTracer::unlock(){
		s_mutex.unlock();
	}

	bool MemoryTracer::canAllocate(size_t sz)
	{
		if(m_uAllocatedSize+sz>m_uMaxSize)
			return false;
		return true;
	}

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
		removeNode(node);
		free(node);
	}

	void MemoryTracer::allocate(LC_ALLOC_PARAMS(void* p,size_t sz))
	{
		if(canAllocate(sz))
		{
			TracerNode* n=(TracerNode*)malloc(sizeof(TracerNode));
			TracerNode tmp(LC_ALLOC_ARGS_SL(p,sz));
			new (n) TracerNode(tmp);
			addNode(n);
		}
		else
		{
			lcThrow(OverflowException);
		}
	} 

	void MemoryTracer::deallocate(void* ptr)
	{
		TracerNode* n=m_pHead;
		while(n)
		{
			if(n->Addr==ptr)break;
			n=n->Next;
		}
		//LC_DEBUG_BREAK_IF(n==NULL);
		if(n)
			removeNode(n);
		free(n);
	}

	void MemoryTracer::dump()
	{

	}

	void MemoryTracer::destroy()
	{
		lock();
		bool hasLeak=false;
		TracerNode* n=m_pHead;
		if(m_pHead)
		{
			LC_WARN("Detected memory leak: %u bytes,%u calls in total%8s\r\n",m_uAllocatedSize,m_uAllocatedCount,LC_SYMBOL_WARN);
		}
		TracerNode* thiz=NULL;
		while(n)
		{
			TracerNode* next=n->Next;
			LC_WARN("Detected memory leak at 0x%08X(%u bytes) in %s within \"%s(...)\" at line:%d%8s\r\n",n->Addr,n->Size,n->File,n->Func,n->Line,LC_SYMBOL_WARN);
			//如果是MemoryTracer，将之放到最后去释放
			if(n->Addr==this)
				delete n->Addr;
			else
				thiz=n;
			n=next;
			hasLeak = true;
		}
		if(thiz)
			delete thiz;
		unlock();
		core::Singleton<MemoryTracer>::destroy();
		if(!hasLeak)
			LC_INFO("Congratulation! No memory leak detected!%8s\r\n",LC_SYMBOL_SUCS);
	}

}
