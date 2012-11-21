#ifndef _FBKernal_Memory_h_
#define _FBKernal_Memory_h_

#pragma warning(disable:4312)

#include <stdlib.h>

#include "FBKernal_Link.h"

///////////////////////////////////////////////
typedef char sizevoid;

class CFBKernal_Memory_Pool_UseUnuseNode : public CFBKernal_LinkNode<CFBKernal_Memory_Pool_UseUnuseNode>
{
protected:
	inline void Clear(void);

public:
	CFBKernal_Memory_Pool_UseUnuseNode()
	{
		Clear();
	}
	~CFBKernal_Memory_Pool_UseUnuseNode()
	{
		Clear();
	}

public:
	sizevoid*				m_pPtr;
	int						m_nSize;
	int						m_nCount;

	#ifdef _DEBUG
	char					m_pFile[256];
	int						m_nLine;
	int						m_nTimes;
	#endif
};

inline void CFBKernal_Memory_Pool_UseUnuseNode::Clear(void)
{
	m_pPtr = 0;
	m_nSize = 0;
	m_nCount = 0;

	#ifdef _DEBUG
	m_pFile[0] = '\0';
	m_nLine = 0;
	m_nTimes = 0;
	#endif
}

class CFBKernal_Memory_Pool_BlockNode : public CFBKernal_LinkNode<CFBKernal_Memory_Pool_BlockNode>
{
protected:
	inline void Clear(void);

public:
	CFBKernal_Memory_Pool_BlockNode()
	{
		Clear();
	}
	~CFBKernal_Memory_Pool_BlockNode()
	{
		Clear();
	}
	
public:
	sizevoid*								m_pPtr;
	CFBKernal_Memory_Pool_UseUnuseNode*		m_pUnuseNodePtr;
};

inline void CFBKernal_Memory_Pool_BlockNode::Clear(void)
{
	m_pPtr = 0;
	m_pUnuseNodePtr = 0;
}

class CFBKernal_Memory_Pool
{
protected:
	inline void Clear(void);

public:
	// nChunkSize == 0 使用普通内存分配
	// nChunkSize > 0  使用内存池分配
	CFBKernal_Memory_Pool(int nChunkSize, int nChunksPerBlock);
	~CFBKernal_Memory_Pool();

	CFBKernal_Memory_Pool_UseUnuseNode* Alloc(int nSize, int nCount, char* pFile = 0, int nLine = 0);
	void Free(CFBKernal_Memory_Pool_UseUnuseNode* pUseNode);

	void Report(void);

protected:
	CFBKernal_Link<CFBKernal_Memory_Pool_BlockNode>				m_BlockLink;

	CFBKernal_Link<CFBKernal_Memory_Pool_UseUnuseNode>			m_UseLink;
	CFBKernal_Link<CFBKernal_Memory_Pool_UseUnuseNode>			m_UnuseLink;

	int															m_nChunkSize;
	int															m_nChunksPerBlock;

	int															m_nTotalTimes;
};

inline void CFBKernal_Memory_Pool::Clear(void)
{
	m_nChunkSize = 0;
	m_nChunksPerBlock = 0;

	m_nTotalTimes = 0;
}

///////////////////////////////////////////////
extern CFBKernal_Memory_Pool			g_FBKernal_Memory_Pool_8;
extern CFBKernal_Memory_Pool			g_FBKernal_Memory_Pool_16;
extern CFBKernal_Memory_Pool			g_FBKernal_Memory_Pool_32;
extern CFBKernal_Memory_Pool			g_FBKernal_Memory_Pool_64;
extern CFBKernal_Memory_Pool			g_FBKernal_Memory_Pool_128;
extern CFBKernal_Memory_Pool			g_FBKernal_Memory_Pool_256;
extern CFBKernal_Memory_Pool			g_FBKernal_Memory_Pool_Big;

