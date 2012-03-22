#ifndef _YON_CORE_EXCEPTION_H_
#define _YON_CORE_EXCEPTION_H_

#include "yonString.h"

namespace yon{
	namespace core{

		class exception{
		private:
			core::stringc msg;
		public:
			exception(const core::stringc& msg):msg(msg){}
			inline const core::stringc& what() const{
				return msg;
			}
		};
	}
}
#endif