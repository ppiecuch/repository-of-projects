#include "lcAllocator.h"
#include "lcFixedMemoryPool.h"
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
			//�����MemoryTracer����֮�ŵ����ȥ�ͷ�
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
			LC_DEBG("Congratulation! No memory leak detected!%8s\r\n",LC_SYMBOL_SUCS);
	}

	/*
	const u32 FixedMemoryPool::s_uGrowth=LC_FIXED_MEMORY_POOL_GROWTH;

	const u32 FixedMemoryPool::s_uAlign=4;

	FixedMemoryPool::FixedMemoryPool():m_pStartFree(NULL),m_pEndFree(NULL),m_uCapacity(0),m_pBlock(NULL){
		::memset((void*)m_pFrees,0x0,sizeof(FixedNodePtr));
	}

	FixedMemoryPool::~FixedMemoryPool(){
		clear();
	}

	c8* FixedMemoryPool::allocateChunk(size_t unitSize,size_t& count){
		size_t szTotal=unitSize*count;
		// �ڴ��ʣ��ռ�
		size_t szLeft =  m_pEndFree - m_pStartFree;
		c8* pResult = NULL;

		// �ڴ��ʣ��ռ���������
		if( szLeft >= szTotal )
		{
			pResult = m_pStartFree;
			// �ƶ�ָ��ʣ��ռ��ָ��
			m_pStartFree += szTotal;
			return pResult;
		}
		else if( szLeft >= unitSize )
		{
			// �ڴ��ʣ��ռ䲻����ȫ���������������㹻һ�����ϵ����飬�ı�����Ĵ�С
			count = szLeft / unitSize;
			// �ƶ�ָ��ʣ��ռ��ָ��
			szTotal = unitSize * count;
			pResult = m_pStartFree;
			m_pStartFree += szTotal;
			return pResult;
		}
		else
		{
			// �ڴ��ʣ��ռ䲻��һ������
			// ��Ҫ��ȡ���ڴ�, ע���һ�η��䶼Ҫ������szTotal�Ĵ�С
			// ͬʱҪ����ԭ�е�m_uCapacity/4�Ķ���ֵ
			size_t szGet = 2 * szTotal + roundup(m_uCapacity >> 4);

			// ����Heap�ռ䣬���������ڴ��
			m_pStartFree = reinterpret_cast<c8*>(operator new(LC_ALLOC_ARGS_MT(szGet)));

			if(m_pStartFree)
			{
				m_uCapacity += szGet;
				m_pEndFree = m_pStartFree + szGet;

				// �洢�����ṩ�ͷ�
				BlockNode* node=LC_NEW BlockNode();
				node->Next=m_pBlock;
				node->Ptr=m_pStartFree;
				m_pBlock=node;

				// �ݹ���ã�����count
				return allocateChunk(unitSize, count);
			}

			count=0;
			return NULL;
		}
	}

	FixedNode* FixedMemoryPool::refill(size_t size){
		// ȱʡΪ__NUM_NODE��������,����ڴ�ռ䲻�㣬��õ����������С��20
		size_t count = s_uGrowth;

		// ����allocateChunk,����ȡ��count������Ϊfrees��������
		// count����Pass By reference����
		c8 *pChunk = allocateChunk(size, count);

		// ���ֻ���һ������,���������ͷ���������ߣ�frees��������
		if( 1 == count )
			return reinterpret_cast<FixedNode*>(pChunk);

		// �������frees��ע��������
		FixedNodePtr *pFree  = m_pFrees;

		// ��Chunk�ռ��ڽ���frees
		// pResult׼�����ظ��ͻ���
		FixedNode* pResult = reinterpret_cast<FixedNode*>(pChunk);

		FixedNode *pCurNode = NULL, *pNextNode = NULL;

		//TODO ����for��ȥ
		// ������һ����λ���ڴ棬����һ������
		--count;
		// ��Ҫ����һ����λ���ڴ棬����һ����λ��ʼ��ʣ���FixedNode��������, ����freesָ�������ÿռ�
		*pFree = pNextNode = reinterpret_cast<FixedNode*>(pChunk + size);

		// ��frees�ĸ����鴮������
		// ��1��ʼ,��0������
		for(size_t i = 1; ; ++i)
		{
			pCurNode = pNextNode;
			//SOLVED �Ƿ���(pNextNode+1)��ͬ ==>����,FixedNode��size�޹�
			pNextNode = reinterpret_cast<FixedNode*>(reinterpret_cast<c8*>(pNextNode) + size);

			if( count == i )
			{
				// �������, ��һ���ڵ�ΪNULL, �˳�ѭ��
				pCurNode->Node = NULL;
				break;
			}
			else
			{
				pCurNode->Node = pNextNode;
			}
		}

		return pResult;
	}

	void FixedMemoryPool::clear(){
		while(m_pBlock)
		{
			BlockNode* next=m_pBlock->Next;
			delete m_pBlock->Ptr;
			delete m_pBlock;
			m_pBlock=next;
		}
	}

	void* FixedMemoryPool::allocate(size_t n){
		LC_DEBUG_BREAK_IF(n!=size);

		FixedNode* pResult = NULL;

		m_lock.lock();

		// Ѱ��frees���ʵ���һ��
		FixedNodePtr* pFree = m_pFrees;
		pResult = *pFree;

		if( pResult == NULL )
		{	
			// ���û���ҵ����õ�������
			pResult = refill(ROUND);
		}
		else
		{
			// ����free����,ʹ��ָ����һ��List�Ľڵ㣬���������ʱ��ͷ���ΪNULL
			*pFree = pResult->Node;
		}

		m_lock.unlock();

		return pResult;
	}

	void FixedMemoryPool::deallocate(void *p, size_t n){
		LC_DEBUG_BREAK_IF(n!=size);

		// �õ��ڴ�ص�ַ
		FixedNode* pTemp = reinterpret_cast<FixedNode*>(p);
		
		m_lock.lock();
		FixedNodePtr* pFree = m_pFrees;
		// ���ա��ı�Nextָ�룬�����صĽڵ����List��ͷ
		pTemp->Node = *pFree;
		*pFree = pTemp;
		m_lock.unlock();
	}

	*/

	void* PrimitiveAllocateTrait::allocate(size_t size){
		LC_DEBG("PrimitiveAllocateTrait::allocate1\r\n");
		return operator new(size);
	}
	void* PrimitiveAllocateTrait::allocate(size_t size,const c8* file,const c8* func,s32 line){
		LC_DEBG("PrimitiveAllocateTrait::allocate2\r\n");
		return operator new(LC_ALLOC_ARGS_SL(size));
	}
	void PrimitiveAllocateTrait::deallocate(void *p, size_t size){
		operator delete(p);
	}

	STFixedMemoryPool fmp;
	void* FixedSizeAllocateTrait::allocate(size_t size){
		LC_DEBG("FixedSizeAllocateTrait::allocate1\r\n");
		return operator new(size);
	}
	void* FixedSizeAllocateTrait::allocate(size_t size,const c8* file,const c8* func,s32 line){
		return fmp.allocate(size);
		//LC_DEBG("FixedSizeAllocateTrait::allocate2\r\n");
		//return operator new(LC_ALLOC_ARGS_SL(size));
	}
	void FixedSizeAllocateTrait::deallocate(void *p, size_t size){
		fmp.deallocate(p,size);
		//operator delete(p);
	}

}
