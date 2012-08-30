#ifndef _YON_I18N_II18NMANAGER_H_
#define _YON_I18N_II18NMANAGER_H_

#include "IReferencable.h"
#include "yonString.h"

namespace yon{
namespace i18n{
	enum ENUM_ENCODE{
		ENUM_ENCODE_UTF8 = 0,
		ENUM_ENCODE_GB18030,

		ENUM_ENCODE_COUNT
	};
	class II18NManager:public virtual core::IReferencable{
	public:

		virtual core::stringc convert(const c8* str,ENUM_ENCODE from, ENUM_ENCODE to) = 0;
	};

	extern "C" YON_API II18NManager* createI18NManager();
}
}
#endif