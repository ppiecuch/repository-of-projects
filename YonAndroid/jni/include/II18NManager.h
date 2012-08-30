#ifndef _YON_I18N_II18NMANAGER_H_
#define _YON_I18N_II18NMANAGER_H_

#include "IReferencable.h"
#include "yonString.h"

namespace yon{
namespace i18n{

	class II18NManager:public virtual core::IReferencable{
	public:

		virtual core::stringc convert(const c8* str,ENUM_ENCODING from, ENUM_ENCODING to) = 0;
	};

	extern "C" YON_API II18NManager* createI18NManager();
}
}
#endif