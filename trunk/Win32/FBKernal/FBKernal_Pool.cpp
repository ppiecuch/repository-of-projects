#include "FBKernal_Pool.h"

CFBKernal_Pool::CFBKernal_Pool(DWORD dwChunkSize)
{
	m_dwChunkSize = dwChunkSize;
	m_dwChunksPerBlock = 16;

	m_pBlockHead = 0;

	#ifndef __DEBUG
	m_pNChunkHead = 0;
	#endif
}
CFBKernal_Pool::~CFBKernal_Pool()
{
	#ifdef __DEBUG
	Report();
	#endif

	if(m_dwChunkSize == 0)
	{
		#ifdef __DEBUG
		CFBKernal_Pool_Chunk* pChunk;
		while(pChunk = m_AChunkList.GetHead())
		{
			m_AChunkList.Pop(pChunk);
			free(pChunk);
		}
		#endif
	}
	else
	{
		CFBKernal_Pool_Block* pNext;
		while(m_pBlockHead)
		{
			/*CFBKernal_Pool_Chunk* front=(CFBKernal_Pool_Chunk*)m_pBlockHead->m_pPtr_Cur;
			while(front){
				CFBKernal_Pool_Chunk* tmp=front;
				front=front->m_pNext;
				free(tmp);
			}*/
			pNext = m_pBlockHead->m_pNext;
			free(m_pBlockHead);
			m_pBlockHead = pNext;
		}

		#ifdef __DEBUG
		m_AChunkList.Pop_All();
		m_NChunkList.Pop_All();
		#else
		/*while(m_pNChunkHead)
		{
			CFBKernal_Pool_Chunk* p=m_pNChunkHead;
			m_pNChunkHead=m_pNChunkHead->m_pNext;
			free(p);
		}*/
		m_pNChunkHead = 0;
		#endif
	}
}

