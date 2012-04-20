// Test.cpp : 定义控制台应用程序的入口点。
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

#include <string.h>


void TRACE(LPCTSTR pszFormat, ...)   
{   
	va_list pArgs;   

	char szMessageBuffer[16380]={0};   
	va_start( pArgs, pszFormat );   
	_vsntprintf( szMessageBuffer, 16380, pszFormat, pArgs );   
	va_end( pArgs );   

	OutputDebugString(szMessageBuffer);   
}  

struct Temp{
	int i;
};

Temp* t=NULL;

void check(Temp* &t)
{
	if(t==NULL)
	{
		t=new Temp();
		cout<<t<<endl;
	}
}
/**
* 将矩阵单位化
*/
inline void identityMatrix4(double m[16]){
	m[0] = 1; m[1] = 0; m[2] = 0; m[3] = 0;
	m[4] = 0; m[5] = 1; m[6] = 0; m[7] = 0;
	m[8] = 0; m[9] = 0; m[10] = 1; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}
/**
* 计算矩阵src的逆矩阵保存到dest
* 原理：首先将结果矩阵单位化，然后将源矩阵单位化，
* 同时将运算规则应用于结果矩阵，最后结果矩阵便是源矩阵的逆矩阵
*/
bool inverseMatrix4(const double src[16],double result[16])
{
	int i, j, k;
	double temp;
	double tempMatrix[4][4];                //临时矩阵
	//复制src矩阵存到临时矩阵中
	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			tempMatrix[i][j] = src[(i<<2)+j];
	//单位化result矩阵
	identityMatrix4(result);
	for (i = 0; i<4; i++)
	{
		//寻找主元
		k = i;
		for (j=i+1; j<4; j++)
		{
			if (fabs(tempMatrix[j][i]) > fabs(tempMatrix[i][i]))
				k = j;
		}
		//如果主元所在行不是第i行，进行行交换
		if (k != i)
		{
			for (j=0; j<4; j++)
			{
				temp = tempMatrix[i][j];
				tempMatrix[i][j] = tempMatrix[k][j];
				tempMatrix[k][j] = temp;
				//result伴随交换
				temp = result[(i<<2)+j];
				result[(i<<2)+j] = result[(k<<2)+j];
				result[(k<<2)+j] = temp;
			}
		}
		//判断主元是否为0, 若是, 则矩阵A不是满秩矩阵,不存在逆矩阵
		//TRACE("tempMatrix[%d][%d]=%.2f\r\n",i,i,tempMatrix[i][i] );
		if (tempMatrix[i][i] == 0)
			return false;
		//消去src的第i列除去i行以外的各行元素
		temp = tempMatrix[i][i];
		for (j=0; j<4; j++)
		{
			tempMatrix[i][j] /=temp;        //主对角线上的元素变为1
			result[(i<<2)+j] /= temp;       //伴随计算
		}
		for (j=0; j<4; j++)	
		{
			if (j != i)						//不是第i行
			{
				temp = tempMatrix[j][i];
				for (k=0; k<4; k++)        //第j行元素 - i行元素*j列i行元素
				{
					tempMatrix[j][k] -= tempMatrix[i][k]*temp;
					result[(j<<2)+k] -= result[(i<<2)+k]*temp;
				}
			}
		}
	}
	return true;
}
void printMatrix(double m[16]){
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			if(j>0)
				cout<<",";
			cout<<m[i*4+j];
			TRACE(",%g",m[i*4+j]);
		}
		cout<<endl;
	}
	TRACE("\r\n");
}

void invertMatrix(double m[16])
{
	double temp[16];
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			temp[j*4+i]=m[4*i+j];
		}
	}
	for(int i=0;i<16;i++)
		m[i]=temp[i];
}
void multiMatrix(double m1[16],double m2[16],double result[16])
{
	for(int i=0;i<16;i++)
		result[i]=0;
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			result[i*4+j]+=m1[i*4+0]*m2[j];
			result[i*4+j]+=m1[i*4+1]*m2[4+j];
			result[i*4+j]+=m1[i*4+2]*m2[8+j];
			result[i*4+j]+=m1[i*4+3]*m2[12+j];
		}
	}
}
char* getFileName(const char *name) 
{ 
	int from,to,i; 
	char *newstr=NULL;
	char *temp=NULL;
	int len=strlen(name);
	char *fullname=(char*)malloc(len+1);
	strncpy(fullname,name,len+1);
	*(fullname+len)=0; 
	if(fullname!=NULL) 
	{ 
		if((temp=strchr(fullname,'.'))==NULL)//if   not   find   dot 
		{
			newstr = (char*)malloc(len+1);
			strncpy(newstr,fullname,len+1);
			*(newstr+len)=0; 
		}
		else 
		{ 
			from = strlen(fullname)-1; 
			to = (temp-fullname);//the   first   dot 's   index; 
			for(i=from;i--;i<=to) 
				if(fullname[i]=='.')break;//find   the   last   dot 
			newstr = (char*)malloc(i+1);
			strncpy(newstr,fullname,i);
			*(newstr+i)=0; 
		}
	}
	free(fullname);
	return newstr; 
} 

char* strcat(char *s1,char*s2)
{
	int i,i1,i2;
	i1=strlen(s1);
	i2=strlen(s2);

	char *s3=(char*)malloc(i1+i2+1);
	while(*s1!='\0')
		*(s3++)=*(s1++);

	while(*s2!='\0')
		*(s3++)=*(s2++);

	*(s3++)='\0';
	for(i=0;i<i1+i2+1;i++)
		s3--;

	return s3;
}


