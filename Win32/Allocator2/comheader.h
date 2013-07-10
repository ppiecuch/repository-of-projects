#if defined( DEBUG_MEMORY )

#ifndef Header_h
#define Header_h 1

#include <iostream>
#include <assert.h>
#include <math.h>  
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#if defined LINUX
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pwd.h>
#include <sys/wait.h>
#elif defined WIN32
#include <windows.h>
#endif // LINUX

#include "memdbg/macrodef.h"

#endif

#endif // DEBUG_MEMORY
