#ifndef _LUCID_CORE_LIST_H_
#define _LUCID_CORE_LIST_H_

#include "lcTypes.h"

namespace lc{
namespace core{

	/*template<class T>
	struct YonListNode
	{
		YonListNode* next;
		YonListNode* prev;
		T element;
	private:
		YonListNode(const T& e) : next(NULL), prev(NULL), element(e) {}

		template<class TT, typename TAlloc> friend class list;
	};*/
	
	//template<class T, typename TAlloc = yonAllocatorMemoryPool<YonListNode<T> > >
	template<class T>
	class list{
	private:
		struct InnerListNode
		{
			InnerListNode(const T& e) : next(NULL), prev(NULL), element(e) {}

			InnerListNode* next;
			InnerListNode* prev;
			T element;
		};

		//yonAllocatorMemoryPool<YonListNode> allocator;
		//TAlloc allocator;
		//YON_API static TAlloc allocator;
		InnerListNode* first;
		InnerListNode* last;
		u32 len;
	public:
		class ConstIterator;

		//! List iterator.
		class Iterator
		{
		public:
			Iterator() : current(0) {}

			Iterator& operator ++()    { current = current->next; return *this; }
			Iterator& operator --()    { current = current->prev; return *this; }
			Iterator  operator ++(s32) { Iterator tmp = *this; current = current->next; return tmp; }
			Iterator  operator --(s32) { Iterator tmp = *this; current = current->prev; return tmp; }

			Iterator& operator +=(s32 num)
			{
				if(num > 0)
				{
					while (num-- && this->current != 0) ++(*this);
				}
				else
				{
					while(num++ && this->current != 0) --(*this);
				}
				return *this;
			}

			Iterator  operator + (s32 num) const { Iterator tmp = *this; return tmp += num; }
			Iterator& operator -=(s32 num) const { return (*this)+=(-num); }
			Iterator  operator - (s32 num) const { return (*this)+ (-num); }

			bool operator ==(const Iterator&      other) const { return current == other.current; }
			bool operator !=(const Iterator&      other) const { return current != other.current; }
			bool operator ==(const ConstIterator& other) const { return current == other.current; }
			bool operator !=(const ConstIterator& other) const { return current != other.current; }

//#if defined (_MSC_VER) && (_MSC_VER < 1300)
//#pragma warning(disable:4284) // infix notation problem when using iterator operator ->
//#endif

			T & operator * () { return current->element; }
			T * operator ->() { return &current->element; }

		private:
			explicit Iterator(InnerListNode* begin) : current(begin) {}

			InnerListNode* current;

			//friend class list<T,TAlloc>;
			friend class list<T>;
			friend class ConstIterator;
		};

		//! List iterator for const access.
		class ConstIterator
		{
		public:

			ConstIterator() : current(0) {}
			ConstIterator(const Iterator& iter) : current(iter.current)  {}

			ConstIterator& operator ++()    { current = current->next; return *this; }
			ConstIterator& operator --()    { current = current->prev; return *this; }
			ConstIterator  operator ++(s32) { ConstIterator tmp = *this; current = current->next; return tmp; }
			ConstIterator  operator --(s32) { ConstIterator tmp = *this; current = current->prev; return tmp; }

			ConstIterator& operator +=(s32 num)
			{
				if(num > 0)
				{
					while(num-- && this->current != 0) ++(*this);
				}
				else
				{
					while(num++ && this->current != 0) --(*this);
				}
				return *this;
			}

			ConstIterator  operator + (s32 num) const { ConstIterator tmp = *this; return tmp += num; }
			ConstIterator& operator -=(s32 num) const { return (*this)+=(-num); }
			ConstIterator  operator - (s32 num) const { return (*this)+ (-num); }

			bool operator ==(const ConstIterator& other) const { return current == other.current; }
			bool operator !=(const ConstIterator& other) const { return current != other.current; }
			bool operator ==(const Iterator&      other) const { return current == other.current; }
			bool operator !=(const Iterator&      other) const { return current != other.current; }

			const T & operator * () { return current->element; }
			const T * operator ->() { return &current->element; }

			ConstIterator & operator =(const Iterator & iterator) { current = iterator.current; return *this; }

		private:
			explicit ConstIterator(InnerListNode* begin) : current(begin) {}

			InnerListNode* current;

			friend class Iterator;
			//friend class list<T,TAlloc>;
			friend class list<T>;
		};

		list(): first(NULL), last(NULL), len(0) {}

		//list(const list<T,TAlloc>& other) : first(0), last(0), len(0){
		list(const list<T>& other) : first(0), last(0), len(0){
			*this = other;
		}

