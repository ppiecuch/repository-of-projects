#ifndef _MUI_IBASEWIDGET_H_
#define _MUI_IBASEWIDGET_H_

#include "MUI_ICroppedRetangle.h"

namespace mui{

	class IBaseWidget : ICroppedRetangle{
	public:

		//virtual void doRender() = 0;
		virtual void preRender() = 0;
	};

}
#endif