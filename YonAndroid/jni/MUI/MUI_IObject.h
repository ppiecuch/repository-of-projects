#ifndef _MUI_IOBJECT_H_
#define _MUI_IOBJECT_H_

#include "yon.h"
using namespace yon;

namespace mui{

	class IObject{
		YON_RTTI_BASE(IObject)
	public:
		IObject() {}
		virtual ~IObject() {}
	};
}
#endif