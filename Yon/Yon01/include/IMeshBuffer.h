#ifndef _YON_SCENE_IMESHBUFFER_H_
#define _YON_SCENE_IMESHBUFFER_H_

#include "IReferencable.h"

namespace yon{
	namespace scene{
		
		class IMeshBuffer : public virtual core::IReferencable{
		public:
			virtual ~IMeshBuffer(){}
		};
	}//scene
}//yon
#endif