int _tmain(int argc, _TCHAR* argv[])
{
	/*check(t);
	cout<<t<<endl;*/

	//矩阵转四元数算法
	/*float		q[4];
	float	mat[4][4];
	mat[0][0]=1;mat[0][1]=0;mat[0][2]=0;mat[0][3]=0;
	mat[1][0]=0;mat[1][1]=0;mat[1][2]=-1;mat[1][3]=0;
	mat[2][0]=0;mat[2][1]=1;mat[2][2]=0;mat[2][3]=0;
	mat[3][0]=0;mat[3][1]=0;mat[3][2]=0;mat[3][3]=1;
	float		trace;
	float		s;
	float		t;
	int     	i;
	int			j;
	int			k;

	static int 	next[ 3 ] = { 1, 2, 0 };

	trace = mat[ 0 ][ 0 ] + mat[ 1 ][ 1 ] + mat[ 2 ][ 2 ];

	if ( trace > 0.0f ) 
	{
		t = trace + 1.0f;
		s = 0.5f / sqrtf(t);
		q[3] = s * t;
		q[0] = ( mat[ 2 ][ 1 ] - mat[ 1 ][ 2 ] ) * s;
		q[1] = ( mat[ 0 ][ 2 ] - mat[ 2 ][ 0 ] ) * s;
		q[2] = ( mat[ 1 ][ 0 ] - mat[ 0 ][ 1 ] ) * s;

	} else {
		i = 0;
		if ( mat[ 1 ][ 1 ] > mat[ 0 ][ 0 ] ) {
			i = 1;
		}
		if ( mat[ 2 ][ 2 ] > mat[ i ][ i ] ) {
			i = 2;
		}
		j = next[ i ];
		k = next[ j ];
		t = ( mat[ i ][ i ] - ( mat[ j ][ j ] + mat[ k ][ k ] ) ) + 1.0f;
		s = 0.5f / sqrtf(t);
		q[i] = s * t;
		q[3] = ( mat[ k ][ j ] - mat[ j ][ k ] ) * s;
		q[j] = ( mat[ j ][ i ] + mat[ i ][ j ] ) * s;
		q[k] = ( mat[ k ][ i ] + mat[ i ][ k ] ) * s;
	}
	cout<<q[0]<<","<<q[1]<<","<<q[2]<<","<<q[3]<<endl;*/


	//四元数转矩阵算法
	/*float ret[4][4];
	float x=-0.71f;
	float y=0.00f;
	float z=0.00f;
	float w=0.71f;
	float xx = x*x;
	float yy = y*y;
	float zz = z*z;
	float xy = x*y;
	float wz = w*z;
	float wy = w*y;
	float xz = x*z;
	float yz = y*z;
	float wx = w*x;

	ret[0][0] = 1.0f-2*(yy+zz);
	ret[0][1] = 2*(xy-wz);
	ret[0][2] = 2*(wy+xz);
	ret[0][3] = 0.0f;

	ret[1][0] = 2*(xy+wz);
	ret[1][1] = 1.0f-2*(xx+zz);
	ret[1][2] = 2*(yz-wx);
	ret[1][3] = 0.0f;

	ret[2][0] = 2*(xy-wy);
	ret[2][1] = 2*(yz+wx);
	ret[2][2] = 1.0f-2*(xx+yy);
	ret[2][3] = 0.0f;

	ret[3][0] = 0.0f;
	ret[3][1] = 0.0f;
	ret[3][2] = 0.0f;
	ret[3][3] = 1.0f;

	for(int i=0;i<4;i++)
		cout<<ret[i][0]<<","<<ret[i][1]<<","<<ret[i][2]<<","<<ret[i][3]<<"\r\n"<<endl;*/

	//求逆矩阵
	
	/*//double src[16]={1,0,0,60.8856,0,0,-1,-11.0517,0,1,0,0.738008,0,0,0,1};
	double src[16]={0.997268,-0.073871,0,10.1117,0,0,-1,0.000002,0.073871,0.997268,0,-64.2066,0,0,0,1};
	//double src[16]={5,2,0,0,2,1,0,0,0,0,8,3,0,0,5,2};
	double result[16];
	inverseMatrix4(src,result);
	printMatrix(src);
	cout<<"result:"<<endl;
	printMatrix(result);*/

	//转置矩阵
	//double src[16]={1,0,0,-5.51729,0,0,-1,0,0,1,0,0.738007,0,0,0,1};
	//printMatrix(src);
	//invertMatrix(src);
	//printMatrix(src);

	//矩阵相乘
	/*
	double m1[16]={0,1,0,0,-1,0,0,0,0,0,1,100,0,0,0,1};
	double m2[16]={0,1,0,0,0,0,1,0,1,0,0,0,0,0,0,1};
	double invert[16]={1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1};
	double result[16],result2[16];
	/*multiMatrix(m1,m2,result);
	printMatrix(result);
	multiMatrix(invert,result,result2);
	printMatrix(result2);*/
	/*multiMatrix(invert,m1,result);
	printMatrix(result);
	multiMatrix(result,m2,result2);
	printMatrix(result2);*/

	/*char str[] ="a,b,c,d*e";  
	const char * split = ",";  
	char * p=NULL;    
	char *pOut=NULL;

	p = strtok_s(str,split,&pOut);    
	while(p!=NULL) {    
		printf ("%s\r\n",p);    
		p = strtok_s(NULL,split,&pOut);  
	}*/

	//const char* name="f.ani";
	//char* filename=getFileName(name);
	//TRACE("%s\r\n",filename);
	//free(filename);

	//unsigned char c=255;
	//char cc=(char)c;
	//unsigned char ccc=(unsigned char)cc;
	//TRACE("%d\r\n",ccc);

	char *s1="good";
	char *s2="day";
	char* r=strcat(s1,s2);
	TRACE("%s\r\n",r);
	free(r);


	_getch();
	return 0;
}

