#ifndef _FBKernal_Assert_h_
#define _FBKernal_Assert_h_

#include <windows.h>
#include <stdio.h>
#include <signal.h>
#include <io.h>

#include <string>
#include <algorithm>
using namespace std;

#ifdef _DEBUG
	#define	FBKernal_Assert(exp)\
		if(!(exp))\
		{\
			char msg[256];\
			sprintf(\
				msg,\
				"FBKernal_Assert(%s);\nFile: %s\nLine: %d",\
				#exp,\
				__FILE__,\
				__LINE__);\
			FBKernal_MessageBox(msg);\
		}
#else
	#define	FBKernal_Assert(exp)\
		(void)0;
#endif

#define FBKernal_MessageBox(msg)\
	switch(MessageBoxA(GetActiveWindow(), msg, "FBKernal MessageBox", MB_ABORTRETRYIGNORE))\
	{\
	case IDABORT:\
		raise(SIGABRT);\
		exit(3);\
		break;\
	case IDRETRY:\
		_asm\
		{\
			int 3\
		}\
		break;\
	case IDIGNORE:\
		ShowWindow(GetActiveWindow(), SW_SHOW);\
		break;\
	};

#endif