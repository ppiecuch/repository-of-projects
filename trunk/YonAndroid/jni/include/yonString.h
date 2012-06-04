#ifndef _YON_CORE_YONSTRING_H_
#define _YON_CORE_YONSTRING_H_

#include "yonTypes.h"
#include <stdio.h>
#include <stdarg.h>
#include "yonMath.h"

#ifdef YON_COMPILE_WITH_ANDROID
#include <wchar.h>
#endif

namespace yon{
	namespace core{

		//! Returns a character converted to lower case
		static inline u32 locale_lower ( u32 x )
		{
			// ansi
			return x >= 'A' && x <= 'Z' ? x + 0x20 : x;
		}

		//! Returns a character converted to upper case
		static inline u32 locale_upper ( u32 x )
		{
			// ansi
			return x >= 'a' && x <= 'z' ? x + ( 'A' - 'a' ) : x;
		}
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

			template <class B>
			string(const string<B>& other):elements(NULL),capacity(0),len(0)
			{
				*this = other;
			}


			/*string(const T* const other):elements(NULL),capacity(0),len(0){
				*this=other;
			}*/
			
			string(const c8* const pFmt,...):elements(NULL),capacity(0),len(0){
				va_list args;
				va_start(args,pFmt);
				T buffer[1024];
				vsprintf_s(buffer,1024,pFmt,args);
				va_end(args);

				*this=buffer;
			}

			string(const wchar_t* const pFmt,...):elements(NULL),capacity(0),len(0){
				va_list args;
				va_start(args,pFmt);
				T buffer[1024];
				vswprintf_s(buffer,1024,pFmt,args);
				va_end(args);

				*this=buffer;
			}

			/*explicit string(const double number):elements(NULL),capacity(0),len(0){
				c8 tmpbuf[255];
				sprintf_s(tmpbuf, 255, "%0.6f", number);
				*this = tmpbuf;
			}

			explicit string(const int number):elements(NULL),capacity(0),len(0){
				c8 tmpbuf[16];
				sprintf_s(tmpbuf, 16, "%d", number);
				*this = tmpbuf;
			}

			explicit string(const unsigned int number):elements(NULL),capacity(0),len(0){
				c8 tmpbuf[16];
				sprintf_s(tmpbuf, 16, "%d", number);
				*this = tmpbuf;
			}

			explicit string(const long number):elements(NULL),capacity(0),len(0){
				c8 tmpbuf[32];
				sprintf_s(tmpbuf, 32, "%ld", number);
				*this = tmpbuf;
			}*/

			~string(){
				delete[] elements;
			}

			T& operator[](u32 i){
				return *(elements+i);
			}

			const T& operator[](u32 i) const{
				return *(elements+i);
			}

