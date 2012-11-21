#include "FBKernal_Memory.h"

#pragma warning(disable:4311)

///////////////////////////////////////////////
CFBKernal_Memory_Pool			g_FBKernal_Memory_Pool_8(8, 256);
CFBKernal_Memory_Pool			g_FBKernal_Memory_Pool_16(16, 128);
CFBKernal_Memory_Pool			g_FBKernal_Memory_Pool_32(32, 64);
CFBKernal_Memory_Pool			g_FBKernal_Memory_Pool_64(64, 32);
CFBKernal_Memory_Pool			g_FBKernal_Memory_Pool_128(128, 16);
CFBKernal_Memory_Pool			g_FBKernal_Memory_Pool_256(256, 8);
CFBKernal_Memory_Pool			g_FBKernal_Memory_Pool_Big(0, 1);
///////////////////////////////////////////////

CFBKernal_Memory_Pool::CFBKernal_Memory_Pool(int nChunkSize, int nChunksPerBlock)
{
	FBKernal_Assert(nChunkSize >= 0 && nChunksPerBlock > 0);

	Clear();

	m_nChunkSize = nChunkSize;
	m_nChunksPerBlock = nChunksPerBlock;
}
CFBKernal_Memory_Pool::~CFBKernal_Memory_Pool()
{
	Report();

	if(m_nChunkSize > 0)
	{
		CFBKernal_Memory_Pool_BlockNode* pBlockNode = (CFBKernal_Memory_Pool_BlockNode*)m_BlockLink.GetHead();

		while(pBlockNode)
		{
			m_BlockLink.Delete(pBlockNode);

			free(pBlockNode->m_pPtr);
			delete []pBlockNode->m_pUnuseNodePtr;
			delete pBlockNode;
			pBlockNode = 0;

			pBlockNode = (CFBKernal_Memory_Pool_BlockNode*)m_BlockLink.GetHead();
		}
	}
	else
	{
		CFBKernal_Memory_Pool_UseUnuseNode* pUseNode = (CFBKernal_Memory_Pool_UseUnuseNode*)m_UseLink.GetHead();

		while(pUseNode)
		{
			m_UseLink.Delete(pUseNode);

			free(pUseNode->m_pPtr);
			delete pUseNode;
			pUseNode = 0;

			pUseNode = (CFBKernal_Memory_Pool_UseUnuseNode*)m_UseLink.GetHead();
		}
	}

	Clear();
}

