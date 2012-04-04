#ifndef _YON_CORE_YONLIST_H_
#define _YON_CORE_YONLIST_H_

#include "yonTypes.h"
#ifdef YON_COMPILE_WITH_WIN32
#include <new.h>
#elif defined(YON_COMPILE_WITH_ANDROID)
#include <new>
#endif

namespace yon{
	namespace core{
		
		template<class T>
		class list{
		private:
			struct YonListNode
			{
				YonListNode(const T& e) : next(NULL), prev(NULL), element(e) {}

				YonListNode* next;
				YonListNode* prev;
				T element;
			};

			YonListNode* first;
			YonListNode* last;
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
				explicit Iterator(YonListNode* begin) : current(begin) {}

				YonListNode* current;

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
				explicit ConstIterator(YonListNode* begin) : current(begin) {}

				YonListNode* current;

				friend class Iterator;
				friend class list<T>;
			};

			list(): first(NULL), last(NULL), len(0) {}

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

			void push_back(const T& element){
				YonListNode* node=(YonListNode*)operator new(sizeof(YonListNode));
				new (node) YonListNode(element);
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
				YonListNode* node =(YonListNode*)operator new(sizeof(YonListNode));
				new (node) YonListNode(element);
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
					YonListNode * next = first->next;
					first->~YonListNode();
					operator delete(first);
					first = next;
				}

				last = NULL;
				len = NULL;
			}

			void operator=(const list<T>& other)
			{
				if(&other == this)
				{
					return;
				}

				clear();

				YonListNode* node = other.first;
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

				it.current->~YonListNode();
				operator delete(it.current);
				it.current = NULL;
				--len;

				return returnIterator;
			}
		};
	}
}
#endif