#ifndef _LUCID_CONFIG_H_
#define _LUCID_CONFIG_H_

#include <stdio.h>

//refer to:http://holos.googlecode.com/svn/trunk/src/h_Defines.h
//refer to:http://forge.voodooprojects.org/svn/chameleon/branches/cparm/i386/modules/include/TargetConditionals.h
//refer to:http://msdn.microsoft.com/en-us/library/b0084kay%28v=vs.80%29.aspx
//refer to:http://markmail.org/message/5ekhfztchs45lz3n#query:+page:1+mid:jfqgzrmwm37uyrt6+state:results
//WinRT: define a simple WINRT define than WINAPI_FAMILY_APP
//refer to:http://patches.videolan.org/patch/913/
//#define __linux__
//refer to:http://www.gossamer-threads.com/lists/linux/kernel/30965

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

// version
// -----------------------------------------------------------------------------
#define LC_VERSION_MAJOR   1
#define LC_VERSION_MINOR   0
#define LC_VERSION_PATCH   0

// version to string
// -----------------------------------------------------------------------------

#define LC_VERSION_STRING  \
	LC_TO_STR(LC_VERSION_MAJOR)"." \
	LC_TO_STR(LC_VERSION_MINOR)"." \
	LC_TO_STR(LC_VERSION_PATCH)

#define LC_HOLOS_TEXT \
	"powered by Holos v" LC_VERSION_STRING " (http://lucid.googlecode.com/)"


// architecture
// -----------------------------------------------------------------------------
#if defined (__i386__) || defined(_X86_) || defined(__X86__) || defined(_M_IX86)
#define LC_ARCH_32
#elif defined (__x86_64) || defined (_M_X64) || defined (__powerpc64__) || (defined __alpha__) || (defined __ia64__) || defined (__s390__) || defined (__s390x__)
#define LC_ARCH_65
#else
#error unrecognized architecture
#endif


// operation system
// -----------------------------------------------------------------------------
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

#endif