CFBKernal_Memory_Pool_UseUnuseNode* CFBKernal_Memory_Pool::Alloc(int nSize, int nCount, char* pFile, int nLine)
{
	if(m_nChunkSize > 0)
	{
		int nChunkSize = nSize * nCount;

		// 内存池分配
		if(nChunkSize > m_nChunkSize || nChunkSize <= 0)
		{
			return 0;
		}

		if(m_UnuseLink.GetSize() == 0)
		{
			// 分配新的block放入blocklink
			CFBKernal_Memory_Pool_BlockNode* pBlockNode = new CFBKernal_Memory_Pool_BlockNode;
			pBlockNode->m_pPtr = (sizevoid*)malloc((m_nChunkSize + sizeof(int)) * m_nChunksPerBlock);
			m_BlockLink.InsertA(0, pBlockNode);

			// 将block中的每个chunk放入unuselink
			pBlockNode->m_pUnuseNodePtr = new CFBKernal_Memory_Pool_UseUnuseNode[m_nChunksPerBlock];
			for(int n = 0; n < m_nChunksPerBlock; n++)
			{
				pBlockNode->m_pUnuseNodePtr[n].m_pPtr = pBlockNode->m_pPtr + (m_nChunkSize + sizeof(int)) * n;
				
				// 记录结点的地址
				*((DWORD*)pBlockNode->m_pUnuseNodePtr[n].m_pPtr) = (DWORD)&pBlockNode->m_pUnuseNodePtr[n];
			
				m_UnuseLink.InsertA(0, &pBlockNode->m_pUnuseNodePtr[n]);			
			}
		}

		// 将unuselink中的尾的一个结点放到uselink尾
		CFBKernal_Memory_Pool_UseUnuseNode* pUnuseNode = (CFBKernal_Memory_Pool_UseUnuseNode*)m_UnuseLink.GetTail();
		m_UnuseLink.Delete(pUnuseNode);

		#ifdef _DEBUG
		// 记录调用位置
		strcpy(pUnuseNode->m_pFile, pFile);
		pUnuseNode->m_nLine = nLine;
		pUnuseNode->m_nTimes = m_nTotalTimes++;
		#endif
		
		pUnuseNode->m_nSize = nSize;
		pUnuseNode->m_nCount = nCount;

		m_UseLink.InsertA(0, pUnuseNode);

		// 返回指针
		return pUnuseNode;
	}
	else
	{
		int nChunkSize = nSize * nCount;

		if(nChunkSize <= 0)
		{
			return 0;
		}

		// 普通分配
		CFBKernal_Memory_Pool_UseUnuseNode* pUseNode = new CFBKernal_Memory_Pool_UseUnuseNode;
		pUseNode->m_pPtr = (sizevoid*)malloc(nChunkSize + sizeof(int));
	
		// 记录结点的地址
		*((DWORD*)pUseNode->m_pPtr) = (DWORD)pUseNode;
		pUseNode->m_nSize = nSize;
		pUseNode->m_nCount = nCount;

		#ifdef _DEBUG
		// 记录调用位置
		strcpy(pUseNode->m_pFile, pFile);
		pUseNode->m_nLine = nLine;
		pUseNode->m_nTimes = m_nTotalTimes++;
		#endif

		m_UseLink.InsertA(0, pUseNode);

		// 返回指针
		return pUseNode;
	}
	return 0;
}
void CFBKernal_Memory_Pool::Free(CFBKernal_Memory_Pool_UseUnuseNode* pUseNode)
{
	if(!pUseNode)
	{
		return;
	}

	if(m_nChunkSize > 0)
	{
		// 内存池释放
		// 将uselink中的结点放到unuselink头
		m_UseLink.Delete(pUseNode);
		m_UnuseLink.InsertB(0, pUseNode);
	}
	else
	{
		// 普通释放
		m_UseLink.Delete(pUseNode);
		free(pUseNode->m_pPtr);
		delete pUseNode;
	}
}

void CFBKernal_Memory_Pool::Report(void)
{
	#ifdef _DEBUG

	if(m_UseLink.GetSize() == 0)
	{
		return;
	}

	OutputDebugStringA("-----------------------------------------------------------------\n");
	OutputDebugStringA("               FlyBeyond Kernal Memory Report(begin)             \n");
	OutputDebugStringA("-----------------------------------------------------------------\n");

	char msg[256];
	CFBKernal_Memory_Pool_UseUnuseNode* pUseNode = (CFBKernal_Memory_Pool_UseUnuseNode*)m_UseLink.GetHead();
	while(pUseNode)
	{
		sprintf(
			msg, 
			"%s(%d) : Memory Leak! [Chunk Size: %d bytes] [Real Size: %d bytes] [Times: %d]\n", 
			pUseNode->m_pFile, 
			pUseNode->m_nLine,
			m_nChunkSize, 
			pUseNode->m_nSize * pUseNode->m_nCount,
			pUseNode->m_nTimes);

		OutputDebugStringA(msg);
		pUseNode = (CFBKernal_Memory_Pool_UseUnuseNode*)pUseNode->m_pNext;
	}

	OutputDebugStringA("-----------------------------------------------------------------\n");
	OutputDebugStringA("               FlyBeyond Kernal Memory Report(end)               \n");
	OutputDebugStringA("-----------------------------------------------------------------\n");
	#endif
}