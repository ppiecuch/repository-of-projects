#ifndef _MATH_UTIL_H_
#define _MATH_UTIL_H_
#include <math.h>

/*************************************************************
						inlineϵ��
**************************************************************/


/**
 *
 * ���ر�num��ģ���������ӽ�num��2�Ĵη�����
 */
inline int ceilPower(const int& num)
{
	int rval=1;
	rval*=2;
	while(rval<num)
		rval<<=1;
	return rval;
}

#endif
