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
// �����ʹ�С�˻���
#define BigLittleSwap16(A)  ((((uint16)(A) & 0xff00) >> 8) | \
				(((uint16)(A) & 0x00ff) << 8))

// �����ʹ�С�˻���
#define BigLittleSwap32(A)  ((((uint32)(A) & 0xff000000) >> 24) | \
                   (((uint32)(A) & 0x00ff0000) >> 8) | \
                   (((uint32)(A) & 0x0000ff00) << 8) | \
                   (((uint32)(A) & 0x000000ff) << 24))

// ������˷���1��С�˷���0
int checkCPUendian();

// ģ��ntohs�����������ֽ���ת�����ֽ���
unsigned short ntohs(unsigned short n);

// ģ��htons�����������ֽ���ת�����ֽ���
unsigned short htons(unsigned short h);

//ͳ�ư��������ַ����ĳ���
int count(const char* s);

/**
 * ���ر�num��ģ���������ӽ�num��2�Ĵη�����
 */
int ceilPower(const int& num);
/**
 * ����·����ȡapk�ļ�
 */
zip* loadAPK (const char* apkPath);
/**
 * ��apk�е�ָ���ļ���ȡ��byte��ɫ���ݣ���Щ���ݿ�ֱ������glTexImage2D,
 * ����֮�⻹����ͼ���width,heigth��colryType(��֧��GL_RGB��GL_RGBA)
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
