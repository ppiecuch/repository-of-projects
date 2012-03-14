#ifndef _YON_CORE_YONARRAY_H_
#define _YON_CORE_YONARRAY_H_

#include "yonTypes.h"
#include <new.h>//����placement new;���Ȱ������ļ�

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

			//������,ǧ�򼶱�10Ԫ�ظ�ֵ�Ĳ�����irrlicht�൱
			array<T>& operator=(const array<T>& other){
				if(this==&other)
					return *this;
				
				if(elements!=NULL){
					clear();
				}
				len=other.len;
				capacity=other.capacity;
				if(capacity>0){
					elements=new T[capacity];
				}

				for (u32 i=0; i<other.len; ++i){
					new (&elements[i]) T(other.elements[i]);
				}
				return *this;
			}
		private:
			void clear(){
				for (u32 i=0; i<len; ++i){
					elements[i].~T();
				}
				delete[] elements;
				elements=NULL;
				capacity=0;
				len=0;
			}
			void reallocate(u32 length)
			{
				T* temp = elements;
				capacity = length;
				elements = new T[capacity];
				u32 amount = len<length ? len :length;
				for (u32 i=0; i<amount; ++i){
					//elements[i] = temp[i];
					//temp[i]=NULL;
					//��΢΢��һ���
					new (&elements[i]) T(temp[i]);
				}
				if (length < len)
					len = length;
				delete[] temp;
			}
			T* elements;
			u32 capacity;
			u32 len;
		};
	}//core
}//yon
#endif