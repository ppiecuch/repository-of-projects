#ifndef _YON_CORE_YONLOOP_H_
#define _YON_CORE_YONLOOP_H_

#include "yonAllocator.h"

namespace yon{
namespace core{

	template<class T, typename TAlloc = yonAllocator<T> >
	class loop{
	private:
		TAlloc allocator;

		s32 front;
		s32 rear;

		T* elements;
		s32 diff;
		s32 total;

		inline void lock(){}
		inline void unlock(){}
		inline void update(){
			diff=rear-front;
			if(diff<0)
				diff+=total;
		}
	public:
		loop(s32 capacity=1024)
			:total(0),elements(NULL),front(0),rear(0),diff(0)
		{
			elements=allocator.allocate(capacity);
			total=capacity;
		}

		~loop(){
			allocator.deallocate(elements);
			elements=NULL;
			total=front=rear=diff=0;
		}

		inline s32 capacity() const {return total;}

		inline s32 size() const{return diff;}

		inline s32 available() const{return total-diff-1;}

		// Return the max num for using pointer incrementment
		inline s32 nextable() const{
			lock();
			if(rear>=front)
			{
				if(front>0)
					return total-rear;
				else
					return total-rear-1;
			}
			s32 result=front-rear-1;
			unlock();
			return result;
		}

		T* consume_pointer(){return elements+front;}
		T* supply_pointer(){return elements+rear;}

		void consume(s32 len){
			lock();
			YON_DEBUG_BREAK_IF(len<0||len>size())
				front+=len;
			if(front>=total)
				front-=total;
			update();
			unlock();
		}

		void supply(s32 len){
			lock();
			YON_DEBUG_BREAK_IF(len<0||len>available())
				rear+=len;
			if(rear>=total)
				rear-=total;
			update();
			unlock();
		}

		void clear(){
			memset(elements,0x0,capacity);
			front=rear=diff=0;
		}
	};
}
}
#endif