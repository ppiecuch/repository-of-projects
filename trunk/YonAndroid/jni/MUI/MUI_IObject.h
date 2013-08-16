#ifndef _MUI_IOBJECT_H_
#define _MUI_IOBJECT_H_

#include "MUI_Prepare.h"

namespace mui{

	class MUI_API IObject{
		YON_RTTI_BASE(IObject)
	public:
		IObject() {}
		virtual ~IObject() {}
	};
}
#endif