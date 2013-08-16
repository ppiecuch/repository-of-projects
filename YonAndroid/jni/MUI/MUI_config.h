#ifndef _MUI_CONFIG_H_
#define _MUI_CONFIG_H_


// Definition of platforms
#define MUI_PLATFORM_WIN32		1
#define MUI_PLATFORM_ANDROID	2
#define MUI_PLATFORM_APPLE		3


// Definition of compilers
#define MUI_COMPILER_WITH_MSVC	1
#define MUI_COMPILER_WITH_GNUC	2


//refer to:https://groups.google.com/forum/?fromgroups#!topic/android-ndk/yTsM7cam_dk
// Find platform
#if defined (__WIN32__) || defined (_WIN32)
#	define MUI_PLATFORM MUI_PLATFORM_WIN32
#elif defined (ANDROID) || defined (__ANDROID__)
#	define MUI_PLATFORM MUI_PLATFORM_ANDROID
#elif defined (__APPLE_CC__)
#	define MUI_PLATFORM MUI_PLATFORM_APPLE
#else
#	pragma error "Unknown platform! Stop building!!!"
#endif


// Find compiler
#if defined( _MSC_VER )
#	define MUI_COMPILER MUI_COMPILER_WITH_MSVC
#	define MUI_COMP_VER _MSC_VER

#elif defined( __GNUC__ )
#	define MUI_COMPILER MUI_COMPILER_WITH_GNUC
#	define MUI_COMP_VER (((__GNUC__)*100) + \
	(__GNUC_MINOR__*10) + \
	__GNUC_PATCHLEVEL__)
#else
#	pragma error "Unknown compiler! Stop building!!!"
#endif



//TODO
/*
// See if we can use __forceinline or if we need to use __inline instead
#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC
#	if MYGUI_COMP_VER >= 1200
#		define MYGUI_FORCEINLINE __forceinline
#	endif
#elif defined(__MINGW32__)
#	if !defined(MYGUI_FORCEINLINE)
#		define MYGUI_FORCEINLINE __inline
#	endif
#else
#	define MYGUI_FORCEINLINE __inline
#endif
*/



// Windows settings
#if MUI_PLATFORM == MUI_PLATFORM_WIN32
#
#	ifdef MUI_BUILD
#		define MUI_API __declspec(dllexport)
#	else
#		define MUI_API __declspec(dllimport)
#	endif
#// Win32 compilers use _DEBUG for specifying debug builds.
#	ifdef _DEBUG
#		define MUI_DEBUG_MODE 1
#	endif
#endif



// Android/Apple Settings
#if MUI_PLATFORM == MUI_PLATFORM_ANDROID || MUI_PLATFORM == MUI_PLATFORM_APPLE
#
// Add -fvisibility=hidden to compiler options. With -fvisibility=hidden, you are telling
// GCC that every declaration not explicitly marked with a visibility attribute (MUI_API)
// has a hidden visibility (like in windows).
#	if __GNUC__ >= 4
#		define MUI_API  __attribute__ ((visibility("default")))
#	else
#		define MUI_API
#	endif
#
#
// Unlike the Win32 compilers, Linux compilers seem to use DEBUG for when
// specifying a debug build.
// (??? this is wrong, on Linux debug builds aren't marked in any way unless
// you mark it yourself any way you like it -- zap ???)
#	ifdef DEBUG
#		define MUI_DEBUG_MODE 1
#	endif

#endif

#endif