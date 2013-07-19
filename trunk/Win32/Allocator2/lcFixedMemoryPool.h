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

	//ROUND_UP ��bytes�ϵ���__ALIGN�ı���
	enum { ROUND = (size + s_uAlign - 1) & ~(s_uAlign - 1) };

	LockType m_lock;

	// �ڴ����ʼλ��
	c8 *m_pStartFree;
	// �ڴ�ؽ���λ��
	c8 *m_pEndFree;
	// ������Ŀռ��С
	u32 m_uCapacity;

	// free����
	FixedNodePtr m_pFrees[1];

	// block����
	BlockNode* m_pBlock;

	// roundup ��bytes�ϵ���s_uAlign�ı���
	static inline u32 roundup(u32 bytes)
	{
		return ((bytes) + s_uAlign - 1) & ~(s_uAlign - 1);
	}

	// !����һ���ռ�,������count����СΪunitSize������
	// ��Щ�ڴ��������ַ��������һ���, ������ָ��
	c8* allocateChunk(size_t unitSize,size_t& count){
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

	// ����һ����СΪn�Ķ���,�������СΪsize���������鵽free������
	FixedNode* refill(size_t cnt){
		// ȱʡΪ__NUM_NODE��������,����ڴ�ռ䲻�㣬��õ����������С��20
		size_t count = s_uGrowth;

		// ����allocateChunk,����ȡ��count������Ϊfrees��������
		// count����Pass By reference����
		c8 *pChunk = allocateChunk(cnt, count);

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
		//--count;
		// ��Ҫ����һ����λ���ڴ棬����һ����λ��ʼ��ʣ���FixedNode��������, ����freesָ�������ÿռ�
		*pFree = pNextNode = reinterpret_cast<FixedNode*>(pChunk + cnt);

		// ��frees�ĸ����鴮������
		// ��1��ʼ,��0������
		//for(size_t i = 1; ; ++i)
		for(size_t i=1;i<count;++i)
		{
			pCurNode = pNextNode;
			//SOLVED �Ƿ���(pNextNode+1)��ͬ ==>����,FixedNode��size�޹�
			pNextNode = reinterpret_cast<FixedNode*>(reinterpret_cast<c8*>(pNextNode) + cnt);

			//if( count == i )
			if( i == count-1 )
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

	// ����ڴ�
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
	 * @brief n�������0
	 */
	void* allocate(size_t n){
		LC_DEBUG_BREAK_IF(n>size);

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

	// p����Ϊ��
	void deallocate(void *p, size_t n){
		LC_DEBUG_BREAK_IF(n>size);

		// �õ��ڴ�ص�ַ
		FixedNode* pTemp = reinterpret_cast<FixedNode*>(p);

		m_lock.lock();
		FixedNodePtr* pFree = m_pFrees;
		// ���ա��ı�Nextָ�룬�����صĽڵ����List��ͷ
		pTemp->Node = *pFree;
		*pFree = pTemp;
		m_lock.unlock();
	}
};

typedef FixedMemoryPool<true,128> MTFixedMemoryPool;
typedef FixedMemoryPool<false,128> STFixedMemoryPool;

}
#endif