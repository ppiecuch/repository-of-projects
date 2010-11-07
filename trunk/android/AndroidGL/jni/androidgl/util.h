#ifndef _UTIL_H_
#define _UTIL_H_
#include <jni.h>
#include <zip.h>
#include <png.h>
#include <stdio.h>
#include <GLES/gl.h>
#include <math.h>

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
