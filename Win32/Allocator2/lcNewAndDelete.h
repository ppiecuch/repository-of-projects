#ifndef _LUCID_NEWANDDELETE_H_
#define _LUCID_NEWANDDELETE_H_

#include "lcTypes.h"
#include "lcException.h"

lcDefineException(OverflowException);
lcDefineException(BadAllocException);


#ifdef LC_SHOW_TRACK
void* operator new(size_t sz);
void operator delete(void* ptr);
#endif
//TODO 如果operator new 在lc命名空间中声明，则对全局的new不做影响，为什么？
void* operator new(LC_ALLOC_PARAMS(size_t sz));
void operator delete(LC_ALLOC_PARAMS(void* ptr));


#endif