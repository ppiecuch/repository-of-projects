#ifndef _UTIL_H_
#define _UTIL_H_
#include <jni.h>
#include <zip.h>
#include <png.h>
#include <stdio.h>
#include <GLES/gl.h>
#include <math.h>

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
