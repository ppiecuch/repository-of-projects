#ifndef _YON_I18N_CI18NMANAGER_H_
#define _YON_I18N_CI18NMANAGER_H_

#include "II18NManager.h"

namespace yon{
namespace i18n{

	class CI18NManager:public II18NManager{
	public:
		virtual core::stringc convert(const c8* str,ENUM_ENCODING from, ENUM_ENCODING to);
	};
}
}
#endif