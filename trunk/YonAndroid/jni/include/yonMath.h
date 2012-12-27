#ifndef _YON_CORE_MATH_H_
#define _YON_CORE_MATH_H_

#include "yonTypes.h"
#include <math.h>
#include <float.h> //For FLT_MAX
#include <limits.h> // For INT_MAX / UINT_MAX

namespace yon{
namespace core{

//�������
const s32 ROUNDING_ERROR_S32 = 0;
const f32 ROUNDING_ERROR_f32 = 0.000001f;
const f64 ROUNDING_ERROR_f64 = 0.00000001;

//32λ
#ifdef PI
#undef PI
#endif

//PI��ض���
const f32 PI=3.1415926535897932384626433832795f;
const f64 PI64= 3.1415926535897932384626433832795028841971693993751;
const f32 PI2=PI+PI;
const f32 RECIPROCAL_PI	= 1.0f/PI;
const f32 HALF_PI	= PI*0.5f;

//�ӽǶ�תΪ����
const f32 DEGTORAD = PI / 180.0f;
//�ӻ���תΪ�Ƕ�
const f32 RADTODEG   = 180.0f / PI;
const f64 RADTODEG64 = 180.0 / PI64;

//refer to:http://bbs.chinaunix.net/thread-3746530-1-1.html
//ANSI/IEEE Std 754-1985��׼
//IEEE 754����㷺ʹ�õĶ����Ƹ�����������׼�������CPU�븡�������������á�
//IEEE 754�涨�˶��ֱ�ʾ������ֵ�ķ�ʽ���ڱ��ĵ���ֻ����32bits��float�������͡�
//������Ϊ3�����֣��ֱ��Ƿ���λS��sign bit����ָ��ƫ��E��exponent bias����С������F��fraction����
//| 1bit |   8bit  |        23bit        |
//|   S  |    E    |          F          |
//Sλռ1bit��Ϊbit31��SλΪ0����������������SλΪ1���������Ǹ�����
//����˵0x449A522C��SλΪ0����ʾ����һ��������0x849A522C��SλΪ1����ʾ����һ��������
//Eλռ8bits��Ϊbit23~bit30��Eλ����2��N�η�������Ҫ��ȥ127��
//����˵EλΪ87����ôEλ��ֵΪ2^��87-127��=9.094947017729282379150390625e-13��
//Fλռ23bits��Ϊbit0~bit22��Fλ��С��������λ��������bit22��2-1=0.5��bit21��2-2=0.25��
//�Դ����ƣ�bit0Ϊ2-23=0.00000011920928955078125����Fλ��������һ��1��
//Ҳ����˵Fλ����ʾ��ֵ��1+��Fλbit22~bit0����ʾ����ֵ��������˵Fλ��0b10100000000000000000001��
//ֻ��bit22��bit20��bit0Ϊ1����ôFλ��ֵΪ1+(2-1+2-3+2-23)��Ϊ1.62500011920928955078125��
// �����������Ӷ����������㵽�������Ĺ�ʽΪ��(-1)S*2^(E-127)*(1+F)��
//�����м�����������Σ�
//1����EλΪ0����FλҲΪ0ʱ��ʾ������0����ʱ��������SλӰ�죬���ֳ�+0��-0����0������ֵ����ȵġ�
//�����������0x00000000��ʾ+0����������0x80000000��ʾ-0��
//2����EλΪ0����Fλ��Ϊ0ʱ������Ϊ(-1)S��2^(-126)��F��ע�⣬Eλ��ָ����-126��������0-127=-127��
//����Fλ��0.xx��ʽ������1.xx��ʽ������0x00000001�ĸ�����Ϊ2^(-126)*2^(-23)=1.4012984643248170709237295832899e-45��
//һ��EΪ��Ϊ0����0��Ϊ1����������2���Ĺ�ϵ����Ϊ��ʽ�ı��ˡ�
//3����EλΪ255����Fλ��Ϊ0ʱ��ʾ����ֵ��Ҳ����˵�ǷǷ���������0x7F800001��
//4����EλΪ255����FλΪ0ʱ��ʾ������������ʱ��������SλӰ�죬����0x7F800000��ʾ�������0xFF800000��ʾ�������
//������ʹ��1��������0ʱ�������������0x7F800000��
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
	//TODO�Ż�
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
	//TODO�Ż�
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

//ע��0����2��N����
inline bool isPowerOf2(const u32 a)
{
	return a&&!(a&(a-1));
}
//��ȡ��value������2��N���ݣ����valueΪ0������0
//ע��0����2��N����
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

//��ȡn�����Ʊ�ʾ��1�ĸ���
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

//��ȡa��b�����Լ��
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