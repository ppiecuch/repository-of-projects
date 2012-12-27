#ifndef _YON_CORE_MATH_H_
#define _YON_CORE_MATH_H_

#include "yonTypes.h"
#include <math.h>
#include <float.h> //For FLT_MAX
#include <limits.h> // For INT_MAX / UINT_MAX

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
const f64 PI64= 3.1415926535897932384626433832795028841971693993751;
const f32 PI2=PI+PI;
const f32 RECIPROCAL_PI	= 1.0f/PI;
const f32 HALF_PI	= PI*0.5f;

//从角度转为弧度
const f32 DEGTORAD = PI / 180.0f;
//从弧度转为角度
const f32 RADTODEG   = 180.0f / PI;
const f64 RADTODEG64 = 180.0 / PI64;

//refer to:http://bbs.chinaunix.net/thread-3746530-1-1.html
//ANSI/IEEE Std 754-1985标准
//IEEE 754是最广泛使用的二进制浮点数算术标准，被许多CPU与浮点运算器所采用。
//IEEE 754规定了多种表示浮点数值的方式，在本文档里只介绍32bits的float浮点类型。
//它被分为3个部分，分别是符号位S（sign bit）、指数偏差E（exponent bias）和小数部分F（fraction）。
//| 1bit |   8bit  |        23bit        |
//|   S  |    E    |          F          |
//S位占1bit，为bit31。S位为0代表浮点数是正数，S位为1代表浮点数是负数，
//比如说0x449A522C的S位为0，表示这是一个正数，0x849A522C的S位为1，表示这是一个负数。
//E位占8bits，为bit23~bit30。E位代表2的N次方，但需要减去127，
//比如说E位为87，那么E位的值为2^（87-127）=9.094947017729282379150390625e-13。
//F位占23bits，为bit0~bit22。F位是小数点后面的位数，其中bit22是2-1=0.5，bit21是2-2=0.25，
//以此类推，bit0为2-23=0.00000011920928955078125。但F位里隐藏了一个1，
//也就是说F位所表示的值是1+（F位bit22~bit0所表示的数值），比如说F位是0b10100000000000000000001，
//只有bit22、bit20和bit0为1，那么F位的值为1+(2-1+2-3+2-23)，为1.62500011920928955078125。
// 综上所述，从二进制数换算到浮点数的公式为：(-1)S*2^(E-127)*(1+F)。
//但还有几个特殊的情形：
//1、若E位为0并且F位也为0时表示浮点数0，此时浮点数受S位影响，表现出+0和-0两种0，但数值是相等的。
//比如二进制数0x00000000表示+0，二进制数0x80000000表示-0。
//2、若E位为0并且F位不为0时浮点数为(-1)S×2^(-126)×F，注意，E位的指数是-126，而不是0-127=-127，
//而且F位是0.xx格式而不是1.xx格式，比如0x00000001的浮点数为2^(-126)*2^(-23)=1.4012984643248170709237295832899e-45。
//一旦E为不为0，从0变为1，不是增加2倍的关系，因为公式改变了。
//3、若E位为255并且F位不为0时表示非数值，也就是说是非法数，例如0x7F800001。
//4、若E位为255并且F位为0时表示无穷大的数，此时浮点数受S位影响，例如0x7F800000表示正无穷大，0xFF800000表示负无穷大。
//当我们使用1个数除以0时，结果将被记作0x7F800000。
/*
float IEEE-754 bit represenation

+0      0x00000000
-0		0x80000000
1.0    0x3f800000
0.5    0x3f000000
3      0x40400000
+inf   0x7f800000
-inf   0xff800000
+NaN   0x7fc00000 or 0x7ff00000
in general: number = (sign ? -1:1) * 2^(exponent) * 1.(mantissa bits)
*/
typedef union { u32 u; s32 s; f32 f; } _int_float_;

#define F32_AS_S32(f)		(*((s32 *) &(f)))
#define F32_AS_U32(f)		(*((u32 *) &(f)))
#define F32_AS_U32_POINTER(f)	( ((u32 *) &(f)))

//! code is taken from IceFPU
//! Integer representation of a floating-point value.
#ifdef USE_FAST_MATH
#define IR(x)                           ((u32&)(x))
#else
inline u32 IR(f32 x) {_int_float_ tmp; tmp.f=x; return tmp.u;}
#endif

//! Absolute integer representation of a floating-point value
#define AIR(x)				(IR(x)&0x7fffffff)

//! returns abs of two values. Own implementation to get rid of STL (VS6 problems)
template<class T>
inline T abs_(const T& a)
{
	return a <(T)0? -a : a;
}
template<class T>
inline bool between(const T& n, const T& a, const T& b)
{
	return n>=a&&n<=b;
}
//! returns linear interpolation of a and b with ratio t
//! \return: a if t==0, b if t==1, and the linear interpolation else
template<class T>
inline T lerp(const T& a, const T& b, const f32 t)
{
	return (T)(a*(1.f-t)) + (b*t);
}

