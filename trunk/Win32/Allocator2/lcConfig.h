#ifndef _LUCID_CONFIG_H_
#define _LUCID_CONFIG_H_

#include "lcPrepare.h"

//refer to:http://holos.googlecode.com/svn/trunk/src/h_Defines.h
//refer to:http://forge.voodooprojects.org/svn/chameleon/branches/cparm/i386/modules/include/TargetConditionals.h
//refer to:http://msdn.microsoft.com/en-us/library/b0084kay%28v=vs.80%29.aspx
//refer to:http://markmail.org/message/5ekhfztchs45lz3n#query:+page:1+mid:jfqgzrmwm37uyrt6+state:results
//WinRT: define a simple WINRT define than WINAPI_FAMILY_APP
//refer to:http://patches.videolan.org/patch/913/
//#define __linux__
//refer to:http://www.gossamer-threads.com/lists/linux/kernel/30965

// version
// -----------------------------------------------------------------------------
#define LC_VERSION_MAJOR   1
#define LC_VERSION_MINOR   0
#define LC_VERSION_PATCH   0


// exception (disable it with LC_WITHOUT_EXCEPTION)
// -----------------------------------------------------------------------------
#ifndef LC_WITHOUT_EXCEPTION
#define LC_WITH_EXCEPTION
#endif


// track (debug: true & release: false)(force true with LC_SHOW_TRACK)
// -----------------------------------------------------------------------------
#if !defined(LC_SHOW_TRACK)&&defined(LC_BUILD_DEBUG)
#define LC_SHOW_TRACK
#endif


// memory
// -----------------------------------------------------------------------------
#define LC_MEMORY_MAX_SIZE 83886080	//80M


// obsolete (debug: true & release: false)(force true with LC_SHOW_OBSOLETE)
// -----------------------------------------------------------------------------
#if !defined(LC_SHOW_OBSOLETE)&&defined(LC_BUILD_DEBUG)
#define LC_SHOW_OBSOLETE
#endif


// logger (debug: debg & release: warn)
// It is the priority if you define the follow macros:
// LC_LOG_LEVEL_DEBG: debg
// LC_LOG_LEVEL_INFO: info
// LC_LOG_LEVEL_WARN: warn
// LC_LOG_LEVEL_EROR: eror
// -----------------------------------------------------------------------------
#if	  defined(LC_LOG_LEVEL_DEBG)
#ifdef LC_LOG_LEVEL
#undef LC_LOG_LEVEL
#endif
#define LC_LOG_LEVEL 0
#elif defined(LC_LOG_LEVEL_INFO)
#ifdef LC_LOG_LEVEL
#undef LC_LOG_LEVEL
#endif
#define LC_LOG_LEVEL 1
#elif defined(LC_LOG_LEVEL_WARN)
#ifdef LC_LOG_LEVEL
#undef LC_LOG_LEVEL
#endif
#define LC_LOG_LEVEL 2
#elif defined(LC_LOG_LEVEL_EROR)
#ifdef LC_LOG_LEVEL
#undef LC_LOG_LEVEL
#endif
#define LC_LOG_LEVEL 3
#else
#ifdef LC_BUILD_DEBUG
#ifdef LC_LOG_LEVEL
#undef LC_LOG_LEVEL
#endif
#define LC_LOG_LEVEL 0
#else
#ifdef LC_LOG_LEVEL
#undef LC_LOG_LEVEL
#endif
#define LC_LOG_LEVEL 2
#endif
#endif


#endif