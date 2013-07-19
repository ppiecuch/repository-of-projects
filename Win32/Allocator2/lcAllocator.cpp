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
		// 内存池剩余空间
		size_t szLeft =  m_pEndFree - m_pStartFree;
		c8* pResult = NULL;

		// 内存池剩余空间满足需求
		if( szLeft >= szTotal )
		{
			pResult = m_pStartFree;
			// 移动指向剩余空间的指针
			m_pStartFree += szTotal;
			return pResult;
		}
		else if( szLeft >= unitSize )
		{
			// 内存池剩余空间不能完全满足需求量，但足够一个以上的区块，改变申请的大小
			count = szLeft / unitSize;
			// 移动指向剩余空间的指针
			szTotal = unitSize * count;
			pResult = m_pStartFree;
			m_pStartFree += szTotal;
			return pResult;
		}
		else
		{
			// 内存池剩余空间不够一个区块
			// 需要获取的内存, 注意第一次分配都要两倍于szTotal的大小
			// 同时要加上原有的m_uCapacity/4的对齐值
			size_t szGet = 2 * szTotal + roundup(m_uCapacity >> 4);

			// 配置Heap空间，用来补充内存池
			m_pStartFree = reinterpret_cast<c8*>(operator new(LC_ALLOC_ARGS_MT(szGet)));

			if(m_pStartFree)
			{
				m_uCapacity += szGet;
				m_pEndFree = m_pStartFree + szGet;

				// 存储起来提供释放
				BlockNode* node=LC_NEW BlockNode();
				node->Next=m_pBlock;
				node->Ptr=m_pStartFree;
				m_pBlock=node;

				// 递归调用，修正count
				return allocateChunk(unitSize, count);
			}

			count=0;
			return NULL;
		}
	}

	FixedNode* FixedMemoryPool::refill(size_t size){
		// 缺省为__NUM_NODE个新区块,如果内存空间不足，获得的区块书可能小于20
		size_t count = s_uGrowth;

		// 调用allocateChunk,尝试取得count个区块为frees的新区块
		// count是以Pass By reference传递
		c8 *pChunk = allocateChunk(size, count);

		// 如果只获得一个区块,则这个区块就分配给调用者，frees无新区块
		if( 1 == count )
			return reinterpret_cast<FixedNode*>(pChunk);

		// 否则调整frees，注入新区块
		FixedNodePtr *pFree  = m_pFrees;

		// 在Chunk空间内建立frees
		// pResult准备返回给客户端
		FixedNode* pResult = reinterpret_cast<FixedNode*>(pChunk);

		FixedNode *pCurNode = NULL, *pNextNode = NULL;

		//TODO 整到for中去
		// 请求了一个单位的内存，减少一个计数
		--count;
		// 需要返回一个单位的内存，从下一个单位开始将剩余的FixedNode连接起来, 导引frees指向新配置空间
		*pFree = pNextNode = reinterpret_cast<FixedNode*>(pChunk + size);

		// 将frees的各区块串接起来
		// 从1开始,第0个返回
		for(size_t i = 1; ; ++i)
		{
			pCurNode = pNextNode;
			//SOLVED 是否与(pNextNode+1)等同 ==>不等,FixedNode与size无关
			pNextNode = reinterpret_cast<FixedNode*>(reinterpret_cast<c8*>(pNextNode) + size);

			if( count == i )
			{
				// 分配完毕, 下一个节点为NULL, 退出循环
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

		// 寻找frees中适当的一个
		FixedNodePtr* pFree = m_pFrees;
		pResult = *pFree;

		if( pResult == NULL )
		{	
			// 如果没有找到可用的则申请
			pResult = refill(ROUND);
		}
		else
		{
			// 调整free数组,使其指向下一个List的节点，当分配完毕时，头结点为NULL
			*pFree = pResult->Node;
		}

		m_lock.unlock();

		return pResult;
	}

	void FixedMemoryPool::deallocate(void *p, size_t n){
		LC_DEBUG_BREAK_IF(n!=size);

		// 得到内存池地址
		FixedNode* pTemp = reinterpret_cast<FixedNode*>(p);
		
		m_lock.lock();
		FixedNodePtr* pFree = m_pFrees;
		// 回收。改变Next指针，将返回的节点放在List开头
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
