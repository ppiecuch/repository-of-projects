#ifndef _LUCID_PREPARE_H_
#define _LUCID_PREPARE_H_

#include <stdio.h>

/**
*  LC_STRING makes a string of X where X is expanded before conversion to a string
*  if X itself contains macros.
*/
#define LC_STRING(X) LC_STRING_IMPL(X)
#define LC_STRING_IMPL(X) #X

/**
*  LC_CONCAT concatenates X and Y  where each is expanded before
*  contanenation if either contains macros.
*/
#define LC_CONCAT(X,Y) LC_CONCAT_IMPL(X,Y)
#define LC_CONCAT_IMPL(X,Y) X##Y



// architecture
#if defined (__i386__) || defined(_X86_) || defined(__X86__) || defined(_M_IX86)
#define LC_ARCH_32
#elif defined (__x86_64) || defined (_M_X64) || defined (__powerpc64__) || (defined __alpha__) || (defined __ia64__) || defined (__s390__) || defined (__s390x__)
#define LC_ARCH_64
#else
#error unrecognized architecture
#endif


// operation system
#if defined(_WIN32)
#if defined( WINAPI_FAMILY_APP )
#define LC_OS_WINRT
#else
#define LC_OS_WINNT
#endif
#elif defined(__GNUC__) && defined(__MACOS_CLASSIC__)
#define LC_OS_MAC
#elif defined(__GNUC__) && (defined(__APPLE_CPP__) || defined(__APPLE_CC__))
#define LC_OS_IOS
#elif defined(__ANDROID__)
#define LC_OS_ANDROID
#elif defined(__linux__)
#define LC_OS_LINUX
#else
#error unrecognized platform
#endif

// compiler
//refer to:http://hi.baidu.com/_jiangming/item/9789df1c807aec9f89a95667
//refer to:MyGUI
/*
介绍预定义宏 "_MSC_VER"
1、_MSC_VER是微软C/C++编译器——cl.exe 编 译代码时预定义的一个宏。需要针对cl编写代码时， 可以使用该宏进行条件编译。
2、_MSC_VER的值表示cl的版本。需要针对cl特定版本编写代码时， 也可以使用该宏进行条件编译。
3、_MSC_VER的类型是"int"，具体版本号定义如下：

MS VC++ 11.0 _MSC_VER = 1700 (Visual Studio 2012)
MS VC++ 10.0 _MSC_VER = 1600 (Visual Studio 2010)
MS VC++ 9.0 _MSC_VER = 1500 (Visual Studio 2008)
MS VC++ 8.0 _MSC_VER = 1400 (Visual Studio 2005)
MS VC++ 7.1 _MSC_VER = 1310 (Visual Studio 2003)
MS VC++ 7.0 _MSC_VER = 1300
MS VC++ 6.0 _MSC_VER = 1200
MS VC++ 5.0 _MSC_VER = 1100

介绍预定义宏“__GNUC__”
1、__GNUC__ 是gcc编译器编译代码时预定义的一个宏。需要针对gcc编写代码时， 可以使用该宏进行条件编译。
2、__GNUC__ 的值表示gcc的版本。需要针对gcc特定版本编写代码时，也可以使用该宏进行条件编译。
3、__GNUC__ 的类型是“int”
*/
#ifdef _MSC_VER
#define LC_CMPL_MSVC
#define LC_CMPL_VERSION _MSC_VER
#elif defined(__GNUC__)
#define LC_CMPL_GCC
#define LC_CMPL_VERSION (((__GNUC__)*100) + \
	(__GNUC_MINOR__*10) + \
	__GNUC_PATCHLEVEL__)
#else
#error unrecognized compiler
#endif



