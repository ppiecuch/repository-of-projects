#ifndef _YON_CORE_YONARRAY_H_
#define _YON_CORE_YONARRAY_H_

#include "yonTypes.h"
#ifdef YON_COMPILE_WITH_WIN32
#include <new.h>
#elif defined(YON_COMPILE_WITH_ANDROID)
#include <new>
#endif

#include "ILogger.h"

namespace yon{
	namespace core{

		template<class T>
		class array{
		public:
			array():elements(NULL),capacity(0),len(0){}
			array(u32 size):elements(NULL),capacity(0),len(0){
				reallocate(size);
			}
			array(const array<T>& other):elements(NULL),capacity(0),len(0){
				*this=other;
			}
			~array(){
				clear();
			}

			void print(){
				printf("\n");
				for(u32 i=0;i<len;++i)
					printf("%d,",elements[i]);
				printf("\n");
			}

			u32 size() const{
				return len;
			}
			bool empty() const{
				return len == 0;
			}

			T* pointer(){
				return elements;
			}

			const T* pointer() const{
				return elements;
			}

			T& getLast()
			{
				YON_DEBUG_BREAK_IF(len==0) // access violation
				return elements[len-1];
			}

			void push(const T& element){
				insert(element, len);
			}
			void push_back(const T& element){
				insert(element, len);
			}
			T pop(){
				return erase(len-1);
			}

			//TODO比vector慢，待优化
			//在index索引上插入arr[0,count]元素组
			void insert(u32 index,const T* arr,u32 count){
				YON_DEBUG_BREAK_IF(index>len);

				if(len+count>capacity){
					u32 newCapacity=len+count+(capacity<500? (capacity<5?5:capacity): capacity>> 2);
					reallocate(newCapacity);
				}

				s32 i;
				if(index==len){
					for(i=0;i<count;++i)
						new (&elements[len+i]) T(arr[i]);
					len+=count;
					return;
				}

				//剩下的都是index<len
				s32 tlen=len-index;//需要后移的部分的长度
				if(tlen<count){
					for(i=0;i<tlen;++i)
						new (&elements[len-1+count-i]) T(elements[len-1-i]);
					tlen=count-tlen;
					for(i=0;i<tlen;++i)
						new (&elements[len-1+tlen-i]) T(arr[count-1-i]);
					tlen=count-tlen;
					for(i=0;i<tlen;++i){
						elements[len-1-i].~T();
						new (&elements[len-1-i]) T(arr[tlen-1-i]);
					}
				}else{
					for(i=0;i<count;++i)
						new (&elements[len-1+count-i]) T(elements[len-1-i]);
					tlen=tlen-count;
					for(i=0;i<tlen;++i){
						elements[len-1-i].~T();
						new (&elements[len-1-i]) T(elements[len-1-count-i]);
					}
					for(i=count-1;i>=0;--i){
						elements[index+i].~T();
						new (&elements[index+i]) T(arr[i]);
					}
				}
				len+=count;
			}


			void insert(const T& element, u32 index){
				YON_DEBUG_BREAK_IF(index>len);

				if(len+1>capacity){
					u32 newCapacity=len+1+(capacity<500? (capacity<5?5:capacity): capacity>> 2);
					reallocate(newCapacity);
				}

				if(index==len){
					new (&elements[len]) T(element);
					++len;
					return;
				}

				//剩下的都是index<len
				new (&elements[len]) T(elements[len-1]);
				for (u32 i=len-1; i>index; --i)
				{
					elements[i].~T();
					new (&elements[i]) T(elements[i-1]);
				}
				elements[index].~T();
				new (&elements[index]) T(element);

				++len;

			}

			T erase(u32 index)
			{
				YON_DEBUG_BREAK_IF(index>=len);

				--len;

				for (u32 i=index; i<len; ++i){
					elements[i].~T();
					new (&elements[i]) T(elements[i+1]);
				}

				T result(elements[len]);

				elements[len].~T();

				return result;
			}

			void clear(){
				for (u32 i=0; i<len; ++i){
					elements[i].~T();
				}
				//delete[] elements;
				operator delete(elements);
				elements=NULL;
				capacity=0;
				len=0;
			}
			void reallocate(u32 length)
			{
				//Logger->debug("start array reallocate\n");
				T* temp = elements;
				capacity = length;
				elements = new T[capacity];
				u32 amount = len<length ? len :length;
				for (u32 i=0; i<amount; ++i){
					//elements[i] = temp[i];
					//temp[i]=NULL;
					//稍微微快一点点
					new (&elements[i]) T(temp[i]);
				}
				for (u32 i=0;i<len;++i)
					temp[i].~T();
				if (length < len)
					len = length;
				delete[] temp;//C++允许删除空指针而不造成任何问题
				//Logger->debug("end array reallocate\n");
			}

			//经测试,千万级别10元素赋值的操作与irrlicht相当
			array<T>& operator=(const array<T>& other){
				if(this==&other)
					return *this;


				//快了一些
				if(other.len>capacity){
					delete[] elements;
					capacity=other.capacity;
					elements=new T[capacity];
				}else{
					for (u32 i=0; i<len; ++i){
						elements[i].~T();
					}
				}
				len=other.len;

				
				/*if(elements!=NULL){
					clear();
				}
				len=other.len;
				capacity=other.capacity;
				if(capacity>0){
					elements=new T[capacity];
				}*/

				for (u32 i=0; i<other.len; ++i){
					new (&elements[i]) T(other.elements[i]);
				}
				return *this;
			}

			T& operator [](u32 index)
			{
				YON_DEBUG_BREAK_IF(index>=capacity);

				return elements[index];
			}

			const T& operator [](u32 index) const
			{
				YON_DEBUG_BREAK_IF(index>=len);

				return elements[index];
			}
		private:
			T* elements;
			u32 capacity;
			u32 len;
		};
	}//core
}//yon
#endif