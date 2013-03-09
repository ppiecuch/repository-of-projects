#ifndef _YON_GUI_IGUISYSTEM_H_
#define _YON_GUI_IGUISYSTEM_H_

#include "IReferencable.h"
#include "IRenderable.h"
#include "IResizable.h"

namespace yon{
namespace gui{

	class IGUISystem : public virtual core::IReferencable,public virtual core::IResizable,public virtual core::IRenderable{
	public:
		virtual ~IGUISystem(){}
	};
}
}
#endif