			template <class B>
			string<T>& operator=(const B* const str)
			{
				if (!str)
				{
					if (!elements)
					{
						capacity = 1;
						elements = new T[capacity];
					}
					len = 0;
					elements[0] = 0x0;
					return *this;
				}

				if ((void*)str == (void*)elements)
					return *this;

				u32 size = 0;
				const B* p = str;
				do
				{
					++size;
				} while(*p++);

				T* oldArray = elements;

				len = size;
				if (len>=capacity)
				{
					capacity = len+1;
					elements = new T[capacity];
				}

				for (u32 i = 0; i<len; ++i)
					elements[i] = (T)str[i];

				if (oldArray != elements)
					delete[] oldArray;

				return *this;
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
			/*void operator+=(const int num){
				*this+=core::string<T>(num);
			}
			void operator+=(const unsigned int num){
				*this+=core::string<T>(num);
			}
			void operator+=(const long num){
				*this+=core::string<T>(num);
			}
			void operator+=(const double num){
				*this+=core::string<T>(num);
			}*/
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

			/*string<T> operator+(const int num){
				string<T> str(*this);
				str+=num;
				return str;
			}
			string<T> operator+(const unsigned int num){
				string<T> str(*this);
				str+=num;
				return str;
			}
			string<T> operator+(const long num){
				string<T> str(*this);
				str+=num;
				return str;
			}
			string<T> operator+(const double num){
				string<T> str(*this);
				str+=num;
				return str;
			}*/
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
			bool operator !=(const T* const str) const
			{
				return !(*this == str);
			}

			bool operator !=(const string<T>& other) const
			{
				return !(*this == other);
			}
			bool operator ==(const T* const str) const
			{
				if (!str)
					return false;

				u32 i;
				for(i=0; elements[i] && str[i]; ++i)
					if (elements[i] != str[i])
						return false;

				return !elements[i] && !str[i];
			}


			bool operator ==(const string<T>& other) const
			{
				for(u32 i=0; elements[i] && other.elements[i]; ++i)
					if (elements[i] != other.elements[i])
						return false;

				return len == other.len;
			}

			//! Is smaller comparator
			bool operator <(const string<T>& other) const
			{
				for(u32 i=0; elements[i] && other.elements[i]; ++i)
				{
					s32 diff = elements[i] - other.elements[i];
					if ( diff )
						return diff < 0;
				}

				return len < other.len;
			}

			s32 findFirst(T c) const
			{
				//TODO优化
				for (u32 i=0; i<len; ++i)
					if (elements[i] == c)
						return i;

				return -1;
			}
			s32 findLast(T c, s32 start = -1) const
			{
				//TODO优化
				start = core::clamp(start<0?(s32)(len)-1:start, 0, (s32)(len) - 1);
				for (s32 i=start; i>=0; --i)
					if (elements[i] == c)
						return i;

				return -1;
			}

			template <class B>
			s32 find(const B* const str, const u32 start = 0) const
			{
				if (str && *str)
				{
					u32 index = 0;

					while (str[index])
						++index;

					if (index > len-1)
						return -1;

					for (u32 i=start; i<len-index; ++i)
					{
						u32 j=0;

						while(str[j] && elements[i+j] == str[j])
							++j;

						if (!str[j])
							return i;
					}
				}

				return -1;
			}
			void replace(T toReplace, T replaceWith)
			{
				for (u32 i=0; i<len; ++i)
					if (elements[i] == toReplace)
						elements[i] = replaceWith;
			}
			bool equalsSubstringIgnoreCase(const string<T>&other, const s32 fromIndex = 0 ) const
			{
				if ((u32)fromIndex>len)
					return false;

				u32 i;
				for( i=0; elements[fromIndex + i] && other[i]; ++i)
					if (locale_lower(elements[fromIndex + i]) != locale_lower(other[i]))
						return false;
				return elements[fromIndex + i] == 0 && other[i] == 0;
			}
			string<T> subString(u32 begin, s32 size) const
			{
				if ((size <= 0) || (begin>=length()))
					return string<T>("");
				// clamp length to maximal value
				if ((size+begin) > length())
					size = length()-begin;

				string<T> o;
				o.reallocate(size);

				for (s32 i=0; i<size; ++i)
					o.elements[i] = elements[i+begin];

				o.elements[size] = 0x0;
				o.len = size;

				return o;
			}
			string<T> subString(u32 begin) const
			{
				if (begin>=length())
					return string<T>("");

				u32 size=length()-begin;

				return subString(begin,size);
			}
			//! split string into parts.
			/** This method will split a string at certain delimiter characters
			into the container passed in as reference. The type of the container
			has to be given as template parameter. It must provide a push_back and
			a size method.
			\param ret The result container
			\param c C-style string of delimiter characters
			\param count Number of delimiter characters
			**/
			template<class container>
			u32 split(container& ret, const T* const c, u32 count=1) const
			{
				if (!c)
					return 0;

				const u32 oldSize=ret.size();
				u32 lastpos = 0;
				for (u32 i=0; i<len; ++i)
				{
					for (u32 j=0; j<count; ++j)
					{
						if (elements[i] == c[j])
						{
							ret.push_back(subString(lastpos, i - lastpos));
							lastpos = i + 1;
							i+=count;
							break;
						}
					}
				}
				if (len > lastpos)
					ret.push_back(subString(lastpos, len- lastpos));
				return ret.size()-oldSize;
			}
			void makeLower()
			{
				for (u32 i=0; i<len; ++i)
					elements[i] = locale_lower( elements[i] );
			}

			void makeUpper()
			{
				for (u32 i=0; i<len; ++i)
					elements[i] = locale_upper( elements[i] );
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
		typedef string<wchar_t> stringw;
	}
}
#endif