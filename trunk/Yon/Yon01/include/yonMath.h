#ifndef _YON_CORE_MATH_H_
#define _YON_CORE_MATH_H_

#include "yonTypes.h"
#include <math.h>

namespace yon{
namespace core{

//32λ
#ifdef PI
#undef PI
#endif

const f32 PI=3.1415926535897932384626433832795f;
const f32 RECIPROCAL_PI	= 1.0f/PI;
const f32 HALF_PI	= PI/2.0f;

//�ӽǶ�תΪ����
const f32 DEGTORAD = PI / 180.0f;
//�ӻ���תΪ�Ƕ�
const f32 RADTODEG   = 180.0f / PI;

}//core
}//yon

#endif