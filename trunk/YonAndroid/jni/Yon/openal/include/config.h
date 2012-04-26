#ifndef CONFIG_H
#define CONFIG_H

#include "yonConfig.h"

/* Define to the library version */
#define ALSOFT_VERSION "1.13"

/* Define if we have the ALSA backend */
/* #undef HAVE_ALSA */

/* Define if we have the OSS backend */
/* #undef HAVE_OSS */

/* Define if we have the Solaris backend */
/* #undef HAVE_SOLARIS */

/* Define if we have the DSound backend */
#ifdef YON_COMPILE_WITH_WIN32
#define HAVE_DSOUND
#endif

/* Define if we have the Windows Multimedia backend */
#ifdef YON_COMPILE_WITH_WIN32
#define HAVE_WINMM
#endif

/* Define if we have the PortAudio backend */
/* #undef HAVE_PORTAUDIO */

/* Define if we have the PulseAudio backend */
/* #undef HAVE_PULSEAUDIO */

/* Define if we have the Wave Writer backend */
#ifdef YON_COMPILE_WITH_WIN32
#define HAVE_WAVE
#endif

/* Define if we have dlfcn.h */
#ifdef YON_COMPILE_WITH_ANDROID
#define HAVE_DLFCN_H 1
#endif

/* Define if we have the stat function */
#define HAVE_STAT 1

#ifdef YON_COMPILE_WITH_ANDROID
/* Define if we have the powf function */
#define HAVE_POWF

/* Define if we have the sqrtf function */
#define HAVE_SQRTF

/* Define if we have the acosf function */
#define HAVE_ACOSF

/* Define if we have the atanf function */
#define HAVE_ATANF

/* Define if we have the fabsf function */
#define HAVE_FABSF

/* Define if we have the strtof function */
#define HAVE_STRTOF

/* Define if we have stdint.h */
#define HAVE_STDINT_H
#endif

/* Define if we have the __int64 type */
#ifdef YON_COMPILE_WITH_WIN32
#define HAVE___INT64
#endif


/* Define to the size of a long int type */
#define SIZEOF_LONG 4

/* Define to the size of a long long int type */
#define SIZEOF_LONG_LONG 8

/* Define to the size of an unsigned int type */
#define SIZEOF_UINT 4

/* Define to the size of a void pointer type */
#define SIZEOF_VOIDP 4

#ifdef YON_COMPILE_WITH_ANDROID
/* Define if we have GCC's destructor attribute */
#define HAVE_GCC_DESTRUCTOR

/* Define if we have GCC's format attribute */
#define HAVE_GCC_FORMAT
#endif

/* Define if we have pthread_np.h */
/* #undef HAVE_PTHREAD_NP_H */

/* Define if we have float.h */
#ifdef YON_COMPILE_WITH_WIN32
#define HAVE_FLOAT_H
#endif

/* Define if we have fenv.h */
/* #undef HAVE_FENV_H */

/* Define if we have fesetround() */
/* #undef HAVE_FESETROUND */

/* Define if we have _controlfp() */
#ifdef YON_COMPILE_WITH_WIN32
#define HAVE__CONTROLFP
#endif

/* Define if we have pthread_setschedparam() */
#ifdef YON_COMPILE_WITH_ANDROID
#define HAVE_PTHREAD_SETSCHEDPARAM
#endif

#endif
