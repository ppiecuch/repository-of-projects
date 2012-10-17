#include <stdio.h>
#include <sys/timeb.h>
#include <time.h>
#include <crtdbg.h>
#include <math.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

typedef unsigned int u32;
typedef int s32;
typedef float f32;

template<class T>
inline const T& min_(const T& a, const T& b)
{
	return a < b ? a : b;
}
inline float round_( float x )
{
	return floorf( x + 0.5f );
}
//x,y越大，误差会跟着越大，[0,100]时，误差在[-4,3]之间
//时间：10^8次,sqrt用时12秒，本函数用时10秒
int Math_DistPointPoint(int dx, int dy) {
	if (dx < 0) {
		dx = -dx;
	}
	if (dy < 0) {
		dy = -dy;
	}

	int min, max;
	if (dx < dy) {
		min = dx;
		max = dy;
	} else {
		min = dy;
		max = dx;
	}
	return ((max << 8) - (max << 3) - (max << 1) + (min << 6) + (min << 5) + (min << 2) + (min << 1)) >> 8;
}

//x,y越大，误差会跟着越大，误差与Math_DistPointPoint相当
//时间：10^8次,sqrt用时12秒，本函数用时10秒
u32 approx_distance( s32 dx, s32 dy )
{
	u32 min, max, approx; if ( dx < 0 ) dx = -dx;
	if ( dy < 0 ) dy = -dy; if ( dx < dy )
	{
		min = dx;
		max = dy;
	} else {
		min = dy;
		max = dx;
	} approx = ( max * 1007 ) + ( min * 441 );
	if ( max < ( min << 4 ))
		approx -= ( max * 40 ); // add 512 for proper rounding
	return (( approx + 512 ) >> 10 );
}
//x,y越大，误差会跟着越大，误差与Math_DistPointPoint相当
//时间：10^8次,sqrt用时12秒，本函数用时10秒
float CarmSqrt(float x){
	union{
		int intPart;
		float floatPart;
	} convertor;
	union{
		int intPart;
		float floatPart;
	} convertor2;
	convertor.floatPart = x;
	convertor2.floatPart = x;
	convertor.intPart = 0x1FBCF800 + (convertor.intPart >> 1);
	convertor2.intPart = 0x5f3759df - (convertor2.intPart >> 1);
	return 0.5f*(convertor.floatPart + (x * convertor2.floatPart));
}
float    Faster_Sqrtf(float f)
{
	float   result;
	_asm
	{
		mov eax, f
		sub eax, 0x3f800000
		sar eax, 1
		add eax, 0x3f800000
		mov result, eax
	}
	return result;
}
int FastDistance2D(int x, int y)
{
	x = abs(x);
	y = abs(y);
	int mn = min_(x,y);
	return(x+y-(mn>>1)-(mn>>2)+(mn>>4));
}
float sqrt_array[] ={0.0,1.0,4.0,3.0*3.0,4.0*4.0,5.0*5.0,6.0*6.0,7.0*7.0,8.0*8.0,9.0*9.0,10.0*10.0,100.0*100.0,1000.0*1000.0};
float sqrtx(float t)
{
	float sqrt_base =1.0;
	int i = 0;
	float sqrt_ret;
	float temp;
	while(i<=12&&t>sqrt_array[i++]) ;//遍历寻找一个基的范围
	if (i>=1&&i<=11)//0.0 < t < 100.0*100.0
	{
		sqrt_base *= (i-1.0);
	}
	else if (i==12)
	{
		sqrt_base = 100.0;
	}else if (i==13)
	{
		sqrt_base = 1000.0;
	}
	sqrt_ret = sqrt_base;
	temp = (sqrt_ret * sqrt_ret - t)/t;
	//while (temp>0.00005 || temp < -0.00005)
	while (temp>0.5 || temp < -0.5)
	{
		sqrt_ret = (sqrt_base + t/sqrt_base)/2.0;
		sqrt_base = sqrt_ret;
		temp = (sqrt_ret * sqrt_ret - t)/t;
	}

	return sqrt_ret;
}
inline f32 reciprocal_squareroot(const f32 f){
	f32 recsqrt;
	__asm rsqrtss xmm0, f           // xmm0 = rsqrtss(f)
	__asm movss recsqrt, xmm0       // return xmm0
	return recsqrt;
}
int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

	struct _timeb start,end;
	double time=0,fasttime=0;
	float k;
	int m;
	int count=0;
	float aberration=0;
#if 0
	for(int i=0;i<10000;++i)
	{
		for(int j=0;j<10000;++j)
		{
#if 1
			k=sqrt((float)(i*i+j*j));
			int l=round_(k);
			if(abs(k-l)<0.0001f)
			{
				//m=Math_DistPointPoint(i,j);
				//m=approx_distance(i,j);
				//m=CarmSqrt((float)(i*i+j*j));
				//m=Faster_Sqrtf((float)(i*i+j*j));
				//m=FastDistance2D(i,j);
				m=sqrtx((float)(i*i+j*j));

				float d=abs(m-k);
				if(d>aberration)
					aberration=d;
				//printf("(%d,%d)-->%d(%.2f)[aberration:%.2f]\r\n",i,j,m,k,aberration);
			}
#else
			_ftime64_s(&start);
			k=sqrt((float)(i*i+j*j));
			_ftime64_s(&end);

			time+=end.time-start.time;
			time+=(end.millitm-start.millitm)/1000.f;

			_ftime64_s(&start);
			//m=Math_DistPointPoint(i,j);
			//m=approx_distance(i,j);
			//m=CarmSqrt((float)(i*i+j*j));
			//m=Faster_Sqrtf((float)(i*i+j*j));
			//m=FastDistance2D(i,j);
			m=sqrtx((float)(i*i+j*j));
			_ftime64_s(&end);

			fasttime+=end.time-start.time;
			fasttime+=(end.millitm-start.millitm)/1000.f;

			++count;
#endif
		}
	}
#else
	for(int i=0;i<10000;++i)
	{
		for(int j=0;j<10000;++j)
		{
#if 1
			if(i==0&&j==0)
				continue;
			k=1/sqrt((float)(i*i+j*j));
			m=reciprocal_squareroot((float)(i*i+j*j));
			float d=abs(m-k);
			if(d>aberration)
				aberration=d;
#else
			_ftime64_s(&start);
			k=1/sqrt((float)(i*i+j*j));
			_ftime64_s(&end);
			time+=end.time-start.time;
			time+=(end.millitm-start.millitm)/1000.f;

			_ftime64_s(&start);
			m=reciprocal_squareroot((float)(i*i+j*j));
			_ftime64_s(&end);
			fasttime+=end.time-start.time;
			fasttime+=(end.millitm-start.millitm)/1000.f;
#endif
		}
	}
#endif

	printf("count:%d,use time:%.2f---%.2f,aberration:%.4f\r\n",count,time,fasttime,aberration);

	getchar();
	return 0;
}