#ifndef _LUCID_CONFIG_H_
#define _LUCID_CONFIG_H_

#include <stdio.h>
#include "lcDefine.h"

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

// version to string
// -----------------------------------------------------------------------------

#define LC_VERSION_STRING  \
	LC_TO_STR(LC_VERSION_MAJOR)"." \
	LC_TO_STR(LC_VERSION_MINOR)"." \
	LC_TO_STR(LC_VERSION_PATCH)

#define LC_HOLOS_TEXT \
	"powered by Holos v" LC_VERSION_STRING " (http://lucid.googlecode.com/)"


// exception
// -----------------------------------------------------------------------------
#define LC_WITH_EXCEPTION

// track
// -----------------------------------------------------------------------------
#define LC_TRACK_DETAIL

#if defined(LC_BUILD_RELEASE)&&!defined(LC_FORCE_TRACK_DETAIL)
#undef LC_TRACK_DETAIL
#endif


// memory
// -----------------------------------------------------------------------------
#define LC_MEMORY_MAX_SIZE 83886080	//80M

#endif