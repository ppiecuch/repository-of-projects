#ifndef _YON_CORE_MATH_H_
#define _YON_CORE_MATH_H_

#include "yonTypes.h"
#include <math.h>

namespace yon{
namespace core{

//舍入误差
const s32 ROUNDING_ERROR_S32 = 0;
const f32 ROUNDING_ERROR_f32 = 0.000001f;
const f64 ROUNDING_ERROR_f64 = 0.00000001;

//32位
#ifdef PI
#undef PI
#endif

//PI相关定义
const f32 PI=3.1415926535897932384626433832795f;
const f32 PI2=PI+PI;
const f32 RECIPROCAL_PI	= 1.0f/PI;
const f32 HALF_PI	= PI*0.5f;

//从角度转为弧度
const f32 DEGTORAD = PI / 180.0f;
//从弧度转为角度
const f32 RADTODEG   = 180.0f / PI;


inline bool equals(const f64 a, const f64 b, const f64 tolerance = ROUNDING_ERROR_f64)
{
	return (a + tolerance >= b) && (a - tolerance <= b);
}

//! returns if a equals b, taking possible rounding errors into account
inline bool equals(const f32 a, const f32 b, const f32 tolerance = ROUNDING_ERROR_f32)
{
	return (a + tolerance >= b) && (a - tolerance <= b);
}

}//core
}//yon

#endif