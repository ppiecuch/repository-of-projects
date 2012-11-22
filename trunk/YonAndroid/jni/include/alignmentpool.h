#ifndef _YON_CORE_ALIGNMENTPOOL_H_
#define _YON_CORE_ALIGNMENTPOOL_H_

#include "yonMap.h"

namespace yon{
namespace core{

	template<size_t ChunkSize,size_t ChunkCountPerBlock,size_t Align>
	class alignmentpool{

		// compile-time check alignment is available.  
		typedef int IsValidAlignment[Align <= 128 && Align<=ChunkSize && ((Align & (Align-1)) == 0) ? +1 : -1];  

		struct chunk{
			chunk():Ptr(NULL),Next(NULL)
			{}

			u8* Ptr;
			chunk* Next;
		};

		struct block{
			block():Data(NULL),Chunks(NULL),Next(NULL){}

			u8* Data;
			chunk* Chunks;
			block* Next;
		};


		block* m_pBlockHead;
		chunk* m_pFreeHead;
		chunk* m_pUsedHead;
		const static u32 m_uRealChunkSize;

		core::map<u32,void*> m_noCaches;

		u32 m_uAlloc,m_uDelloc;
		u32 test;

	public:

		alignmentpool():m_pBlockHead(NULL),m_pFreeHead(NULL),m_pUsedHead(NULL),test(1),m_uAlloc(0),m_uDelloc(0){
			printf("m_uRealChunkSize:%d,ChunkCountPerBlock:%d,Align:%d\n",m_uRealChunkSize,ChunkCountPerBlock,Align);
		}
		virtual ~alignmentpool();

		//当count>1时，只保证首地址进行字节对齐
		void* allocate(u32 size,u32 count);
		void deallocate(void* p);
	};

	template<size_t ChunkSize,size_t ChunkCountPerBlock,size_t Align>
	const u32 alignmentpool<ChunkSize,ChunkCountPerBlock,Align>::m_uRealChunkSize=ChunkSize+Align;

	template<size_t ChunkSize,size_t ChunkCountPerBlock,size_t Align>
	alignmentpool<ChunkSize,ChunkCountPerBlock,Align>::~alignmentpool(){
		//printf("release:%08X,test:%u,Alloc:%u,Delloc:%u\n",this,test,m_uAlloc,m_uDelloc);
		block* bp=m_pBlockHead;
		while(bp){
			block* tmp=bp;
			bp=bp->Next;

			free(tmp->Data);
			delete[] tmp->Chunks;
			//printf("release block:%08X\n",tmp);
			delete tmp;
		}
		m_pBlockHead=NULL;
	}

	template<size_t ChunkSize,size_t ChunkCountPerBlock,size_t Align>
	void* alignmentpool<ChunkSize,ChunkCountPerBlock,Align>::allocate(u32 size,u32 count){
		//以最坏的情况估计空间（偏移量最多Align-1，外加上一个字节空间的offset）
		u32 total=size*count+Align;
		//printf("size:%u,count:%u\n",size,count);
		if(total>m_uRealChunkSize)
		{
			u8* p=(u8*)malloc(total);
			u32 addr=reinterpret_cast<u32>(p);
			u32 alignAddr=(addr+Align-1)&~(Align-1);
			m_noCaches[alignAddr]=p;
			//printf("WARN,no cache:%08X,%08X\n",addr,alignAddr);
			++m_uAlloc;
			return (void*)(p+(alignAddr-addr));
		}

		test<<=1;
		if(m_pFreeHead==NULL)
		{
			block* pb=new block;
			//printf("extend block:%08X\n",pb);
			pb->Data=(u8*)malloc(m_uRealChunkSize*ChunkCountPerBlock);

			pb->Chunks=new chunk[ChunkCountPerBlock];
			for(u32 i=0;i<ChunkCountPerBlock;++i)
			{
				pb->Chunks[i].Ptr=pb->Data+ChunkSize*i;
				//printf("%d:%08X\n",i,pb->Chunks[i].Ptr);
				pb->Chunks[i].Next=m_pFreeHead;
				m_pFreeHead=&pb->Chunks[i];
			}

			pb->Next=m_pBlockHead;
			m_pBlockHead=pb;
		}

		chunk* tmp=m_pFreeHead;
		m_pFreeHead=m_pFreeHead->Next;
		tmp->Next=m_pUsedHead;
		m_pUsedHead=tmp;

		u32 addr=reinterpret_cast<u32>(tmp->Ptr+1);
		u32 alignAddr=(addr+Align-1)&~(Align-1);
		u8 offset=(u8)(alignAddr-addr);
		u8* ptr=tmp->Ptr+1+offset;
		*((u8*)ptr-1)=offset;
		test-=1;
		++m_uAlloc;
		return (void*)ptr;
	}

	template<size_t ChunkSize,size_t ChunkCountPerBlock,size_t Align>
	void alignmentpool<ChunkSize,ChunkCountPerBlock,Align>::deallocate(void* p){
		if(p==NULL)
			return;

		void* realPointer;
		if(m_noCaches.remove(reinterpret_cast<u32>(p),&realPointer))
		{
			//printf("noCaches remove:%08X->%08X\n",p,realPointer);
			free(realPointer);
			++m_uDelloc;
			return;
		}

		chunk* tmp=m_pUsedHead;
		m_pUsedHead=m_pUsedHead->Next;
		u8* cp=(u8*)p;
		u8 offset=cp[-1];
		tmp->Ptr=cp-offset-1;
		//printf("recycle:%08X\n",tmp->Ptr);
		tmp->Next=m_pFreeHead;
		m_pFreeHead=tmp;
		++m_uDelloc;
	}
}
}
#endif