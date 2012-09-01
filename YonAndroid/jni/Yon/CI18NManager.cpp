#include "CI18NManager.h"
#include "EncodeConvertor.h"
#include "ILogger.h"

//#pragma comment(lib, "libiconv/IconvLib-1.11.1.lib")

namespace yon{
namespace i18n{

	core::stringc CI18NManager::convert(const c8* str,ENUM_ENCODING from, ENUM_ENCODING to)
	{
		if(from==to)
			return core::stringc(str);
		
		if(from==ENUM_ENCODING_GB18030&&to==ENUM_ENCODING_UTF8)
			return EncodeConvertor::getInstance().GB18030ToUTF8(str);
		else if(from==ENUM_ENCODING_UTF8&&to==ENUM_ENCODING_GB18030)
			return EncodeConvertor::getInstance().UTF8ToGB18030(str);

		Logger->warn(YON_LOG_WARN_FORMAT,"not support convert %d encode to %d encode, do nothing!");
		return core::stringc(str);
	}

	YON_API II18NManager* createI18NManager(){
		return new CI18NManager();
	}
}
}