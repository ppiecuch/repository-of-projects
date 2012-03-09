#ifndef _YON_CORE_MATRIX4_H_
#define _YON_CORE_MATRIX4_H_

#include "yonTypes.h"

namespace yon{
	namespace core{

		template<T>
		class matrix4{
		private:
			T M[16];
		public:
			matrix4();
			matrix4(const matrix4<T>& other);
			inline void makeIdentity();
			inline matrix4<T>& operator=(const matrix4<T>& other);
			const T& operator[](u32 index) const{return M[index];}
		};
	}
}
#endif