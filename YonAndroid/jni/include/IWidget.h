#ifndef _YON_GUI_IWIDGET_H_
#define _YON_GUI_IWIDGET_H_

#include "IReferencable.h"
#include "yonString.h"
#include "IGUISystem.h"

//refer to:http://www.php100.com/manual/css3_0

namespace yon{
namespace gui{

	class IWidget : public virtual core::IReferencable,public virtual event::IEventReceiver{
	public:
		//const core::stringc& getTag() const{
			//TODO
		//}

		//virtual bool isVisible() = 0;

		virtual const core::stringc& getId() const = 0;

		virtual IWidget* getParent() const = 0;

		virtual IGUISystem* getGUISystem() const = 0;
	};
}
}
#endif