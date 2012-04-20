// GaussBlur.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <math.h>
using namespace std;

#include <stdio.h>  
#include <wtypes.h>  
#include <stdarg.h>  
#include <tchar.h> 

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

#define M_PI 3.14159265358979323846 

void TRACE(LPCTSTR pszFormat, ...)   
{   
	va_list pArgs;   

	char szMessageBuffer[16380]={0};   
	va_start( pArgs, pszFormat );   
	_vsntprintf( szMessageBuffer, 16380, pszFormat, pArgs );   
	va_end( pArgs );   

	OutputDebugString(szMessageBuffer);   
}

void MakeGauss()
{
	double sigma = 1.4;     // σ是正态分布的标准偏差 这里为 1.4
	double dResult[5][5];    // 用于存储结果
	double dResult1[5][5];    // 用于存储结果

	// 数组的中心点
	int nCenterX = 2, nCenterY = 2;  // 中心点位置以1开始的吧
	int nSize = 5;
	// 数组的某一点到中心点的距离
	double  dDis; 
	double PI = 3.1415926535;
	// 中间变量
	double  dValue; 
	double  dSum  ;
	dSum = 0 ; 
	int i, j;

	for(i = 0; i< nSize; ++i)
	{
		for(j = 0; j < nSize; ++j)
		{
			dDis = (i - nCenterX) * (i - nCenterX) + (j  - nCenterY) * (j - nCenterY);
			dValue = exp( - dDis / (2 * sigma * sigma)) /
				(2 * PI * sigma * sigma);
			dResult[i][j] = dValue;
			dSum += dValue;
		}
	}
	// 归一化
	for(i = 0; i< nSize; ++i)
	{
		for(j = 0; j < nSize; ++j)
		{
			dResult1[i][j] = dResult[i][j] / dSum;
		}
	}
	std::cout << dSum << std::endl;
	for(i = 0; i< nSize; ++i)
	{
		for(j = 0; j < nSize; ++j)
		{
			// dResult1才是高斯的结果, 但是dResult * 1.95 * 100却得到了文章上说的结果
			// 一个标准差为1.4的高斯5x5的卷积核
			// 暂时不知道为什么。
			std::cout << (int)(dResult[i][j] * 1.95 * 100) << "  ";
		}
		std::cout << std::endl;
	}
}
// 代码2(网上找的)
void Gauss()
{
	int h_size;
	float siz,sigma;
	int i, j;
	printf("Please input size of gaussian core\n");
	scanf("%d",&h_size);
	printf("Please input sigma:\n");
	scanf("%f",&sigma);
	siz=(h_size-1)/2;
	float **a,**b;
	a=new float*[h_size];
	for(int i=0;i<h_size;i++)   a[i]=new float[h_size];
	b=new float*[h_size];
	for( i=0;i<h_size;i++)   b[i]=new float[h_size];
	for(i=0;i<h_size;i++)
	{
		for(j=0;j<h_size;j++)
		{
			a[i][j]=-siz+j;
			printf("%4.2f ",a[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	for( i=0;i<h_size;i++)
	{
		for(j=0;j<h_size;j++)
		{
			b[i][j]=a[j][i];
			printf("%4.2f ",b[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	float h_sum=0;
	for( i=0;i<h_size;i++)
	{
		for(j=0;j<h_size;j++)
		{
			a[i][j]=a[i][j]*a[i][j];
			b[i][j]=b[i][j]*b[i][j];
			a[i][j]=-(a[i][j]+b[i][j])/(2*sigma*sigma);
			a[i][j]=exp(a[i][j]);
			if(a[i][j]<0.0001) a[i][j]=0;
			h_sum=h_sum+a[i][j];
		}
	}

	for(i=0;i<h_size;i++)
	{
		for(j=0;j<h_size;j++)
		{
			a[i][j]=a[i][j]/h_sum;
		}
	}
	for(i=0;i<h_size;i++)
	{
		for(j=0;j<h_size;j++)
		{
			printf("%4.4f ",a[i][j]);
		}
		printf("\n");
	}
}

float GetGaussianDistribution( float x, float y, float sigma ) {
	//float g = 1.0f / sqrt( 2.0f * M_PI * sigma * sigma );
	//return g * exp( -(x * x + y * y) / (2 * sigma * sigma) );
	return exp( -0.5000f*(x*x+y*y)/(sigma*sigma) ) / sqrt(2.0f*M_PI*sigma*sigma);
}
/**
* g(x)=e(-x^2/sigma^2)/sqrt(2*pi)*sigma
* //TODO 好像是错的
*/
void MakeGaussVector(float* gauss,int size,float sigma)
{
	/*int i, j;
	float hsize=(size-1)/2;
	for(int i=0;i<=hsize;i++)
		gauss[i]=gauss[size-1-i]=GetGaussianDistribution(0,i,sigma);*/
	float *a;
	int i, j;
	float hsize=(size-1)/2;
	a=new float[size];
	for(i=0;i<size;i++)
	{
		a[i]=-hsize+i;
		printf("%4.2f ",a[i]);
	}
	printf("\n");
	float h_sum=0;
	for( i=0;i<size;i++)
	{
		a[i]=a[i]*a[i];
		a[i]=-a[i]/(sigma*sigma);
		a[i]=exp(a[i]);
		printf("%4.2f ",a[i]/(sqrt(2*M_PI)*sigma));
		if(a[i]<0.0001) a[i]=0;
			h_sum=h_sum+a[i];
	}
	printf("\n");

	for(i=0;i<size;i++)
	{
		*(gauss+i)=a[i]/h_sum;
	}
	delete[] a;
	
	for(i=0;i<size;i++)
	{
		printf("%4.4f ",*(gauss+i));
		
	}
	printf("\n");
	

}

/**
 * G(x,y)=e^(-(x^2+y^2)/2*sigma^2)/2*pi*sigma^2 
 */
void MakeGaussMatrix(float** gauss,int size,float sigma)
{
	int i, j;
	float hsize=(size-1)/2;
	float **a,**b;
	a=new float*[size];
	for(int i=0;i<size;i++)
		a[i]=new float[size];
	b=new float*[size];
	for( i=0;i<size;i++)
		b[i]=new float[size];
	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			a[i][j]=-hsize+j;
			printf("%4.2f ",a[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	for( i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			b[i][j]=a[j][i];
			printf("%4.2f ",b[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	float h_sum=0;
	for( i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			a[i][j]=a[i][j]*a[i][j];
			b[i][j]=b[i][j]*b[i][j];
			a[i][j]=-(a[i][j]+b[i][j])/(2*sigma*sigma);
			a[i][j]=exp(a[i][j]);
			if(a[i][j]<0.0001) a[i][j]=0;
			h_sum=h_sum+a[i][j];
		}
	}

	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			*((float*)gauss+i*size+j)=a[i][j]/h_sum;
		}
	}
	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			printf("%4.4f ",*((float*)gauss+i*size+j));
		}
		printf("\n");
	}

	for( i=0;i<size;i++)
	{
		delete[] a[i];
		delete[] b[i];
	}
	delete[] a;
	delete[] b;


}

/**
 * 快速高斯模糊算法
 */
void GaussBlur(float** data,int width,int height,int radius,double sigma)
{
	float** temp;
	temp=new float*[height];
	for(int i=0;i<height;i++)
		temp[i]=new float[width];
	float factor;

	int len=2*radius+1;
	float* gauss=new float[len];
	/*for(int i=0;i<len;i++)
	{
		gauss[i]=GetGaussianDistribution(i,i,sigma);
		printf("%.2f,",gauss[i]);
	}
	printf("\r\n");*/
	MakeGaussVector(gauss,len,sigma);

	//横变换
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			temp[i][j]=0;
			for(int k=-radius;k<=radius;k++)
				if(j+k>=0&&j+k<width)
				{
					temp[i][j]+=*((float*)data+i*width+j+k)*gauss[k+radius];
				}
		}
	}

	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			*((float*)data+i*width+j)=temp[i][j];
		}
	}

	//竖变换
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			temp[i][j]=0;
			for(int k=-radius;k<=radius;k++)
				if(i+k>=0&&i+k<height)
				{
					temp[i][j]+=*((float*)data+(i+k)*width+j)*gauss[k+radius];
				}
		}
	}

	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			*((float*)data+i*width+j)=temp[i][j];
		}
	}

	delete[] gauss;
	for(int i=0;i<height;i++)
		delete[] temp[i];
	delete[] temp;
}
void GaussBlur2(float** data,int width,int height,int radius,double sigma)
{
	float** temp;
	temp=new float*[height];
	for(int i=0;i<height;i++)
		temp[i]=new float[width];
	float factor;

	int len=2*radius+1;
	float* gauss=new float[len];
	/*for(int i=0;i<len;i++)
	{
	gauss[i]=GetGaussianDistribution(i,i,sigma);
	printf("%.2f,",gauss[i]);
	}
	printf("\r\n");*/
	MakeGaussVector(gauss,len,sigma);

	//横变换
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			temp[i][j]=0;
			for(int k=-radius;k<=radius;k++)
				if(j+k>=0&&j+k<width)
				{
					temp[i][j]+=data[i][j+k]*gauss[k+radius];
				}
		}
	}

	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			data[i][j]=temp[i][j];
		}
	}

	//竖变换
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			temp[i][j]=0;
			for(int k=-radius;k<=radius;k++)
				if(i+k>=0&&i+k<height)
				{
					temp[i][j]+=data[i+k][j]*gauss[k+radius];
				}
		}
	}

	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			data[i][j]=temp[i][j];
		}
	}

	delete[] gauss;
	for(int i=0;i<height;i++)
		delete[] temp[i];
	delete[] temp;
}

