#include "EncodeConvertor.h"

#ifdef YON_COMPILE_WITH_WIN32
#include <windows.h>
#include "iconv.h"
#elif defined(YON_COMPILE_WITH_ANDROID)
#include "iconv.h"
#include <string.h>
#endif

#include "ILogger.h"

namespace yon{
namespace i18n{

	EncodeConvertor& EncodeConvertor::getInstance()
	{
		static EncodeConvertor instance;
		return instance;
	}

	u32 count_len(const c8* str)
	{
#if 1
		u32 size = 0;
		const c8* p = str;
		do
		{
			++size;
		} while(*p++);

		return size;
#else
		return strlen(str);
#endif
	}

	bool EncodeConvertor::code_convert(const char *from_charset,const char *to_charset,const char *inbuf,int inlen,char *outbuf,int outlen)
	{
		iconv_t cd;
		const char **pin = &inbuf;
		char **pout = &outbuf;
		size_t iconv_ret;
		cd = iconv_open(to_charset,from_charset);

		if (cd==(iconv_t)-1)
		{
			Logger->warn(YON_LOG_WARN_FORMAT,"iconv_open failed!");
			return false;
		}

		memset(outbuf,0,outlen);
		iconv_ret = iconv(cd,pin,(size_t *)&inlen,pout,(size_t *)&outlen);

		if (iconv_ret == (size_t)-1)
		{
			Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("iconv(%d,%d) failed!",inlen,outlen).c_str());
			return false;
		}

		iconv_close(cd);
		return true;
	}


	core::stringc EncodeConvertor::UTF8ToGB18030(const c8* str){
/*#ifdef YON_COMPILE_WITH_WIN32
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
		delete[] pUnicode;
		return result;
#elif defined(YON_COMPILE_WITH_ANDROID)*/
		u32 size=count_len(str);
		core::stringc result;
		u32 osize=size;
		c8* outbuf=new c8[osize];
		if(code_convert("utf-8","gb18030",str,size,outbuf,osize))
			result=outbuf;
		else
			Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("convert encode(u2g) with:%s(%d) failed!",str,size).c_str());
		delete[] outbuf;
		return result;
//#endif
	}

	core::stringc EncodeConvertor::GB18030ToUTF8(const c8* str){
/*#ifdef YON_COMPILE_WITH_WIN32
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
		delete[] pUnicode;
		return result;
#elif defined(YON_COMPILE_WITH_ANDROID)*/
		u32 size=count_len(str);
		core::stringc result;
		u32 osize=size<<1;
		c8* outbuf=new c8[osize];
		if(code_convert("gb18030","utf-8",str,size,outbuf,osize))
			result=outbuf;
		else
			Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("convert encode(g2u) with:%s(%d) failed!",str,size).c_str());
		delete[] outbuf;
		return result;
//#endif
	}
}
}