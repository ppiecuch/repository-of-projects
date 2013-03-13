#ifndef _YON_CORE_YONGROUP_H_
#define _YON_CORE_YONGROUP_H_

#include "yonAllocator.h"
#include "yonMath.h"
#include "heapsort.h"

namespace yon{
namespace core{

	template< class T, typename TAlloc = yonAllocator<T> >
	class array{
	public:
		array()
			: data(0), allocated(0), used(0),
			strategy(ALLOC_STRATEGY_DOUBLE), free_when_destroyed(true), is_sorted(true)
		{}

		array(u32 start_count)
			: data(0), allocated(0), used(0),
			strategy(ALLOC_STRATEGY_DOUBLE), free_when_destroyed(true), is_sorted(true)
		{
			reallocate(start_count);
		}


		array(const array<T, TAlloc>& other) : data(0)
		{
			*this = other;
		}

		~array()
		{
			clear();
		}

		void print(){
			printf("\n");
			for(u32 i=0;i<used;++i)
				printf("%d,",data[i]);
			printf("\n");
		}

		const array<T, TAlloc>& operator=(const array<T, TAlloc>& other)
		{
			if (this == &other)
				return *this;
			strategy = other.strategy;

			if (data)
				clear();

			//if (allocated < other.allocated)
			if (other.allocated == 0)
				data = 0;
			else
				data = allocator.allocate(other.allocated); // new T[other.allocated];

			used = other.used;
			free_when_destroyed = true;
			is_sorted = other.is_sorted;
			allocated = other.allocated;

			for (u32 i=0; i<other.used; ++i)
				allocator.construct(&data[i], other.data[i]); // data[i] = other.data[i];

			return *this;
		}

		T& getLast()
		{
			YON_DEBUG_BREAK_IF(!used); // access violation

			return data[used-1];
		}


		//! Gets last element
		const T& getLast() const
		{
			YON_DEBUG_BREAK_IF(!used); // access violation

			return data[used-1];
		}

		T& operator [](u32 index)
		{
			YON_DEBUG_BREAK_IF(index>=used); // access violation

			return data[index];
		}

		const T& operator [](u32 index) const
		{
			YON_DEBUG_BREAK_IF(index>=used); // access violation

			return data[index];
		}

		T* pointer()
		{
			return data;
		}

		const T* const_pointer() const
		{
			return data;
		}

		u32 size() const
		{
			return used;
		}

		bool empty() const
		{
			return used == 0;
		}

		void push_back(const T& element)
		{
			insert(element, used);
		}

		void push_front(const T& element)
		{
			insert(element);
		}

		void pop_back(){
			return erase(used-1);
		}

		void pop_front(){
			return erase(0);
		}


		s32 binary_search(const T& element)
		{
			sort();
			return binary_search(element, 0, used-1);
		}
		//! Performs a binary search for an element, returns -1 if not found.
		/** \param element: Element to search for.
		\param left First left index
		\param right Last right index.
		\return Position of the searched element if it was found, otherwise -1
		is returned. */
		s32 binary_search(const T& element, s32 left, s32 right) const
		{
			if (!used)
				return -1;

			s32 m;

			do
			{
				m = (left+right)>>1;

				if (element < data[m])
					right = m - 1;
				else
					left = m + 1;

			} while((element < data[m] || data[m] < element) && left<=right);
			// this last line equals to:
			// " while((element != array[m]) && left<=right);"
			// but we only want to use the '<' operator.
			// the same in next line, it is "(element == array[m])"


			if (!(element < data[m]) && !(data[m] < element))
				return m;

			return -1;
		}

		bool isSorted() const{
			return used==0||is_sorted;
		}

		//由小到大进行排序
		//可能存在BUG：sort();array[X]=XXXX;此时仍被标为有序
		//注意，因为使用了堆排序，此排序是“不稳定的”
		void sort()
		{
			if (!is_sorted && used>1)
				heapsort(data, used);
			is_sorted = true;
		}

		void insert(const T& element, u32 index=0)
		{
			YON_DEBUG_BREAK_IF(index>used); // access violation

			if (used + 1 > allocated)
			{
				// this doesn't work if the element is in the same
				// array. So we'll copy the element first to be sure
				// we'll get no data corruption
				const T e(element);

				// increase data block
				u32 newAlloc;
				switch ( strategy )
				{
				case ALLOC_STRATEGY_DOUBLE:
					newAlloc = used + 1 + (allocated < 500 ?
						(allocated < 5 ? 5 : used) : used >> 2);
					break;
				default:
				case ALLOC_STRATEGY_SAFE:
					newAlloc = used + 1;
					break;
				}
				reallocate( newAlloc);

				// move array content and construct new element
				// first move end one up
				for (u32 i=used; i>index; --i)
				{
					if (i<used)
						allocator.destruct(&data[i]);
					allocator.construct(&data[i], data[i-1]); // data[i] = data[i-1];
				}
				// then add new element
				if (used > index)
					allocator.destruct(&data[index]);
				allocator.construct(&data[index], e); // data[index] = e;
			}
			else
			{
				// element inserted not at end
				if ( used > index )
				{
					// create one new element at the end
					allocator.construct(&data[used], data[used-1]);

					// move the rest of the array content
					for (u32 i=used-1; i>index; --i)
					{
						data[i] = data[i-1];
					}
					// insert the new element
					data[index] = element;
				}
				else
				{
					// insert the new element to the end
					allocator.construct(&data[index], element);
				}
			}
			// set to false as we don't know if we have the comparison operators
			is_sorted = false;
			++used;
		}

