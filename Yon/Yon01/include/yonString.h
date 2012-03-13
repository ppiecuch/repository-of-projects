#ifndef _YON_CORE_YONSTRING_H_
#define _YON_CORE_YONSTRING_H_

#include "yonTypes.h"

namespace yon{
	namespace core{

		//capacity包含'\0',len不包含'\0'
		template<class T>
		class string{
		public:
			string():elements(NULL),capacity(1),len(0){
				elements=new T[capacity];
				//0x0对应的字符是'\0'；空格对应'\x20'；0x30对应数字0
				elements[0]=0x0;
			}
			string(const string<T>& other):elements(NULL),capacity(0),len(0){
				*this=other;
			}

			string(const T* const other):elements(NULL),capacity(0),len(0){
				*this=other;
			}

			~string(){
				delete[] elements;
			}

			T& operator[](u32 i){
				return *(elements+i);
			}

			const T& operator[](u32 i) const{
				return *(elements+i);
			}

			string<T>& operator=(const string<T>& other){
				if(this==&other)
					return *this;

				len=other.len;
				if(len>=capacity){
					delete[] elements;
					capacity=len+1;
					elements=new T[capacity];
				}

				for(u32 i=0;i<len;++i)
					elements[i]=other[i];
				elements[len]=0x0;
				return *this;
			}

			string<T>& operator=(const T* const other){
				if(elements==NULL){
					capacity=1;
					len=0;
					elements=new T[capacity];
					elements[0]=0x0;
				}
				if(other==NULL){
					return *this;
				}
				if(elements==other)
					return *this;

				u32 size=0;
				const T* p=other;
				while(*p){
					++size;
					++p;
				}

				if(size>=capacity)
					reallocate(size);
				for(u32 i=0;i<size;++i)
					elements[i]=*(other+i);
				elements[size]=0x0;
				len=size;
				return *this;
			}

			void operator+=(const string<T>& other) {
				u32 size=other.len+1;
				if(len+size>capacity)
					reallocate(len+size);
				for(u32 i=0;i<size;++i)
					elements[len+i]=other[i];
				elements[size+len]=0x0;
				len+=other.len;

			}
			void operator+=(const T* const other){
				if(other==NULL)
					return;

				u32 size=0;
				const T* p=other;
				while(*p){
					++size;
					++p;
				}

				if(len+size>=capacity)
					reallocate(len+size);
				for(u32 i=0;i<size;++i)
					elements[len+i]=*(other+i);
				elements[size+len]=0x0;
				len+=size;
			}

			string<T> operator+(const string<T>& other) const{
				string<T> str(*this);
				str+=other;
				return str;
			}
			string<T> operator+(const T* const other) const{
				string<T> str(*this);
				str+=other;
				return str;
			}
			const T* c_str() const
			{
				return elements;
			}
			u32 length() const{
				return len;
			}
		private:
			void reallocate(u32 length)
			{
				T* temp = elements;
				capacity = length+1;
				elements = new T[capacity];
				u32 amount = len<length ? len+1 :length+1;
				for (u32 i=0; i<amount; ++i)
					elements[i] = temp[i];
				if (length < len)
					len = length;
				delete[] temp;
			}
			T* elements;
			u32 capacity;
			u32 len;
		};
		typedef string<c8> stringc;
		typedef string<c16> stringw;
	}
}
#endif