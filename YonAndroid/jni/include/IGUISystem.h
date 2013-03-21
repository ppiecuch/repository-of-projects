#ifndef _YON_GUI_IGUISYSTEM_H_
#define _YON_GUI_IGUISYSTEM_H_

#include "IReferencable.h"
#include "IRenderable.h"
#include "IResizable.h"
#include "IEventReceiver.h"
#include "path.h"

namespace yon{
namespace gui{

	class ITheme;
	class IWidget;

	class IGUISystem : public virtual core::IReferencable,public core::IResizable,public core::IRenderable,public event::IEventReceiver{
	public:
		virtual ~IGUISystem(){}

		virtual bool bindTheme(ITheme* theme) = 0;
		virtual ITheme* getBindedTheme() = 0;

		virtual ITheme* getTheme(const io::path& name) = 0;
		virtual bool removeTheme(ITheme* theme) = 0;

		virtual IWidget* getRootWidget() = 0;
	};
}
}
#endif