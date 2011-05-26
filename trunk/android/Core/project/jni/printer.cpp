#include "printer.h"

/* ʵ��itoa������Դ���� */ 
char* myitoa(int num,char *str,int radix) 
{ 
	/* ������ */ 
	char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	unsigned unum;/* �м���� */
	int i=0,j,k;
	/* ȷ��unum��ֵ */
	if(radix==10&&num<0)/* ʮ���Ƹ��� */
	{
		unum=(unsigned)-num;
		str[i++]='-';
	}
	else unum=(unsigned)num;/* ������� */
	/* ���� */
	do
		{
		str[i++]=index[unum%(unsigned)radix];
		unum/=radix;
	}while(unum);
	str[i]='\0';
	/* ת�� */ 
	if(str[0]=='-') k=1; /* ʮ���Ƹ��� */ 
	else k=0; 
	/* ��ԭ���ġ�/2����Ϊ��/2.0������֤��num��16~255֮�䣬radix����16ʱ��Ҳ�ܵõ���ȷ��� */ 
	for(j=k;j<(i-1)/2.0+k;j++) 
	{ 
		num=str[j]; 
		str[j]=str[i-j-1+k]; 
		str[i-j-1+k]=num; 
	} 
	return str;
}
/*��������*/
int round(double d)
{ 
	if ((int)d+0.5>d)
	return (int)d;
	else
	return (int)d+1;
}

char* Printer::init(int len)
{
	char *dest=new char[len];
	dest[0]='\0';
	return dest;
}

void Printer::merge(char *dest,const char src[])
{
	if(dest[0]=='\0')
		strcpy(dest,src);
	else
		strcat(dest,src);
}

void Printer::merge(char *&dest,int num)
{
	char charBuffer[11];
	if(dest[0]=='\0')
		strcpy(dest,myitoa(num,charBuffer,10));
	else
		strcat(dest,myitoa(num,charBuffer,10));
}
void Printer::merge(char *&dest,float num)
{
	int temp=round(num*100);
	char charBuffer[11];
	myitoa(temp,charBuffer,10);
	int index=strlen(charBuffer);
	charBuffer[index+1]=charBuffer[index];
	charBuffer[index]=charBuffer[index-1];
	charBuffer[index-1]=charBuffer[index-2];
	charBuffer[index-2]='.';
	if(dest[0]=='\0')
		strcpy(dest,charBuffer);
	else
		strcat(dest,charBuffer);
	
}

void Printer::print(const Point3d& p)
{
	char* str=Printer::init(30);
	Printer::merge(str,"{x:");
	Printer::merge(str,p.x);
	Printer::merge(str,",y:");
	Printer::merge(str,p.y);
	Printer::merge(str,",z:");
	Printer::merge(str,p.z);
	Printer::merge(str,"}");
	
	_print_(str);
	
}
void Printer::print(const Cuboid& c)
{
	char* str=Printer::init(30);
	Printer::merge(str,"{l:");
	Printer::merge(str,c.getLength());
	Printer::merge(str,",w:");
	Printer::merge(str,c.getWidth());
	Printer::merge(str,",h:");
	Printer::merge(str,c.getHeight());
	Printer::merge(str,"}");
	
	_print_(str);
}
