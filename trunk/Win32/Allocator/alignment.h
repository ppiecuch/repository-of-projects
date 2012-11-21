#ifndef _YON_CORE_ALIGNMENT_H_
#define _YON_CORE_ALIGNMENT_H_

namespace yon{
namespace core{

	template<size_t ChunkSize,size_t ChunkCountPerBlock,size_t Align>
	class alignment{

		// compile-time check alignment is available.  
		typedef int IsValidAlignment[Align <= 128 && Align<=ChunkSize && ((Align & (Align-1)) == 0) ? +1 : -1];  

		struct chunk{
			c8* Ptr;
			u32 Size;
			u32 Count;
			chunk* Next;
		};

		struct block{
			block():Data(NULL),Chunks(NULL),Next(NULL){}

			c8* Data;
			chunk* Chunks;
			block* Next;
		};


		u8 m_uOffset;
		block* m_pBlockHead;
		chunk* m_pFreeHead;
		chunk* m_pUsedHead;

	public:

		alignment();
		virtual ~alignment();

		//分配成功，返回空间的指针，否则返回NULL
		void* allocate(u32 size,u32 count);
		void deallocate(void* p);
	};

	template<size_t ChunkSize,size_t ChunkCountPerBlock,size_t Align>
	void* alignment<ChunkSize,ChunkCountPerBlock,Align>::allocate(u32 size,u32 count){
		u32 total=size*count;
		if(total>ChunkSize)
			return NULL;

		if(m_pFreeHead==NULL)
		{
			//TODO ChunkSize size Align offset之间的关系
			block* pb=new block;
			pb->Data=(c8*)malloc(ChunkSize*ChunkCountPerBlock);

			pb->Chunks=new chunk[ChunkCountPerBlock];
			for(u32 i=0;i<ChunkCountPerBlock;++i)
			{
				pb->Chunks[i].Ptr=pb->Data+ChunkSize*i;

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

		return (void*)tmp->Ptr;
	}
}
}
#endif