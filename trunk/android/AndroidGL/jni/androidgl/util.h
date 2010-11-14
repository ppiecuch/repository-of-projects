#ifndef _UTIL_H_
#define _UTIL_H_
#include <jni.h>
#include <zip.h>
#include <png.h>
#include <stdio.h>
#include <GLES/gl.h>
#include <math.h>

#define RED_IN_LONG 0xFF000000
#define GREEN_IN_LONG 0x00FF0000
#define BLUE_IN_LONG 0x0000FF00
#define ALPHA_IN_LONG 0x000000FF

typedef unsigned short int uint16;
typedef unsigned long int uint32;
// 短整型大小端互换
#define BigLittleSwap16(A)  ((((uint16)(A) & 0xff00) >> 8) | \
				(((uint16)(A) & 0x00ff) << 8))

// 长整型大小端互换
#define BigLittleSwap32(A)  ((((uint32)(A) & 0xff000000) >> 24) | \
                   (((uint32)(A) & 0x00ff0000) >> 8) | \
                   (((uint32)(A) & 0x0000ff00) << 8) | \
                   (((uint32)(A) & 0x000000ff) << 24))

// 本机大端返回1，小端返回0
int checkCPUendian();

// 模拟ntohs函数，网络字节序转本机字节序
unsigned short ntohs(unsigned short n);

// 模拟htons函数，本机字节序转网络字节序
unsigned short htons(unsigned short h);

//统计包含中文字符串的长度
int count(const char* s);

/**
 * 返回比num大的，并且是最接近num的2的次方的数
 */
int ceilPower(const int& num);
/**
 * 根据路径读取apk文件
 */
zip* loadAPK (const char* apkPath);
/**
 * 从apk中的指定文件读取出byte颜色数据，这些数据可直接用于glTexImage2D,
 * 除此之外还返回图像的width,heigth及colryType(暂支持GL_RGB跟GL_RGBA)
 */
png_byte* readPngData(const char* filename,int& width,int& height,int& colorType,zip* apk);


void gluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar);
void gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
	              GLfloat centerx, GLfloat centery, GLfloat centerz,
	              GLfloat upx, GLfloat upy, GLfloat upz);

long floatToFixed(float value);
void printGLInteger(const char *name, GLenum s);
void printGLString(const char *name, GLenum s);

/*
* Register several native methods for one class.
*/

int registerNativeMethods(JNIEnv* env, const char* className,
    JNINativeMethod* gMethods, int numMethods);

// ----------------------------------------------------------------------------
/*
 * This is called by the VM when the shared library is first loaded.
 */
typedef union {
    JNIEnv* env;
    void* venv;
} UnionJNIEnvToVoid;
#endif