void* CFBKernal_Pool::Malloc(DWORD dwSize, DWORD dwCount, BOOL bDump)
{
	if(m_dwChunkSize == 0)
	{
		CFBKernal_Pool_Chunk* pChunk = (CFBKernal_Pool_Chunk*)malloc(sizeof(CFBKernal_Pool_Chunk) + dwSize * dwCount);

		#ifdef __DEBUG
		pChunk->m_pPrev = 0;
		pChunk->m_dwSize = dwSize;
		#endif 

		pChunk->m_dwCount = dwCount;
		pChunk->m_pPool = this;

		// debug info
		#ifdef __DEBUG
		FBKernal_Pool_Construct(&pChunk->m_CallStack);
		if(bDump)
		{
			FBKERNAL_CALLSTACK_DUMP(pChunk->m_CallStack);
		}

		// push to achunklist
		m_AChunkList.Push_Back(pChunk);
		#endif

		return ++pChunk;
	}
	else
	{
		DWORD dwTotal = dwSize * dwCount;
		if(dwTotal > m_dwChunkSize)
		{
			return 0;
		}

		// 是否最后一个block和nchunklist都为空
		if(
			#ifdef __DEBUG
			(m_NChunkList.Size() == 0) &&
			#else
			(m_pNChunkHead == 0) &&
			#endif
			((!m_pBlockHead) || (m_pBlockHead->m_pPtr_Cur == m_pBlockHead->m_pPtr + (sizeof(CFBKernal_Pool_Chunk) + m_dwChunkSize) * m_dwChunksPerBlock)))
		{
			// 申请新的block
			m_dwChunksPerBlock *= 2;

			CFBKernal_Pool_Block* pBlock = (CFBKernal_Pool_Block*)malloc(
				sizeof(CFBKernal_Pool_Block) +
				(sizeof(CFBKernal_Pool_Chunk) + m_dwChunkSize) * m_dwChunksPerBlock);
			if(!pBlock)
			{
				return 0;
			}

			pBlock->m_pPtr = pBlock->m_pPtr_Cur = (char*)pBlock + sizeof(CFBKernal_Pool_Block);

			pBlock->m_pNext = m_pBlockHead;
			m_pBlockHead = pBlock;
		}

		// 首先去最后一个block中找是否有可用的
		if(m_pBlockHead->m_pPtr_Cur < m_pBlockHead->m_pPtr + (sizeof(CFBKernal_Pool_Chunk) + m_dwChunkSize) * m_dwChunksPerBlock)
		{
			CFBKernal_Pool_Chunk* pChunk = (CFBKernal_Pool_Chunk*)(m_pBlockHead->m_pPtr_Cur);

			#ifdef __DEBUG
			pChunk->m_pPrev = 0;
			#endif

			m_pBlockHead->m_pPtr_Cur += sizeof(CFBKernal_Pool_Chunk) + m_dwChunkSize;

			#ifdef __DEBUG
			pChunk->m_dwSize = dwSize;
			#endif

			pChunk->m_dwCount = dwCount;
			pChunk->m_pPool = this;

			// debug info
			#ifdef __DEBUG
			FBKernal_Pool_Construct(&pChunk->m_CallStack);
			if(bDump)
			{
				FBKERNAL_CALLSTACK_DUMP(pChunk->m_CallStack);
			}

			// push to achunklist
			m_AChunkList.Push_Back(pChunk);
			#endif

			return ++pChunk;
		}
		else
		{
			// nchunklist中肯定有可以使用的
			#ifdef __DEBUG
			CFBKernal_Pool_Chunk* pChunk = m_NChunkList.GetTail();

			// pop from nchunklist
			m_NChunkList.Pop(pChunk);
			#else
			CFBKernal_Pool_Chunk* pChunk = m_pNChunkHead;
			m_pNChunkHead = m_pNChunkHead->m_pNext;
			#endif
			
			#ifdef __DEBUG
			pChunk->m_dwSize = dwSize;
			#endif

			pChunk->m_dwCount = dwCount;

			// debug info
			#ifdef __DEBUG
			if(bDump)
			{
				FBKERNAL_CALLSTACK_DUMP(pChunk->m_CallStack);
			}

			// push to achunklist
			m_AChunkList.Push_Back(pChunk);
			#endif

			return ++pChunk;
		}
	}
	return 0;
}
void CFBKernal_Pool::Free(void* pPtr)
{
	if(!pPtr)
	{
		return;
	}

	if(m_dwChunkSize == 0)
	{
		CFBKernal_Pool_Chunk* pChunk = (CFBKernal_Pool_Chunk*)((char*)pPtr - sizeof(CFBKernal_Pool_Chunk));

		#ifdef __DEBUG
		m_AChunkList.Pop(pChunk);
		//if(m_AChunkList.GetSize() < 0)
		//{
		//	int a = 10;
		//}
		#endif

		free(pChunk);
	}
	else
	{
		CFBKernal_Pool_Chunk* pChunk = (CFBKernal_Pool_Chunk*)((char*)pPtr - sizeof(CFBKernal_Pool_Chunk));

		#ifdef __DEBUG
		m_AChunkList.Pop(pChunk);
		//if(m_AChunkList.GetSize() < 0)
		//{
		//	int a = 10;
		//}
		m_NChunkList.Push_Back(pChunk);
		#else
		pChunk->m_pNext = m_pNChunkHead;
		m_pNChunkHead = pChunk;
		#endif
	}
}

void CFBKernal_Pool::Report(void)
{
	#ifdef __DEBUG
	if(m_AChunkList.Size() == 0)
	{
		return;
	}

	OutputDebugString("\n================================ FlyBeyond Pool Report (start) ================================\n");

	char szMsg[FBKERNAL_STRING_LARGE];
	CFBKernal_Pool_Chunk* pChunk = m_AChunkList.GetHead();

	int nAdd = 0;
	while(pChunk)
	{
		if(pChunk != m_AChunkList.GetHead())
		{
			OutputDebugString("\n");
		}

		sprintf(
			szMsg, 
			"-- [%d] memory leak - size: %d bytes, count: %d, total: %d bytes\n", 
			nAdd++, 
			pChunk->m_dwSize, 
			pChunk->m_dwCount,
			pChunk->m_dwSize * pChunk->m_dwCount);
		OutputDebugString(szMsg);

		CFBKernal_CallStack_Node* pNode = pChunk->m_CallStack.m_Stack->GetHead();
		while(pNode)
		{
			sprintf(szMsg, "%s(%d) : %s\n", pNode->m_pFile, pNode->m_nLine, pNode->m_pFunction);
			OutputDebugString(szMsg);
			pNode = pChunk->m_CallStack.m_Stack->GetNext(pNode);
		}
		pChunk = m_AChunkList.GetNext(pChunk);
	}

	OutputDebugString("================================ FlyBeyond Pool Report (end)  ================================\n\n");
	#endif
}