typedef unsigned char UBYTE;

void DoPreComputation(float *src, int src_w, int src_h, float *dst)
{
	for (int y=0;y<src_h;y++)
	{
		for (int x=0;x<src_w;x++)
		{
			int tot=src[0];
			if (x>0) tot+=dst[-1];
			if (y>0) tot+=dst[-src_w];
			if (x>0 && y>0) tot-=dst[-src_w-1];
			*dst++=tot;
			src++;
		}
	}
}

// this is a utility function used by DoBoxBlur below
float ReadP(float *p, int w, int h, int x, int y)
{
	if (x<0) x=0; else if (x>=w) x=w-1;
	if (y<0) y=0; else if (y>=h) y=h-1;
	return p[x+y*w];
}

// the main meat of the algorithm lies here
void DoBoxBlur(float *src, int src_w, int src_h, float *dst, float *p, int boxw, int boxh)
{
	if (boxw<0 || boxh<0)
	{
		memcpy(dst,src,src_w*src_h); // deal with degenerate kernel sizes
		return;
	}
	float mul=1.f/((boxw*2+1)*(boxh*2+1));
	for (int y=0;y<src_h;y++)
	{
		for (int x=0;x<src_w;x++)
		{
			int tot=ReadP(p,src_w,src_h,x+boxw,y+boxh)
				+ReadP(p,src_w,src_h,x-boxw,y-boxh)
				-ReadP(p,src_w,src_h,x-boxw,y+boxh)
				-ReadP(p,src_w,src_h,x+boxw,y-boxh);
			*dst++=float(tot*mul);
			src++;
		}
	}
}
void DoConvert(float *src, int src_w, int src_h, float* dst)
{
	float center=(float)*(src+(src_h-1)*src_w/2+(src_w-1)/2);
	for (int y=0;y<src_h;y++)
	{
		for (int x=0;x<src_w;x++)
		{
			*dst++=*src/center;
			src++;
		}
	}
}
void DoGaussBlur(int src_w, int src_h, int box_w, int box_h, float* dst)
{
	int len=src_w*src_h;
	float *src=new float[len];
	float* p=new float[len];
	float* temp=new float[len];
	for(int i=0;i<len;i++)
		src[i]=len;
	DoPreComputation(src,src_w,src_h,p);
	DoBoxBlur(src,src_w,src_h,temp,p,box_w,box_h);
	DoConvert(temp,src_w,src_h,dst);

	for(int i=0;i<len;i++)
	{

		TRACE("%.2f,",dst[i]);
		if(i%src_w==src_w-1)
			TRACE("\r\n");
	}
	delete[] p;
	delete[] temp;
	delete[] src;
}

