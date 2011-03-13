#ifndef _PRINTER_H_
#define _PRINTER_H_
#include <string.h>
#include "point3d.h"
//#include <iostream>
#include <android/log.h>


class Printer{
public:
	//初始化char数组用于merge
	static char* init(int len);
	//拼接字符串
	static void merge(char *dest,const char src[]);
	static void merge(char *&dest,int num);

	static void print(const Point3d& p);
};

#endif
