#ifndef _PRINTER_H_
#define _PRINTER_H_
#include <string.h>
#include "point3d.h"
//#include <iostream>
#include <android/log.h>


class Printer{
public:
	//��ʼ��char��������merge
	static char* init(int len);
	//ƴ���ַ���
	static void merge(char *dest,const char src[]);
	static void merge(char *&dest,int num);

	static void print(const Point3d& p);
};

#endif
