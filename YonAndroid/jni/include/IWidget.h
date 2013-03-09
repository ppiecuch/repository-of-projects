#ifndef _YON_GUI_IWIDGET_H_
#define _YON_GUI_IWIDGET_H_

#include "IReferencable.h"
#include "yonString.h"

//refer to:http://www.php100.com/manual/css3_0

namespace yon{
namespace gui{

	class IWidget : public virtual core::IReferencable{
	public:
		const core::stringc& getTag() const{
			//TODO
		}

		const core::stringc& getId() const{
			//TODO
		}

		IWidget* getParent() const{
			//TODO
		}
	};
}
}
#endif