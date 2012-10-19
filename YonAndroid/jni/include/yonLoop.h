#ifndef _YON_CORE_YONLOOP_H_
#define _YON_CORE_YONLOOP_H_

#include "yonAllocator.h"

namespace yon{
namespace core{

	//0 1 2 3 4
	template< class T, typename TAlloc = yonAllocator<T> >
	class loop{
	private:
		s32 front;
		s32 rear;
		s32 current;

		s32 total;
		s32 diff;

		TAlloc allocator;
		T* elements;

		inline void lock(){}
		inline void unlock(){}
		inline void update(){
			diff=rear-front;
			if(rear<0)
				rear+=total;
		}
	public:
		loop(s32 capacity=1024)
			:front(0),rear(0),current(0),total(0),diff(0),elements(NULL){
				elements=allocator.allocate(capacity);
				total=capacity;
		}
		~loop(){
			if(elements)
				allocator.deallocate(elements);
		}

		inline s32 capacity(){return total;}
		inline s32 size(){return diff;}
		inline s32 available(){return total-diff-1;}
		inline s32 increasable(){
			if(rear>=front)
			{
				if(front>0)
					return total-rear;
				else
					return total-rear-1;
			}
			return front-rear-1;
		}
	};
}
}
#endif