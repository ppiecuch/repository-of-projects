#ifndef _LUCID_FIXEDMEMORYPOOL_H_
#define _LUCID_FIXEDMEMORYPOOL_H_

#include "lcTypes.h"
#include "lcUncopyable.h"

namespace lc{

template<bool isMT,size_t size>
class FixedMemoryPool : public Uncopyable{
private:
	struct BlockNode{
		BlockNode():Next(NULL),Ptr(NULL){}
		BlockNode* Next;
		void* Ptr;
	};

	union FixedNode
	{
		union FixedNode* Node;
		c8 Data[1];
	};
	typedef typename VolatileTrait<FixedNode, isMT>::ValueType	FixedNodePtr;
	typedef typename LockTrait<isMT>::ValueType LockType;

	static const u32 s_uGrowth=LC_FIXED_MEMORY_POOL_GROWTH;
	static const u32 s_uAlign=4;

	//ROUND_UP 将bytes上调至__ALIGN的倍数
	enum { ROUND = (size + s_uAlign - 1) & ~(s_uAlign - 1) };

	LockType m_lock;

	// 内存池起始位置
	c8 *m_pStartFree;
	// 内存池结束位置
	c8 *m_pEndFree;
	// 所分配的空间大小
	u32 m_uCapacity;

	// free数组
	FixedNodePtr m_pFrees[1];

	// block链表
	BlockNode* m_pBlock;

	// roundup 将bytes上调至s_uAlign的倍数
	static inline u32 roundup(u32 bytes)
	{
		return ((bytes) + s_uAlign - 1) & ~(s_uAlign - 1);
	}

	// !配置一块大空间,可容纳count个大小为unitSize的区块
	// 这些内存在物理地址上是连在一起的, 返回其指针
	c8* allocateChunk(size_t unitSize,size_t& count){
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

	// 返回一个大小为n的对象,并加入大小为size的其他区块到free队列中
	FixedNode* refill(size_t cnt){
		// 缺省为__NUM_NODE个新区块,如果内存空间不足，获得的区块书可能小于20
		size_t count = s_uGrowth;

		// 调用allocateChunk,尝试取得count个区块为frees的新区块
		// count是以Pass By reference传递
		c8 *pChunk = allocateChunk(cnt, count);

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
		//--count;
		// 需要返回一个单位的内存，从下一个单位开始将剩余的FixedNode连接起来, 导引frees指向新配置空间
		*pFree = pNextNode = reinterpret_cast<FixedNode*>(pChunk + cnt);

		// 将frees的各区块串接起来
		// 从1开始,第0个返回
		//for(size_t i = 1; ; ++i)
		for(size_t i=1;i<count;++i)
		{
			pCurNode = pNextNode;
			//SOLVED 是否与(pNextNode+1)等同 ==>不等,FixedNode与size无关
			pNextNode = reinterpret_cast<FixedNode*>(reinterpret_cast<c8*>(pNextNode) + cnt);

			//if( count == i )
			if( i == count-1 )
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

	// 清空内存
	void clear(){
		while(m_pBlock)
		{
			BlockNode* next=m_pBlock->Next;
			delete m_pBlock->Ptr;
			delete m_pBlock;
			m_pBlock=next;
		}
	}
public:
	FixedMemoryPool():m_pStartFree(NULL),m_pEndFree(NULL),m_uCapacity(0),m_pBlock(NULL){
		::memset((void*)m_pFrees,0x0,sizeof(FixedNodePtr));
	}
	~FixedMemoryPool(){
		clear();
	}

	/**
	 * @brief n必须大于0
	 */
	void* allocate(size_t n){
		LC_DEBUG_BREAK_IF(n>size);

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

	// p不能为空
	void deallocate(void *p, size_t n){
		LC_DEBUG_BREAK_IF(n>size);

		// 得到内存池地址
		FixedNode* pTemp = reinterpret_cast<FixedNode*>(p);

		m_lock.lock();
		FixedNodePtr* pFree = m_pFrees;
		// 回收。改变Next指针，将返回的节点放在List开头
		pTemp->Node = *pFree;
		*pFree = pTemp;
		m_lock.unlock();
	}
};

typedef FixedMemoryPool<true,128> MTFixedMemoryPool;
typedef FixedMemoryPool<false,128> STFixedMemoryPool;

}
#endif