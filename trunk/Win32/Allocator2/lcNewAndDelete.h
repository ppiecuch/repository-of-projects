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
//TODO ���operator new ��lc�����ռ������������ȫ�ֵ�new����Ӱ�죬Ϊʲô��
void* operator new(LC_ALLOC_PARAMS(size_t sz));
void operator delete(LC_ALLOC_PARAMS(void* ptr));


#endif