template<class type>
type* FBKernal_Memory_New(int nCount, CFBKernal_Memory_Pool* pPool = 0, char* pFile = 0, int nLine = 0)
{
	CFBKernal_Memory_Pool_UseUnuseNode* pUnuseNode;

	if(pPool)
	{
		pUnuseNode = pPool->Alloc(sizeof(type), nCount, pFile, nLine);
	}
	else
	{
		int nSize = sizeof(type) * nCount;
		if(nSize <= 8)
		{
			pUnuseNode = g_FBKernal_Memory_Pool_8.Alloc(sizeof(type), nCount, pFile, nLine);
		}
		else
		if(nSize <= 16)
		{
			pUnuseNode = g_FBKernal_Memory_Pool_16.Alloc(sizeof(type), nCount, pFile, nLine);
		}
		else
		if(nSize <= 32)
		{
			pUnuseNode = g_FBKernal_Memory_Pool_32.Alloc(sizeof(type), nCount, pFile, nLine);
		}
		else
		if(nSize <= 64)
		{
			pUnuseNode = g_FBKernal_Memory_Pool_64.Alloc(sizeof(type), nCount, pFile, nLine);
		}
		else
		if(nSize <= 128)
		{
			pUnuseNode = g_FBKernal_Memory_Pool_128.Alloc(sizeof(type), nCount, pFile, nLine);
		}
		else
		if(nSize <= 256)
		{
			pUnuseNode = g_FBKernal_Memory_Pool_256.Alloc(sizeof(type), nCount, pFile, nLine);
		}
		else
		if(nSize > 256)
		{
			pUnuseNode = g_FBKernal_Memory_Pool_Big.Alloc(sizeof(type), nCount, pFile, nLine);
		}
	}
	// 调用构造函数
	if(pUnuseNode)
	{
		for(int n = 0; n < pUnuseNode->m_nCount; n++)
		{
			new((type*)(pUnuseNode->m_pPtr + sizeof(int)) + n) type;
		}
	}
	else
	{
		return 0;
	}

	// 返回实际地址
	return (type*)(pUnuseNode->m_pPtr + sizeof(int));
}
template<class type>
void FBKernal_Memory_Delete(type* pPtr, CFBKernal_Memory_Pool* pPool = 0)
{
	if(!pPtr)
	{
		return;
	}

	CFBKernal_Memory_Pool_UseUnuseNode* pUseNode = (CFBKernal_Memory_Pool_UseUnuseNode*)(*((int*)((sizevoid*)pPtr - sizeof(int))));

	// 调用析构函数
	for(int n = 0; n < pUseNode->m_nCount; n++)
	{
		((type*)pPtr + n)->~type();
	}

	if(pPool)
	{
		pPool->Free(pUseNode);
	}
	else
	{
		int nSize = pUseNode->m_nSize * pUseNode->m_nCount;
		if(nSize <= 8)
		{
			g_FBKernal_Memory_Pool_8.Free(pUseNode);
		}
		else
		if(nSize <= 16)
		{
			g_FBKernal_Memory_Pool_16.Free(pUseNode);
		}
		else
		if(nSize <= 32)
		{
			g_FBKernal_Memory_Pool_32.Free(pUseNode);
		}
		else
		if(nSize <= 64)
		{
			g_FBKernal_Memory_Pool_64.Free(pUseNode);
		}
		else
		if(nSize <= 128)
		{
			g_FBKernal_Memory_Pool_128.Free(pUseNode);
		}
		else
		if(nSize <= 256)
		{
			g_FBKernal_Memory_Pool_256.Free(pUseNode);
		}
		else
		if(nSize > 256)
		{
			g_FBKernal_Memory_Pool_Big.Free(pUseNode);
		}
	}
	pUseNode = 0;
}

// 缺省的内存分配器，可以自己定义
#ifdef _DEBUG
#define FB_New(type, nCount)\
	FBKernal_Memory_New<type>(nCount, 0, __FILE__, __LINE__);
#else
#define FB_New(type, nCount)\
	FBKernal_Memory_New<type>(nCount, 0);
#endif

#define FB_Delete(pPtr)\
	FBKernal_Memory_Delete(pPtr, 0);\
	pPtr = 0;

#endif