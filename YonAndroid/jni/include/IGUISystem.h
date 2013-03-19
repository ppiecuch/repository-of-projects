#ifndef _YON_GUI_IGUISYSTEM_H_
#define _YON_GUI_IGUISYSTEM_H_

#include "IReferencable.h"
#include "IRenderable.h"
#include "IResizable.h"
#include "path.h"

namespace yon{
namespace gui{

	class ITheme;

	class IGUISystem : public virtual core::IReferencable,public virtual core::IResizable,public virtual core::IRenderable{
	public:
		virtual ~IGUISystem(){}

		virtual bool bindTheme(ITheme* theme) = 0;
		virtual ITheme* getBindedTheme() = 0;

		virtual ITheme* getTheme(const io::path& name) = 0;
		virtual bool removeTheme(ITheme* theme) = 0;
	};
}
}
#endif