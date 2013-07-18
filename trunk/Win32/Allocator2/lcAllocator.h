#ifndef _LUCID_ALLOCATOR_H_
#define _LUCID_ALLOCATOR_H_

#include "lcException.h"
#include "lcSingleton.h"
#include "lcBackwardCompatibility.h"
#include "lcMutex.h"
//���������Ԥ������ڴ��ϴ���������ȱʡ��new���������в�ͨ�ġ�Ҫ���������⣬�������placement new���졣
#include <new>

//refer to:SGIMallocPool.hhp

namespace lc{

//TODO ��Ϊ�ǹ�����

//refer to:http://holos.googlecode.com/svn/trunk/src/h_MemTracer.h
class MemoryTracer : public core::Singleton<MemoryTracer>,public Obsolete<MemoryTracer>{
private:
	struct TracerNode{
		TracerNode():Prev(NULL),Next(NULL),Addr(NULL),Size(0),File(NULL),Func(NULL),Line(-1){}
		TracerNode(void* p,u32 size):Prev(NULL),Next(NULL),Addr(p),Size(size),File(NULL),Func(NULL),Line(-1){}
		TracerNode(void* p,u32 size,const c8* file,const c8* func,s32 line):Prev(NULL),Next(NULL),Addr(p),Size(size),File(file),Func(func),Line(line){}

		TracerNode* Prev;
		TracerNode* Next;

		void*		Addr;
		u32			Size;
		const c8*	File;
		const c8*	Func;
		s32			Line;
	};

	static mutex s_mutex;

	TracerNode* m_pHead;
	TracerNode* m_pTail;

	u32			m_uAllocatedSize;
	u32			m_uAllocatedCount;
	u32			m_uMaxSize;

	MemoryTracer();
	static void lock();
	static void unlock();
	bool canAllocate(size_t sz);
	void addNode(TracerNode* node);
	void removeNode(TracerNode* node);
	void deallocateNode(TracerNode* node);

	friend class core::Singleton<MemoryTracer>;
	friend void* ::operator new(LC_ALLOC_PARAMS(size_t size));
	friend void ::operator delete(void* ptr);
public:

	void allocate(LC_ALLOC_PARAMS(void* p,size_t sz));
	void deallocate(void* ptr);
	u32 getAllocatedSize() const{return m_uAllocatedSize;}
	u32 getAllocatedCount() const{return m_uAllocatedCount;}
	void setMaxSize(size_t sz){m_uMaxSize=sz;}
	u32 getMaxSize() const{return m_uMaxSize;}
	void dump();
	virtual void destroy();
};

template<bool isMT,size_t size>
class FixedMemoryPool : public Uncopyable{
private:
	//ROUND_UP ��bytes�ϵ���__ALIGN�ı���
	enum { ROUND = (size + s_uAlign - 1) & ~(s_uAlign - 1) };

	union FixedNode
	{
		union FixedNode* Node;
		c8 Data[1];
	};
	typedef typename VolatileTrait<FixedNode, isMT>::ValueType	FixedNodePtr;
	typedef typename LockTrait<isMT>::ValueType LockType;

	static const u32 s_uGrowth;
	static const u32 s_uAlign;

	LockType m_lock;

	// �ڴ����ʼλ��
	c8 *m_pStartFree;
	// �ڴ�ؽ���λ��
	c8 *m_pEndFree;
	// ������Ŀռ��С
	u32 m_uCapacity;

	// free����
	FixedNodePtr m_pFrees[1];

	// roundup ��bytes�ϵ���s_uAlign�ı���
	static inline u32 roundup(u32 bytes)
	{
		return ((bytes) + s_uAlign - 1) & ~(s_uAlign - 1);
	}

	// ���������С������ʹ�õ�n��frees��n��0����
	static inline size_t indexInFrees(size_t bytes)
	{
		return (((bytes) + s_uAlign - 1) / s_uAlign) - 1;
	}

	// !����һ���ռ�,������count����СΪunit������
	// ��Щ�ڴ��������ַ��������һ���, ������ָ��
	c8* allocateChunk(size_t unit,size_t& count);

	// ����һ����СΪn�Ķ���,�������СΪn���������鵽free������
	FixedNode* refill(size_t n);

	// ����ڴ�
	void clear();
public:
	FixedMemoryPool();
	~FixedMemoryPool();

	// n�������0
	void* allocate(size_t n);

	// p����Ϊ��
	void deallocate(void *p, size_t n);
};

struct PrimitiveAllocateTrait{
	static void* allocate(size_t size);
	static void* allocate(size_t size,const c8* file,const c8* func,s32 line);
	static void deallocate(void *p, size_t size);
};

struct FixedSizeAllocateTrait{
	static void* allocate(size_t size);
	static void* allocate(size_t size,const c8* file,const c8* func,s32 line);
	static void deallocate(void *p, size_t size);
};

//TODO mem namespace
enum ENUM_ALLOCATE_STRATEGY{
	ENUM_ALLOCATE_STRATEGY_PRIMITIVE = 0,
	ENUM_ALLOCATE_STRATEGY_FIXED_SIZE,
	ENUM_ALLOCATE_STRATEGY_COUNT
};

template<ENUM_ALLOCATE_STRATEGY strategy>
struct AllocateTrait
{
	typedef PrimitiveAllocateTrait ValueType;
};

template<>
struct AllocateTrait<ENUM_ALLOCATE_STRATEGY_FIXED_SIZE>
{
	typedef FixedSizeAllocateTrait ValueType;
};

template<typename T,ENUM_ALLOCATE_STRATEGY strategy,bool isMT>
class Allocator{
	typedef typename AllocateTrait<strategy>::ValueType AllocateType;
public:
	T* allocate(size_t cnt)
	{
		return cnt==0?NULL:(T*)AllocateType::allocate(cnt* sizeof(T));
	}

	T* allocate(size_t cnt,const c8* file,const c8* func,s32 line)
	{
		return cnt==0?NULL:(T*)AllocateType::allocate(cnt* sizeof(T),file,func,line);
	}

	//! Deallocate memory for an array of objects
	void deallocate(T* ptr)
	{
		if(ptr==NULL)
			return;
		AllocateType::deallocate(ptr,sizeof(T));
	}

	//! Construct an element
	void construct(T* ptr, const T&e)
	{
		new ((void*)ptr) T(e);
	}

	//! Destruct an element
	void destruct(T* ptr)
	{
		ptr->~T();
	}
};

}

#endif