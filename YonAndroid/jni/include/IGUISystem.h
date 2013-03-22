#ifndef _YON_GUI_IGUISYSTEM_H_
#define _YON_GUI_IGUISYSTEM_H_

#include "IRenderable.h"
#include "IResizable.h"
#include "path.h"
#include "IWidget.h"

namespace yon{
namespace gui{

	class ITheme;
	class IWidget;

	class IGUISystem : public core::IResizable,public core::IRenderable,public IWidget{
	public:
		IGUISystem(const core::stringc& id);
		virtual ~IGUISystem(){}

		virtual bool bindTheme(ITheme* theme) = 0;
		virtual ITheme* getBindedTheme() = 0;

		virtual ITheme* getTheme(const io::path& name) = 0;
		virtual bool removeTheme(ITheme* theme) = 0;
	};

	IGUISystem::IGUISystem(const core::stringc& id)
		:IWidget(IWidget::NONE,NULL,NULL,id,core::ZERO_RECTI)
	{

	}
}
}
#endif