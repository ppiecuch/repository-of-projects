#ifndef _YON_CORE_MEMORYPOOL_H_
#define _YON_CORE_MEMORYPOOL_H_

#include "yonTypes.h"
#include <stdlib.h>

namespace yon{
namespace core{

	template<size_t Size>
	class memorypool{
		struct chunk{
			chunk* Next;
		};
		struct block{
			c8* First;
			c8* Current;
			block* Next;
		};
		const static size_t BLOCK_SIZE;
		const static size_t CHUNK_SIZE;
		u32 m_uChunkSize;
		u32 m_uChunkCountPerBlock;
		block* m_pBlockHead;
		chunk* m_pChunkHead;
	public:
		memorypool();
		virtual ~memorypool();

		//分配成功，返回空间的指针，否则返回NULL
		void* allocate(u32 size,u32 count);
		void deallocate(void* p);
	};

	template<size_t Size>
	const size_t memorypool<Size>::BLOCK_SIZE=sizeof(block);

	template<size_t Size>
	const size_t memorypool<Size>::CHUNK_SIZE=sizeof(chunk);

	template<size_t Size>
	memorypool<Size>::memorypool()
		:m_uChunkCountPerBlock(16),m_pBlockHead(NULL),m_pChunkHead(NULL),m_uChunkSize(Size)
	{}

	template<size_t Size>
	memorypool<Size>::~memorypool(){
		block* pBlock;
		while(m_pBlockHead)
		{
			pBlock=m_pBlockHead;
			m_pBlockHead=m_pBlockHead->Next;
			free(pBlock);
		}
		m_pChunkHead=NULL;
	}

	template<size_t Size>
	void* memorypool<Size>::allocate(u32 size,u32 count){
		u32 total=size*count;
		YON_DEBUG_BREAK_IF(total>m_uChunkSize);
		if(total>m_uChunkSize)
			return NULL;
		chunk* result=NULL;
		if(m_pChunkHead!=NULL)
		{
			chunk* p=m_pChunkHead;
			m_pChunkHead=m_pChunkHead->Next;
			result=++p;
		}
		else if(m_pBlockHead==NULL||m_pBlockHead->Current==m_pBlockHead->First+(CHUNK_SIZE+m_uChunkSize)*m_uChunkCountPerBlock)
		{
			m_uChunkCountPerBlock<<=1;
			block* pBlock=(block*)malloc(BLOCK_SIZE+(CHUNK_SIZE+m_uChunkSize)*m_uChunkCountPerBlock);
			if(!pBlock)
				return NULL;
			pBlock->Current=pBlock->First=(c8*)pBlock+BLOCK_SIZE;
			pBlock->Next=m_pBlockHead;
			m_pBlockHead=pBlock;
		}
		if(m_pBlockHead->Current<m_pBlockHead->First+(CHUNK_SIZE+m_uChunkSize)*m_uChunkCountPerBlock)
		{
			chunk* p=(chunk*)m_pBlockHead->Current;
			m_pBlockHead->Current+=CHUNK_SIZE+m_uChunkSize;
			result=++p;
		}
		//printf("memorypool<%d> allocate:%u->%08x\r\n",Size,size,result);
		return result;
	}

	template<size_t Size>
	void memorypool<Size>::deallocate(void* p){
		//printf("memorypool<%d> deallocate:%08x\r\n",Size,p);
		chunk* pChunk=(chunk*)((c8*)p-CHUNK_SIZE);
		pChunk->Next=m_pChunkHead;
		m_pChunkHead=pChunk;
	}
}
}
#endif