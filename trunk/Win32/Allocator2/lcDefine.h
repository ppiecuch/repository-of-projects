#ifndef _LUCID_DEFINE_H_
#define _LUCID_DEFINE_H_

#include "LCConfig.h"

//refer to:http://svn.bgmod.com/code/trunk/public/tier0/platform.h
//弱符号与强符号
//refer to:http://book.51cto.com/art/200904/121029.htm

//链接器就会按如下规则处理与选择被多次定义的全局符号：
//规则1：不允许强符号被多次定义（即不同的目标文件中不能有同名的强符号）；如果有多个强符号定义，则链接器报符号重复定义错误。
//规则2：如果一个符号在某个目标文件中是强符号，在其他文件中都是弱符号，那么选择强符号。
//规则3：如果一个符号在所有目标文件中都是弱符号，那么选择其中占用空间最大的一个。
//比如目标文件A定义全局变量global为int型，占4个字节；目标文件B定义global为double型，占8个字节，那么目标文件A和B链接后，
//符号global占8个字节（尽量不要使用多个不同类型的弱符号，否则容易导致很难发现的程序错误）。

#if defined(LC_OS_WINNT) || defined(LC_OS_WINRT)
#define LC_SELECTANY __declspec(selectany)
#elif defined(LC_OS_MAC) || defined(LC_OS_IOS) || defined(LC_OS_ANDROID) || defined(LC_OS_LINUX)
#define LC_SELECTANY __attribute__((weak))
#else
#define LC_SELECTANY static
#endif

//refer to:http://eeepage.info/gcc-marco-01/
#define LC_FILE __FILE__
#define LC_FUNC __FUNCTION__
//#define LC_FUNC __func__
#define LC_LINE __LINE__

//refer to:http://code.taobao.org/p/screagame/src/trunk/src/core/ScreaTypes.h
#if defined(_DEBUG)
#    if defined(_WINDOWS_API_) && defined(_MSC_VER) && !defined (_WIN32_WCE)
#        if defined(WIN64) || defined(_WIN64) // 兼容x64配置
#            include <crtdbg.h>
#            define LC_DEBUG_BREAK_IF(_CONDITION_) if (_CONDITION_) {_CrtDbgBreak();}
#        else
#            define LC_DEBUG_BREAK_IF(_CONDITION_) if (_CONDITION_) {_asm int 3}
#        endif
#    else
#        include <assert.h>
#        define LC_DEBUG_BREAK_IF(_CONDITION_) assert( !(_CONDITION_) );
#    endif
#else
#    define LC_DEBUG_BREAK_IF(_CONDITION_)
#endif


#endif