		~list(){
			clear();
		}

		u32 size() const{
			return len;
		}

		bool empty() const{
			return (first == NULL);
		}

		//! Inserts an element after an element.
		/** \param it Iterator pointing to element after which the new element
		should be inserted.
		\param element The new element to be inserted into the list.
		*/
		void insert_after(const Iterator& it, const T& element)
		{
			//YonListNode* node = (YonListNode*)operator new(sizeof(YonListNode));
			//new (node) YonListNode(element);
			//YonListNode<T>* node=allocator.allocate(1);
			//allocator.construct(node,element);
			InnerListNode* node=LC_NEW InnerListNode(element);

			node->next = it.current->next;

			if (it.current->next)
				it.current->next->prev = node;

			node->prev = it.current;
			it.current->next = node;
			++len;

			if (it.current == last)
				last = node;
		}


		//! Inserts an element before an element.
		/** \param it Iterator pointing to element before which the new element
		should be inserted.
		\param element The new element to be inserted into the list.
		*/
		void insert_before(const Iterator& it, const T& element)
		{
			//YonListNode* node = (YonListNode*)operator new(sizeof(YonListNode));
			//new (node) YonListNode(element);
			//YonListNode<T>* node=allocator.allocate(1);
			//allocator.construct(node,element);
			InnerListNode* node=LC_NEW InnerListNode(element);

			node->prev = it.current->prev;

			if (it.current->prev)
				it.current->prev->next = node;

			node->next = it.current;
			it.current->prev = node;
			++len;

			if (it.current == first)
				first = node;
		}

		void push_back(const T& element){
			//YonListNode* node=(YonListNode*)operator new(sizeof(YonListNode));
			//new (node) YonListNode(element);
			//YonListNode<T>* node=allocator.allocate(1);
			//allocator.construct(node,element);
			InnerListNode* node=LC_NEW InnerListNode(element);
			++len;

			if (first == NULL)
				first = node;

			node->prev = last;

			if (last != NULL)
				last->next = node; 

			last = node;
		}

		void push_front(const T& element)
		{
			//YonListNode* node =(YonListNode*)operator new(sizeof(YonListNode));
			//new (node) YonListNode(element);
			//YonListNode<T>* node=allocator.allocate(1);
			//allocator.construct(node,element);
			InnerListNode* node=LC_NEW InnerListNode(element);
			++len;

			if (first == NULL)
			{
				last = node;
				first = node;
			}
			else
			{
				node->next = first;
				first->prev = node;
				first = node;
			}
		}

		void clear()
		{
			while(first!=NULL)
			{
				//YonListNode<T> * next = first->next;
				//first->~YonListNode();
				//operator delete(first);
				//allocator.destruct(first);
				//allocator.deallocate(first);
				InnerListNode* next=first->next;
				delete first;
				first = next;
			}

			last = NULL;
			len = 0;
		}

		//void operator=(const list<T,TAlloc>& other)
		void operator=(const list<T>& other)
		{
			if(&other == this)
			{
				return;
			}

			clear();

			//YonListNode<T>* node = other.first;
			InnerListNode* node=other.first;
			while(node)
			{
				push_back(node->element);
				node = node->next;
			}
		}

		Iterator begin()
		{
			return Iterator(first);
		}


		//! Gets first node.
		/** \return A const list iterator pointing to the beginning of the list. */
		ConstIterator begin() const
		{
			return ConstIterator(first);
		}

		Iterator end()
		{
			return Iterator(0);
		}


		//! Gets end node.
		/** \return Const list iterator pointing to null. */
		ConstIterator end() const
		{
			return ConstIterator(0);
		}

		Iterator getLast()
		{
			return Iterator(last);
		}


		//! Gets last element.
		/** \return Const list iterator pointing to the last element of the list. */
		ConstIterator getLast() const
		{
			return ConstIterator(last);
		}

		Iterator erase(Iterator it)
		{
			// suggest changing this to a const Iterator& and
			// working around line: it.Current = 0 (possibly with a mutable, or just let it be garbage?)

			Iterator returnIterator(it);
			++returnIterator;

			if(it.current == first)
			{
				first = it.current->next;
			}
			else
			{
				it.current->prev->next = it.current->next;
			}

			if(it.current == last)
			{
				last = it.current->prev;
			}
			else
			{
				it.current->next->prev = it.current->prev;
			}

			//it.current->~YonListNode();
			//operator delete(it.current);
			//allocator.destruct(it.current);
			//allocator.deallocate(it.current);
			delete it.current;
			it.current = NULL;
			--len;

			return returnIterator;
		}
	};
}
}
#endif