		//TODO比vector慢，待优化
		//在index索引上插入arr[0,count]元素组
		void insert(u32 index,const T* arr,u32 count){
			YON_DEBUG_BREAK_IF(index>used);

			if(used+count>allocated){
				u32 newCapacity=used+count+(allocated<500? (allocated<5?5:allocated): allocated>> 2);
				reallocate(newCapacity);
			}

			u32 i;
			if(index==used){
				for(i=0;i<count;++i)
					//new (&data[used+i]) T(arr[i]);
					allocator.construct(&data[used+i], arr[i]);
				used+=count;
				return;
			}

			//剩下的都是index<len
			u32 tlen=used-index;//需要后移的部分的长度
			if(tlen<count){
				for(i=0;i<tlen;++i)
					//new (&data[used-1+count-i]) T(data[used-1-i]);
					allocator.construct(&data[used-1+count-i], data[used-1-i]);
				tlen=count-tlen;
				for(i=0;i<tlen;++i)
					//new (&data[used-1+tlen-i]) T(arr[count-1-i]);
					allocator.construct(&data[used-1+tlen-i], arr[count-1-i]);
				tlen=count-tlen;
				for(i=0;i<tlen;++i){
					//data[used-1-i].~T();
					//new (&data[used-1-i]) T(arr[tlen-1-i]);
					allocator.destruct(&data[used-1-i]);
					allocator.construct(&data[used-1-i], arr[tlen-1-i]);
				}
			}else{
				for(i=0;i<count;++i)
					//new (&data[used-1+count-i]) T(data[used-1-i]);
					allocator.construct(&data[used-1+count-i], data[used-1-i]);
				tlen=tlen-count;
				for(i=0;i<tlen;++i){
					//data[used-1-i].~T();
					//new (&data[used-1-i]) T(data[used-1-count-i]);
					allocator.destruct(&data[used-1-i]);
					allocator.construct(&data[used-1-i], data[used-1-count-i]);
				}
				for(i=count-1;i>=0;--i){
					//data[index+i].~T();
					//new (&data[index+i]) T(arr[i]);
					allocator.destruct(&data[index+i]);
					allocator.construct(&data[index+i], arr[i]);
				}
			}
			is_sorted = false;
			used+=count;
		}

		void erase(u32 index)
		{
			YON_DEBUG_BREAK_IF(index>=used); // access violation

			for (u32 i=index+1; i<used; ++i)
			{
				allocator.destruct(&data[i-1]);
				allocator.construct(&data[i-1], data[i]); // data[i-1] = data[i];
			}

			allocator.destruct(&data[used-1]);

			--used;
		}

		void set_used(u32 usedNow)
		{
			if (allocated < usedNow)
				reallocate(usedNow);

			used = usedNow;
		}


		void reallocate(u32 new_size)
		{
			T* old_data = data;

			data = allocator.allocate(new_size); //new T[new_size];
			//@hzb fix bug:20130310A
			//不加此行时，data得到的空间数据是随机的，如果此时用户通过
			//[]操作符获得T& 对象直接进行操作是很危险的，必崩
			//TODO 统一清空并不合适，使用默认构造函数是否更合适？
			memset(data,0x0,new_size* sizeof(T));
			allocated = new_size;

			// copy old data
			s32 end = used < new_size ? used : new_size;

			for (s32 i=0; i<end; ++i)
			{
				// data[i] = old_data[i];
				allocator.construct(&data[i], old_data[i]);
			}

			// destruct old data
			for (u32 j=0; j<used; ++j)
				allocator.destruct(&old_data[j]);

			if (allocated < used)
				used = allocated;

			allocator.deallocate(old_data); //delete [] old_data;
		}

		void clear()
		{
			if (free_when_destroyed)
			{
				for (u32 i=0; i<used; ++i)
					allocator.destruct(&data[i]);

				allocator.deallocate(data); // delete [] data;
			}
			data = 0;
			used = 0;
			allocated = 0;
			is_sorted = true;
		}
	private:
		T* data;
		u32 allocated;
		u32 used;
		TAlloc allocator;
		eAllocStrategy strategy:4;
		bool free_when_destroyed:1;
		bool is_sorted:1;
	};

}
}
#endif