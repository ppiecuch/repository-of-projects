#ifndef _YON_CORE_IRENDERABLE_H_
#define _YON_CORE_IRENDERABLE_H_

#include "IReferencable.h"
#include "IVideoDriver.h"

namespace yon{
	namespace core{

		class IRenderable : public virtual IReferencable{
		public:
			virtual void render(video::IVideoDriver* driver) = 0;
		};
	}
}
#endif