//! returns minimum of two values. Own implementation to get rid of the STL (VS6 problems)
template<class T>
inline const T& min_(const T& a, const T& b)
{
	return a < b ? a : b;
}

//! returns minimum of three values. Own implementation to get rid of the STL (VS6 problems)
template<class T>
inline const T& min_(const T& a, const T& b, const T& c)
{
	return a < b ? min_(a, c) : min_(b, c);
}

//! returns maximum of two values. Own implementation to get rid of the STL (VS6 problems)
template<class T>
inline const T& max_(const T& a, const T& b)
{
	return a < b ? b : a;
}

//! returns maximum of three values. Own implementation to get rid of the STL (VS6 problems)
template<class T>
inline const T& max_(const T& a, const T& b, const T& c)
{
	return a < b ? max_(b, c) : max_(a, c);
}

//! clamps a value between low and high
template <class T>
inline const T clamp(const T& value, const T& low, const T& high)
{
	return min_(max_(value,low), high);
}

inline bool equals(const f64 a, const f64 b, const f64 tolerance = ROUNDING_ERROR_f64)
{
	return (a + tolerance >= b) && (a - tolerance <= b);
}

// returns if a equals b, taking possible rounding errors into account
inline bool equals(const f32 a, const f32 b, const f32 tolerance = ROUNDING_ERROR_f32)
{
	return (a + tolerance >= b) && (a - tolerance <= b);
}

inline bool equals(const u32 a, const u32 b)
{
	return a==b;
}

inline s32 floor32(f32 x)
{
	return (s32) floorf ( x );
}

inline s32 ceil32( f32 x )
{
	//TODO优化
	return (s32) ceilf ( x );
}

inline f32 round_( f32 x )
{
	return floorf( x + 0.5f );
}

inline s32 round32(f32 x)
{
	return (s32) round_(x);
}

inline f32 reciprocal(const f32 f)
{
	//TODO优化
	return 1.f/f;
}

inline f64 reciprocal ( const f64 f )
{
	return 1.0 / f;
}

inline bool iszero(const f64 a, const f64 tolerance = ROUNDING_ERROR_f64)
{
	return fabs(a) <= tolerance;
}

inline bool iszero(const f32 a, const f32 tolerance = ROUNDING_ERROR_f32)
{
	return fabsf(a) <= tolerance;
}

inline bool iszero(const s32 a, const s32 tolerance = 0)
{
	return ( a & 0x7ffffff ) <= tolerance;
}

inline bool iszero(const u32 a, const u32 tolerance = 0)
{
	return a <= tolerance;
}

inline s32 s32_min(s32 a, s32 b)
{
	const s32 mask = (a - b) >> 31;
	return (a & mask) | (b & ~mask);
}

inline s32 s32_max(s32 a, s32 b)
{
	const s32 mask = (a - b) >> 31;
	return (b & mask) | (a & ~mask);
}

inline s32 s32_clamp (s32 value, s32 low, s32 high)
{
	return s32_min(s32_max(value,low), high);
}

template<class T>
inline const T square(const T& a)
{
	return a*a;
}

inline f32 squareroot(const f32 f)
{
	return sqrtf(f);
}

inline f64 squareroot(const f64 f)
{
	return sqrt(f);
}
inline s32 squareroot(const s32 f)
{
	return static_cast<s32>(squareroot(static_cast<f32>(f)));
}
inline f32 reciprocal_squareroot(const f32 f)
{
	return 1.f/ sqrtf(f);
}
inline s32 reciprocal_squareroot(const s32 x)
{
	return static_cast<s32>(reciprocal_squareroot(static_cast<f32>(x)));
}

//注意0不是2的N次幂
inline bool isPowerOf2(const u32 a)
{
	return a&&!(a&(a-1));
}
//获取比value大的最近2的N次幂，如果value为0，返回0
//注意0不是2的N次幂
inline u32 nearestPowerOf2(const u32 value)
{
	/* Error! */
	if (value == 0) return 0;

	u32 rval=1;
	while(rval<value) rval<<=1;
	return rval;

}
inline f32 fract(const f32 x )
{
	return x - floorf (x);
}

//获取n二进制表示中1的个数
inline u32 countBits(const u32 a)
{
	u32 s=0;
	u32 n=a;
	while(n)  
	{  
		n &= (n - 1);  
		++s;  
	}
	return s;
}

//获取a和b的最大公约数
inline s32 gcd(s32 a,s32 b){  
	s32 k=1,t;  
	while(~a&1 && ~b&1)k<<=1,a>>=1,b>>=1;  
	t=(a&1)?-b:a>>1;  
	do{  
		while(~t&1)t>>=1;  
		if(t>0)a=t;else b=-t;  
	}while(t=a-b);  
	return a*k;  
}  

}
}

#endif