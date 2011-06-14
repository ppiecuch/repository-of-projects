#ifndef _MATH_UTIL_H_
#define _MATH_UTIL_H_
#include <math.h>

/*************************************************************
						inline系列
**************************************************************/


/**
 *
 * 返回比num大的，并且是最接近num的2的次方的数
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
