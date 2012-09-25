#ifndef _YON_CORE_YONSET_H_
#define _YON_CORE_YONSET_H_

/**
from:http://ezgame.googlecode.com/svn/EzGame-1.0.0/CoreLibs/EzCore/EzSet.h
#include "EzAllocator.h"

template <class T, typename TAlloc = EzAllocatorInterface<T> > 
class EZCORE_ENTRY EzSet
{
public:
	EzSet(unsigned int uiInitialSize = 0);
	~EzSet(void);

	void reallocate(unsigned int new_size);
	void clear();
	unsigned int find(const T& element) const;
	unsigned int add(const T& element);
	void remove(unsigned int uiIndex);
	void orderedRemove(unsigned int uiIndex);
	unsigned int size();

	//operator 
	void operator=(const EzSet<T>& other);
	bool operator == (const EzSet<T>& other) const;
	bool operator != (const EzSet<T>& other) const;
	T& operator [](unsigned int index);
	const T& operator [](unsigned int index) const;



protected:
	T				*m_pBase;
	unsigned int				m_uiAllocated;
	unsigned int				m_uiUsed;
};


template <class T, typename TAlloc>
inline EzSet<T, TAlloc>::EzSet(unsigned int uiInitialSize)
:m_pBase(NULL), m_uiAllocated(uiInitialSize), m_uiUsed(0)
{
	reallocate(uiInitialSize);
}

template <class T, typename TAlloc>
inline EzSet<T, TAlloc>::~EzSet()
{
	for (unsigned int i=0; i<m_uiUsed; ++i)
	{
		TAlloc::destruct(&m_pBase[i]);
		TAlloc::deallocate(m_pBase);
	}
}


template <class T, typename TAlloc>
inline void EzSet<T, TAlloc>::reallocate(unsigned int new_size)
{
	T* old_data = m_pBase;

	m_pBase = TAlloc::allocate(new_size);
	m_uiAllocated = new_size;

	// copy old data
	int end = m_uiUsed < new_size ? m_uiUsed : new_size;

	for (int i=0; i<end; ++i)
	{
		TAlloc::construct(&m_pBase[i], old_data[i]);
	}

	// destruct old data
	for (unsigned int j=0; j<m_uiUsed; ++j)
		TAlloc::destruct(&old_data[j]);

	if (m_uiAllocated < m_uiUsed)
		m_uiUsed = m_uiAllocated;

	TAlloc::deallocate(old_data);
}


template <class T, typename TAlloc>
unsigned int EzSet<T, TAlloc>::size()
{
	return m_uiUsed;
}

template <class T, typename TAlloc>
inline void EzSet<T, TAlloc>::clear()
{
	for (unsigned int i=0; i<m_uiUsed; ++i)
		TAlloc::destruct(&m_pBase[i]);

	TAlloc::deallocate(m_pBase);

	m_pBase = NULL;
	m_uiAllocated = 0;
	m_uiUsed = 0;
}


template <class T, typename TAlloc>
inline unsigned int EzSet<T, TAlloc>::find(const T& element) const
{
	// If the element is in the list, the index is returned, else -1.

	EZASSERT(m_uiUsed <= m_uiAlloced);

	unsigned int i;

	for (i = 0; i < m_uiUsed; i++)
	{
		if (m_pBase[i] == element)
			return i;
	}
	return -1;
}

template <class T, typename TAlloc>
inline unsigned int EzSet<T, TAlloc>::add(const T& element)
{
	EZASSERT(m_uiUsed <= m_uiAllocated);

	if (m_uiUsed == m_uiAllocated)
	{
		reallocate(m_uiAllocated > 0 ? (2 * m_uiAllocated) : 1);
	}

	EZASSERT(m_uiUsed < m_uiAllocated);
	m_pBase[m_uiUsed++] = element;

	return m_uiUsed - 1;
}

template <class T, typename TAlloc>
inline void EzSet<T, TAlloc>::remove(unsigned int uiIndex)
{
	EZASSERT(uiIndex < m_uiUsed);

	m_pBase[uiIndex] = m_pBase[--m_uiUsed];
}

template <class T, typename TAlloc>
inline void EzSet<T, TAlloc>::orderedRemove(unsigned int uiIndex)
{
	EZASSERT(uiIndex < m_uiUsed);

	for (unsigned int ui = uiIndex; ui < m_uiUsed - 1; ui++)
	{
		m_pBase[ui] = m_pBase[ui + 1];
	}
	m_uiUsed--;
}

template <class T, typename TAlloc>
inline void EzSet<T, TAlloc>::operator=(const EzSet<T>& other)
{
	if (m_pBase)
	{
		for (unsigned int i=0; i<m_uiUsed; ++i)
			TAlloc::destruct(&m_pBase[i]);

		TAlloc::deallocate(m_pBase);

	}

	//if (allocated < other.allocated)
	if (other.m_uiAllocated == 0)
		m_pBase = 0;
	else
		m_pBase = TAlloc::allocate(other.m_uiAllocated); // new T[other.allocated];

	m_uiUsed = other.m_uiUsed;
	m_uiAllocated = other.m_uiAllocated;


	for (unsigned int i=0; i<other.used; ++i)
		TAlloc::construct(&m_pBase[i], other.m_pBase[i]); // data[i] = other.data[i];
}


template <class T, typename TAlloc>
inline bool EzSet<T, TAlloc>::operator == (const EzSet<T>& other) const
{
	if (m_uiUsed != other.m_uiUsed)
		return false;

	for (unsigned int i=0; i<other.m_uiUsed; ++i)
	{
		if (m_pBase[i] != other[i])
			return false;
	}
	return true;
}

template <class T, typename TAlloc>
inline bool EzSet<T, TAlloc>::operator != (const EzSet<T>& other) const
{
	return !(*this==other);
}


//! Direct access operator
template <class T, typename TAlloc>
inline T& EzSet<T, TAlloc>::operator [](unsigned int index)
{
	EZVERIFY(index>=m_uiUsed) // access violation
		return m_pBase[index];
}


//! Direct const access operator
template <class T, typename TAlloc>
inline const T& EzSet<T, TAlloc>::operator [](unsigned int index) const
{
	EZVERIFY(index>=m_uiUsed) // access violation
		return m_pBase[index];
}

*/
#endif