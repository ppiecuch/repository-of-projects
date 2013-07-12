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


// exception
// -----------------------------------------------------------------------------
#define LC_WITH_EXCEPTION


// track (debug: true & release: false)
// -----------------------------------------------------------------------------
#if !defined(LC_SHOW_TRACK)&&defined(LC_BUILD_DEBUG)
#define LC_SHOW_TRACK
#endif


// memory
// -----------------------------------------------------------------------------
#define LC_MEMORY_MAX_SIZE 83886080	//80M


// obsolete (debug: true & release: false)
// -----------------------------------------------------------------------------
#if !defined(LC_SHOW_OBSOLETE)&&defined(LC_BUILD_DEBUG)
#define LC_SHOW_OBSOLETE
#endif

#endif