#define R 3
void cal_gaussian_matrix(float* m,double sigma)
{
	MakeGaussVector(m,2*R+1,sigma);
}
void gauss_blur(float indata[][R],float outdata[][R],int width,int height,double sigma)
{
	int x,y,r;
	float v;
	float gaussMatrix[2*R+1];//高斯模板
	cal_gaussian_matrix(gaussMatrix,sigma);//计算高斯模板
	float** temp=(float**)new float*[width];
	for(x=0;x<width;++x)
		temp[x]=new float[height];
	for(y=0;y<height;++y)
	{
		for(x=0;x<width;++x)
		{
			v=0.0;
			for(r=-R;r<=R;++r)
			{
				//v+=indata[x+r][y]*gaussMatrix[r+R];//原文这里越界
				v+=indata[x][y]*gaussMatrix[r+R];
			}
			temp[x][y]=v;//水平方向变换后的数据
		}
	}
	for(x=0;x<width;++x)
	{
		for(y=0;y<height;++y)
		{
			v=0.0;
			for(r=-R;r<=R;++r)
			{
				//v+=temp[x][y+r]*gaussMatrix[r+R];//原文这里越界
				v+=temp[x][y]*gaussMatrix[r+R];
			}
			outdata[x][y]=v;//竖直方向变换后的数据
			printf("%.2f,",outdata[x][y]);
		}
		printf("\n");
	}
	for(x=0;x<width;++x)
		delete[] temp[x];
	delete[] temp;
}

