#ifndef _YON_GUI_ISKIN_H_
#define _YON_GUI_ISKIN_H_

#include "IReferencable.h"
#include "rect.h"

namespace yon{
	namespace video{
		class ITexture;
	}
namespace gui{

	class ISkin : public virtual core::IReferencable{
	public:

		virtual video::ITexture* getTexture() = 0;
		virtual const core::rectf& getPositions() = 0;

		//TODO color
	};
}
}
#endif