//standard
//refer to:http://www.cse.psu.edu/~dheller/cmpsc311/Lectures/Standards/CStandard/CStandard.c
/* Which version of the C standard are we using?
* Print some information to stdout.
*
* Mind the indentation!
*
* The following are required in the C11 Standard (mandatory macros).
*   __STDC__				C89		C99		C11
*   __STDC_HOSTED__					C99		C11
*   __STDC_VERSION__		(C94)	C99		C11
* The following are optional in the C11 Standard (environment macros).
*   __STDC_ISO_10646__				C99		C11
*   __STDC_MB_MIGHT_NEQ_WC__		C99		C11
*   __STDC_UTF_16__							C11
*   __STDC_UTF_32__							C11
* The following are optional in the C11 Standard (conditional feature macros).
*   __STDC_ANALYZABLE__						C11
*   __STDC_IEC_559__				C99		C11
*   __STDC_IEC_559_COMPLEX__		C99		C11
*   __STDC_LIB_EXT1__						C11
*   __STDC_NO_ATOMICS__						C11
*   __STDC_NO_COMPLEX__						C11
*   __STDC_NO_THREADS__						C11
*   __STDC_NO_VLA__							C11
*
* The following are required in the C11 Standard (mandatory macros).
*   __DATE__				C89		C99		C11
*   __FILE__				C89		C99		C11
*   __LINE__				C89		C99		C11
*   __TIME__				C89		C99		C11
*/

#if defined (__cplusplus)
#define LC_STD_CPP
#elif defined(__STDC__)
#define LC_STD_C
#else
#error unrecognized language standard
#endif


//build
#if defined(DEBUG) || defined(_DEBUG)
#define LC_BUILD_DEBUG
#else
#define LC_BUILD_RELEASE
#endif

//NULL
//refer to:http://en.sourceforge.jp/projects/iutest/scm/svn/blobs/235/trunk/include/internal/iutest_internal_defs.hpp
#ifndef NULL
#  ifdef			LC_STD_CPP
#    define NULL    0
#  else
#    define NULL    (void*)0
#  endif
#endif

//refer to:http://hi.baidu.com/hj11yc/item/7b84fc962c96cdd11b49dfb4
//refer to:http://szsu.wordpress.com/2011/07/28/portable_tls_qualifier/
//可移植的静态线程局部存储修饰符
//TLS是一种为每个线程分配自己局部数据的机制。
//该属性只能用于数据或不含成员函数的类的声明和定义，不能用于函数的声明和定义。
//该属性的使用可能会影响DLL的延迟载入。
//该属性只能用于静态数据，包括全局数据对象(static和extern)，局部静态对象，类的静态数据成员；不能用于自动数据对象。
//该属性必须同时用于数据的声明和定义，不管它的声明和定义是在一个文件还是多个文件。
//__declspec(thread)不能用作类型修饰符。
//如果在类声明的同时没有定义对象，则__declspec(thread)将被忽略
#if defined(LC_CMPL_GCC)
#define LC_THREAD_LOCAL __thread
#elif defined(LC_CMPL_MSVC)
#define LC_THREAD_LOCAL __declspec(thread)
#else
#pragma message("Warning: LC_THREAD_LOCAL is not supported by this compiler")
#define LC_THREAD_LOCAL
#endif

//refer to:MyGUI
#ifdef LC_CMPL_MSVC
#	if LC_CMPL_VERSION < 1310 // VC++ 7.1
#		define LC_OBSOLETE_START(text)
#		define LC_OBSOLETE_END
#	else
#		define LC_OBSOLETE_START(text) __declspec(deprecated(text))
#		define LC_OBSOLETE_END
#	endif
#elif defined(LC_CMPL_GCC)
#	if LC_CMPL_VERSION < 310 // gcc 3.1
#		define LC_OBSOLETE_START(text)
#		define LC_OBSOLETE_END
#	else
#		define LC_OBSOLETE_START(text)
#		define LC_OBSOLETE_END __attribute__((deprecated))
#	endif
#else
#	define LC_OBSOLETE_START(text)
#	define LC_OBSOLETE_END
#endif


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
#if defined(LC_OS_WINNT) || defined(LC_OS_WINRT)
#define LC_FUNC __FUNCTION__
#else
#define LC_FUNC __func__
#endif
#define LC_LINE __LINE__

//refer to:http://code.taobao.org/p/screagame/src/trunk/src/core/ScreaTypes.h
#if defined(LC_BUILD_DEBUG)
#    if defined(LC_OS_WINNT) || defined(LC_OS_WINRT)
#        if defined(LC_ARCH_64) // 兼容x64配置
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
