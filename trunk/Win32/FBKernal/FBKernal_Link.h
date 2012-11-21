#ifndef _FBKernal_Link_h_
#define _FBKernal_Link_h_

#include "FBKernal_Assert.h"

template<class Owner>
class CFBKernal_LinkNode
{
public:
	CFBKernal_LinkNode();
	virtual ~CFBKernal_LinkNode();

public:
	Owner*      m_pPrev;
	Owner*      m_pNext;
};

template<class Owner>
CFBKernal_LinkNode<Owner>::CFBKernal_LinkNode()
{
	m_pPrev = 0;
	m_pNext = 0;
}
template<class Owner>
CFBKernal_LinkNode<Owner>::~CFBKernal_LinkNode()
{
	m_pPrev = 0;
	m_pNext = 0;
}

template<class Type>
class CFBKernal_Link
{
protected:
	void Clear(void);

public:
	CFBKernal_Link();
	~CFBKernal_Link();

	// ָ�����ǰ�����
	// pPos = 0 ͷ���
	void InsertB(Type* pPos, Type* pNode);
	// ָ�����������
	// pPos = 0 β���
	void InsertA(Type* pPos, Type* pNode);
	
	void Delete(Type* pNode);
	void DeleteAll(void);

	int GetSize(void);
	Type* GetHead(void);
	Type* GetTail(void);

	Type* Get(int nIndex);

private:
	Type*				m_pHead;
	Type*				m_pTail;
	int					m_nSize;
};

template<class Type>
void CFBKernal_Link<Type>::Clear(void)
{
	m_pHead = 0;
	m_pTail = 0;
	m_nSize = 0;
}

template<class Type>
CFBKernal_Link<Type>::CFBKernal_Link()
{
	Clear();
}
template<class Type>
CFBKernal_Link<Type>::~CFBKernal_Link()
{
	Clear();
}

template<class Type>
void CFBKernal_Link<Type>::InsertB(Type* pPos, Type* pNode)
{
	FBKernal_Assert(pNode);
	FBKernal_Assert((m_pHead && m_pTail) || (!m_pHead && ! m_pTail));

	//#ifdef _DEBUG
	//// ��ֹ����Ѿ����ڵĽ��
	//BOOL bExist = false;
	//Type* pNodeIt = m_pHead;
	//while(pNodeIt)
	//{
	//	if(pNodeIt == pNode)
	//	{
	//		bExist = true;
	//		break;
	//	}
	//	pNodeIt = pNodeIt->m_pNext;
	//}
	//FBKernal_Assert(!bExist);
	//#endif

	if(!pPos)
	{
		if(!m_pHead && !m_pTail)
		{
			pNode->m_pPrev = 0;
			pNode->m_pNext = 0;

			m_pHead = pNode;
			m_pTail = pNode;
		}
		else
		{
			pNode->m_pPrev = 0;
			pNode->m_pNext = m_pHead;

			m_pHead->m_pPrev = pNode;
			m_pHead = pNode;
		}
	}
	else
	{
		// ��ӵ�ָ��λ��ǰ
		if(pPos->m_pPrev)
		{
			pPos->m_pPrev->m_pNext = pNode;
		}
		pNode->m_pPrev = pPos->m_pPrev;

		pNode->m_pNext = pPos;
		pPos->m_pPrev = pNode;

		if(pNode->m_pPrev == 0)
		{
			m_pHead = pNode;
		}
	}
	m_nSize++;
}

template<class Type>
void CFBKernal_Link<Type>::InsertA(Type* pPos, Type* pNode)
{
	FBKernal_Assert(pNode);
	FBKernal_Assert((m_pHead && m_pTail) || (!m_pHead && ! m_pTail));

	//#ifdef _DEBUG
	//// ��ֹ����Ѿ����ڵĽ��
	//BOOL bExist = false;
	//Type* pNodeIt = m_pHead;
	//while(pNodeIt)
	//{
	//	if(pNodeIt == pNode)
	//	{
	//		bExist = true;
	//		break;
	//	}
	//	pNodeIt = pNodeIt->m_pNext;
	//}
	//FBKernal_Assert(!bExist);
	//#endif
	
	if(!pPos)
	{
		// ��ӵ�β
		if(!m_pHead && !m_pTail)
		{
			pNode->m_pPrev = 0;
			pNode->m_pNext = 0;

			m_pHead = pNode;
			m_pTail = pNode;
		}
		else
		{
			pNode->m_pPrev = m_pTail;
			pNode->m_pNext = 0;

			m_pTail->m_pNext = pNode;
			m_pTail = pNode;
		}
	}
	else
	{
		// ��ӵ�ָ��λ�ú�
		if(pPos->m_pNext)
		{
			pPos->m_pNext->m_pPrev = pNode;
		}
		pNode->m_pNext = pPos->m_pNext;

		pNode->m_pPrev = pPos;
		pPos->m_pNext = pNode;

		if(pNode->m_pNext == 0)
		{
			m_pTail = pNode;
		}
	}
	m_nSize++;
}

template<class Type>
void CFBKernal_Link<Type>::Delete(Type* pNode)
{
	FBKernal_Assert(pNode);

	//#ifdef _DEBUG
	//// ��ֹɾ��û�еĽ��
	//BOOL bExist = false;
	//Type* pNodeIt = m_pHead;
	//while(pNodeIt)
	//{
	//	if(pNodeIt == pNode)
	//	{
	//		bExist = true;
	//		break;
	//	}
	//	pNodeIt = pNodeIt->m_pNext;
	//}
	//FBKernal_Assert(bExist);
	//#endif

	if(pNode->m_pPrev)
	{
		pNode->m_pPrev->m_pNext = pNode->m_pNext;
	}
	if(pNode->m_pNext)
	{
		pNode->m_pNext->m_pPrev = pNode->m_pPrev;
	}
	if(m_pHead == pNode)
	{
		m_pHead = pNode->m_pNext;
	}
	if(m_pTail == pNode)
	{
		m_pTail = pNode->m_pPrev;
	}
	m_nSize--;
}
template<class Type>
void CFBKernal_Link<Type>::DeleteAll(void)
{
	Clear();
}

template<class Type>
int CFBKernal_Link<Type>::GetSize(void)
{
	return m_nSize;
}
template<class Type>
Type* CFBKernal_Link<Type>::GetHead(void)
{
	return m_pHead;
}
template<class Type>
Type* CFBKernal_Link<Type>::GetTail(void)
{
	return m_pTail;
}

#endif