int _tmain(int argc, _TCHAR* argv[])
{
	EnableMemLeakCheck();

#if 0
	float data[3][3]={{1,2,3},{5,6,7},{9,10,11}};
	//float data[3][3]={{3,3,3},{3,3,3},{3,3,3}};
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			printf("%.2f,",data[i][j]);
		}
	}
	printf("\r\n");
	GaussBlur((float**)data,3,3,1,1);
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			printf("%.2f,",data[i][j]);
		}
		printf("\r\n");
	}
#endif
	//float data[5][5]={{100,2,3,4,5},{6,7,8,9,10},{11,12,13,14,15},{16,17,18,19,20},{21,22,23,24,25}};
	//float data[5][5]={{25,25,25,25,25},{25,25,25,25,25},{25,25,25,25,25},{25,25,25,25,25},{25,25,25,25,25}};
	int count=(2<<1)+1;
	float** data=(float**)new float*[count];
	for(int i=0;i<count;++i)
	{
		data[i]=new float[count];
		for(int j=0;j<count;++j)
		{
			data[i][j]=25;
		}
	}

	for(int i=0;i<5;i++)
	{
		for(int j=0;j<5;j++)
		{
			printf("%.2f,",data[i][j]);
		}
	}
	printf("\r\n");
	GaussBlur2((float**)data,5,5,5,3);
	printf("\n");
	for(int i=0;i<5;i++)
	{
		for(int j=0;j<5;j++)
		{
			printf("%.2f,",data[i][j]);
		}
		printf("\r\n");
	}
	//TMD，输入数据与输出数据是一样的
	//float indata[3][3]={{3,3,3},{3,3,3},{3,3,3}};
	//float indata[3][3]={{0,1,2},{3,4,5},{6,7,8}};
	//float outdata[3][3];
	//gauss_blur(indata,outdata,3,3,1.1283);
	//MakeGauss();
	//Gauss();
	//float v[7];
	//MakeGaussVector((float*)v,7,1.1283);
	//float t[7][7];
	//MakeGaussMatrix((float**)t,7,0.1);

	//float data[5][5]={{100,2,3,4,5},{6,7,8,9,10},{11,12,13,14,15},{16,17,18,19,20},{21,22,23,24,25}};
	//效果也不理想
	/*float data[5][5]={
		{25,25,25,25,25},
		{25,25,25,25,25},
		{25,25,25,25,25},
		{25,25,25,25,25},
		{25,25,25,25,25}};
	GaussBlur((float**)data,5,5,2);
	for(int i=0;i<5;i++)
	{
		for(int j=0;j<5;j++)
		{
			TRACE("%.2f,",data[i][j]);
		}
		TRACE("\r\n");
	}*/

	/*
	float data[3][3]={{1,2,3},{5,6,7},{9,10,11}};
	GaussBlur((float**)data,3,3,1);
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			TRACE("%.2f,",data[i][j]);
		}
		TRACE("\r\n");
	}
*/

	//float src[9]={1,2,3,5,6,7,9,10,11};
	/*float src[9]={9,9,9,9,9,9,9,9,9};
	float dst[9];
	float p[9];
	DoPreComputation(src,3,3,p);
	for(int i=0;i<9;i++)
	{
		
		TRACE("%.2f,",p[i]);
		if(i%3==2)
			TRACE("\r\n");
	}
	DoBoxBlur(src,3,3,dst,p,1,1);
	for(int i=0;i<9;i++)
	{

		TRACE("%.2f,",dst[i]);
		if(i%3==2)
			TRACE("\r\n");
	}*/

	/*UBYTE src[16]={16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16};
	UBYTE dst[16];
	ULONG p[16];
	DoPreComputation(src,4,4,p);
	for(int i=0;i<16;i++)
	{

		TRACE("%d,",p[i]);
		if(i%4==3)
			TRACE("\r\n");
	}
	DoBoxBlur(src,4,4,dst,p,1,1);
	for(int i=0;i<16;i++)
	{

		TRACE("%d,",dst[i]);
		if(i%4==3)
			TRACE("\r\n");
	}*/

	//结果不理想
	/*float src[25]={
		25,25,25,25,25,
		25,25,25,25,25,
		25,25,25,25,25,
		25,25,25,25,25,
		25,25,25,25,25};
	float dst[25];
	float p[25];
	DoPreComputation(src,5,5,p);
	for(int i=0;i<25;i++)
	{

		TRACE("%.2f,",p[i]);
		if(i%5==4)
			TRACE("\r\n");
	}
	DoBoxBlur(src,5,5,dst,p,1,1);
	for(int i=0;i<25;i++)
	{

		TRACE("%.2f,",dst[i]);
		if(i%5==4)
			TRACE("\r\n");
	}*/

	/*float src[49]={49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49};
	float dst[49];
	float p[49];
	float f[49];
	DoPreComputation(src,7,7,p);
	for(int i=0;i<49;i++)
	{

		TRACE("%.2f,",p[i]);
		if(i%7==6)
			TRACE("\r\n");
	}
	DoBoxBlur(src,7,7,dst,p,3,3);
	for(int i=0;i<49;i++)
	{

		TRACE("%.2f,",dst[i]);
		if(i%7==6)
			TRACE("\r\n");
	}
	DoConvert(dst,7,7,f);
	for(int i=0;i<49;i++)
	{

		TRACE("%.2f,",f[i]);
		if(i%7==6)
			TRACE("\r\n");
	}*/
	//float dst[81];
	//DoGaussBlur(9,9,3,3,dst);
	//上限
	//float dst[81];
	//DoGaussBlur(9,9,8,8,dst);
	getchar();
	getchar();
	return 0;
}

