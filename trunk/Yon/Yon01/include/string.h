#ifndef _YON_CORE_STRING_H_
#define _YON_CORE_STRING_H_

namespace yon{
	namespace core{

		template<class T>
		class string{
		public:
			string():str(NULL),capacity(1),len(0){
				str=new T[capacity];
				//0x0对应的字符是'\0'；空格对应'\x20'；0x30对应数字0
				str[0]=0x0;
			}

			~string(){
				delete[] str;
			}

			T operator[](u32 i) const{
				return *(str+i);
			}

			string<T>& operator=(const string<T>& other){
				if(this==&other)
					return *this;

				len=other.len;
				if(len>capacity){
					delete[] T;
					capacity=len+1;
					str=new T[capacity];
				}
			}

			u32 length() const{
				return len;
			}
		private:
			T* str;
			u32 capacity;
			u32 len;
		};
		typedef string<c8> stringc;
		typedef string<c16> stringw;
	}
}
#endif