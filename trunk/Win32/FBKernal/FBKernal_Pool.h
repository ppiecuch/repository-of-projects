#ifndef _FBKernal_Pool_h_
#define _FBKernal_Pool_h_

#include <stdio.h>
#include <new.h>
#include <Windows.h>
//#include "FBKernal_List.h"
//#include "FBKernal_CallStack.h"

class CFBKernal_Pool;

template<class Type>
void FBKernal_Pool_Construct(Type* pPtr)
{
	if(!pPtr)
	{
		return;
	}
	new(pPtr) Type;
}
template<class Type>
void FBKernal_Pool_Destruct(Type* pPtr)
{
	if(!pPtr)
	{
		return;
	}
	pPtr->~Type();
}

#ifdef __DEBUG
struct CFBKernal_Pool_Chunk : public CFBKernal_ListNode<CFBKernal_Pool_Chunk>
#else
struct CFBKernal_Pool_Chunk
#endif
{
	#ifdef __DEBUG
	CFBKernal_CallStack			m_CallStack;
	DWORD						m_dwSize;
	#endif

	union
	{
		DWORD					m_dwCount;
		#ifndef __DEBUG
		CFBKernal_Pool_Chunk*	m_pNext;
		#endif
	};

	CFBKernal_Pool*				m_pPool;
};

struct CFBKernal_Pool_Block
{
	char*					m_pPtr;
	char*					m_pPtr_Cur;

	CFBKernal_Pool_Block*	m_pNext;
};

class CFBKernal_Pool
{
public:
	CFBKernal_Pool(DWORD dwChunkSize);
	virtual ~CFBKernal_Pool();

	void* Malloc(DWORD dwSize, DWORD dwCount, BOOL bDump);
	void Free(void* pPtr);

	template<class Type>
	Type* New(DWORD dwCount, BOOL bDump);

	template<class Type>
	void Delete(Type* pPtr);

	void Report(void);

protected:
	DWORD									m_dwChunkSize;
	DWORD									m_dwChunksPerBlock;

	CFBKernal_Pool_Block*					m_pBlockHead;

	#ifdef __DEBUG
	BOOL									m_bDump;

	CFBKernal_List<CFBKernal_Pool_Chunk>	m_AChunkList;
	CFBKernal_List<CFBKernal_Pool_Chunk>	m_NChunkList;
	#else
	CFBKernal_Pool_Chunk*					m_pNChunkHead;
	#endif
};

template<class Type>
Type* CFBKernal_Pool::New(DWORD dwCount, BOOL bDump)
{
	Type* pPtr = (Type*)Malloc(sizeof(Type), dwCount, bDump);
	if(!pPtr)
	{
		return 0;
	}

	Type* pTemp = pPtr;
	for(DWORD n = 0; n < dwCount; n++)
	{
		new(pTemp) Type;
		pTemp++;
	}
	return pPtr;
}
template<class Type>
void CFBKernal_Pool::Delete(Type* pPtr)
{
	if(!pPtr)
	{
		return;
	}

	Type* pTemp = pPtr;
	DWORD dwCount = ((CFBKernal_Pool_Chunk*)((char*)pPtr - sizeof(CFBKernal_Pool_Chunk)))->m_dwCount;
	for(DWORD n = 0; n < dwCount; n++)
	{
		pTemp->~Type();
		pTemp++;
	}
	Free(pPtr);
}

#endif
