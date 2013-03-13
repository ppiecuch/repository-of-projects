#ifndef _YON_CORE_IREFERENCABLE_H_
#define _YON_CORE_IREFERENCABLE_H_

#include "yonTypes.h"

namespace yon{

	namespace core{

		class IReferencable{
		public:
			IReferencable():referenced(1){}
			virtual ~IReferencable(){}

			inline void grab() const{++referenced;}

			inline bool drop() const
			{
				YON_DEBUG_BREAK_IF(referenced<=0);

				--referenced;
				if(referenced==0){
					delete this;
					return true;
				}

				return false;
			}

			s32 getReferenced() const{
				return referenced;
			}
		private:
			mutable s32 referenced;
		};
	}
}

#endif