#include "EncodeConvertor.h"

#ifdef YON_COMPILE_WITH_WIN32
#include <windows.h>
#include "iconv.h"
#elif defined(YON_COMPILE_WITH_ANDROID)
#include "iconv.h"
#endif


namespace yon{
namespace i18n{

	EncodeConvertor& EncodeConvertor::getInstance()
	{
		static EncodeConvertor instance;
		return instance;
	}

	core::stringc EncodeConvertor::UTF8ToGB18030(const c8* str){
#ifdef YON_COMPILE_WITH_WIN32
		//utf8->unicode
		s32 u16len = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
		WCHAR* pUnicode = new WCHAR[u16len+1]; 
		memset(pUnicode, 0, (u16len<<1)+2);
		MultiByteToWideChar(CP_UTF8, 0, str, -1, pUnicode, u16len);
		//unicode->gb
		s32 gblen = WideCharToMultiByte(CP_ACP, 0, pUnicode, -1, NULL, 0, NULL, NULL);
		c8* pGb = new c8[gblen+1];
		memset(pGb, 0, gblen+1);
		WideCharToMultiByte(CP_ACP, 0, pUnicode, -1, pGb, gblen, NULL, NULL);
		core::stringc result(pGb);
		delete[] pGb;
		return result;
#elif defined(YON_COMPILE_WITH_ANDROID)
		core::stringc result;
		return result;
#endif
	}

	core::stringc EncodeConvertor::GB18030ToUTF8(const c8* str){
#ifdef YON_COMPILE_WITH_WIN32
		//gb->unicode
		s32 u16len = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
		WCHAR* pUnicode = new WCHAR[u16len+1]; 
		memset(pUnicode, 0, (u16len<<1)+2);
		MultiByteToWideChar(CP_ACP, 0, str, -1, pUnicode, u16len);
		//unicode->utf8
		s32 u8len = WideCharToMultiByte(CP_UTF8, 0, pUnicode, -1, NULL, 0, NULL, NULL);
		c8* pUtf8 = new c8[u8len+1];
		memset(pUtf8, 0, u8len+1);
		WideCharToMultiByte(CP_UTF8, 0, pUnicode, -1, pUtf8, u8len, NULL, NULL);
		core::stringc result(pUtf8);
		delete[] pUtf8;
		return result;
#elif defined(YON_COMPILE_WITH_ANDROID)
		u32 size = 0;
		const c8* p = str;
		do
		{
			++size;
		} while(*p++);

		core::stringc result;
		iconv_t cd;
		cd = iconv_open("utf-8","gb18030");
		if(cd==0)
			return result;
		c8* outbuf=new c8[size];
		memset(outbuf, 0, size);
		const char** pinbuf=&str;
		s32 rc = iconv(cd,pinbuf,(size_t*)&size,&outbuf,(size_t*)&size);
		if (rc == -1)
		{
			delete[] outbuf;
			return result;
		}
		result=outbuf;
		delete[] outbuf;
		iconv_close(cd);
		return result;
#endif
	}
}
}