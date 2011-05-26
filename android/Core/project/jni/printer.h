#ifndef _PRINTER_H_
#define _PRINTER_H_
#include <string.h>
#include "point3d.h"
#include "cuboid.h"

#include <iostream>
#define _print_(str) std::cout<<str<<std::endl;
//#include <android/log.h>
//#define _print_(str) __android_log_print(ANDROID_LOG_INFO, "Printer",str);


class Printer{
public:
	//初始化char数组用于merge
	static char* init(int len);
	//拼接字符串
	static void merge(char *dest,const char src[]);
	static void merge(char *&dest,int num);
	static void merge(char *&dest,float num);

	static void print(const Point3d& p);
	static void print(const Cuboid& c);
};

#endif
