#include <jni.h>
#include <stdio.h>
#include <GLES/gl.h>
#include <zipint.h>
#include <zip.h>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <freetype/ftmodapi.h>
#include "log.h"
#include "androidgl.h"
#include "util.h"
//#include "font/UnicodeFont.h"
#include "font/UnicodePainter.h"
#include "font/Font.h"
#include "encode/GBnUnicode.h"


/***************************************/
/*           FreeType中文显示           */
/***************************************/

float cameraPosition[]={0,10,40};
zip* APKArchive;
//UnicodeFont* font;
UnicodePainter* painter;
const static char* fontPath="assets/font/SIMKAI.TTF";
//const static char* text="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const static char* text="经";
//const static char* text="a";
unsigned short* unicode;
int len;

static int  sWindowWidth  = 320;
static int  sWindowHeight = 480;
static float angle=0;


void appInit(JNIEnv*  env, jobject thiz, jstring apkPath)
{
	LOGI("AndroidGL","apkPath:s%",apkPath);
	glClearColorx((GLfixed)(0.1f * 65536),(GLfixed)(0.2f * 65536),(GLfixed)(0.3f * 65536), 0x10000);

	const char* str;
	jboolean isCopy;
	str = env->GetStringUTFChars(apkPath, &isCopy);
	LOGI("AndroidGL","str:s%",*str);
	APKArchive=loadAPK(str);

	zip_file* file;
	file=zip_fopen(APKArchive, fontPath, 0);
	if(!file)
	{
		LOGE("AndroidGL","Error opening %s from APK",fontPath);
		return;
	}

	unsigned long fileSize=file->bytes_left;
	FT_Byte* fileBase=new FT_Byte[fileSize];
	zip_fread(file, fileBase, fileSize);
	LOGI("AndroidGL","zip_fread:%ld",fileSize);

	len=gb2Unicode(text,unicode);
	LOGI("AndroidGL","len:%d:unicode:%lx",len,unicode[0]);

	painter=new UnicodePainter(fileBase,fileSize,12);

	/*创建Unicode纹理*/
//	font=new UnicodeFont(fileBase,fileSize,12,12);
//	asciiFont->setCharSpacing(2);
//	asciiFont->setLineSpacing(18);
//	unsigned char color[]={0x00,0xFF,0x00,0xFF};
//	asciiFont->setColor(color);

	delete[] str;


	zip_fclose(file);

	LOGI("AndroidGL","Init",NULL);
}

void appResize(int width,int height)
{
	/*视口变换*/
	glViewport(0, 0, width, height);
	/*投影变换*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(60.0,(float)width/(float)height,0.1f,200);
	glOrthof(-sWindowWidth/2,sWindowWidth/2,-sWindowHeight/2,sWindowHeight/2,100,-100);
	/*模型矩阵*/
	glMatrixMode(GL_MODELVIEW);
	/*启用剔除功能*/
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	LOGI("AndroidGL","Resize",NULL);
}

void appRender(long tick, int width, int height)
{

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(cameraPosition[0],cameraPosition[1],cameraPosition[2],0,0,0,0,1,0);

	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
//	font->draw(text,10-sWindowWidth/2,sWindowHeight/2-40);
	painter->draw(unicode,len,10-sWindowWidth/2,sWindowHeight/2-40);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisable(GL_TEXTURE_2D);
}


/* Call to initialize the graphics state */
void
Java_org_tinder_studio_gl_AndroidGLView_nativeInit( JNIEnv*  env, jobject thiz, jstring apkPath )
{
    appInit(env,thiz,apkPath);
}


/* Call to finalize the graphics state */
void
Java_org_tinder_studio_gl_AndroidGLView_nativeDone( JNIEnv*  env, jobject thiz )
{
}

/* Call to render the next GL frame */
void
Java_org_tinder_studio_gl_AndroidGLView_nativeRender( JNIEnv*  env, jobject thiz )
{
    appRender(0, sWindowWidth, sWindowHeight);
}

void
Java_org_tinder_studio_gl_AndroidGLView_nativeResize( JNIEnv*  env, jobject  thiz, jint w, jint h )
{
    sWindowWidth  = w;
    sWindowHeight = h;
    appResize(w,h);
    LOGI("AndroidGL","resize w=%d h=%d", w, h);
}
void
Java_org_tinder_studio_gl_AndroidGLView_nativePause(JNIEnv *env, jobject thiz)
{
	LOGI("AndroidGL","Pause",0);
}

static JNINativeMethod methods[] = {

  {"nativeInit", "()", (void*)Java_org_tinder_studio_gl_AndroidGLView_nativeInit },
  {"nativeDone", "()", (void*)Java_org_tinder_studio_gl_AndroidGLView_nativeDone },
  {"nativeRender", "()", (void*)Java_org_tinder_studio_gl_AndroidGLView_nativeRender },
  {"nativeResize", "()", (void*)Java_org_tinder_studio_gl_AndroidGLView_nativeResize }

};

static const char *classPathName = "org/tinder/studio/gl/AndroidGLView";

/*
 * Register native methods for all classes we know about.
 *
 * returns JNI_TRUE on success.
 */
static int registerNatives(JNIEnv* env)
{
  if (!registerNativeMethods(env, classPathName,
                 methods, sizeof(methods) / sizeof(methods[0]))) {
    return JNI_FALSE;
  }
  return JNI_TRUE;
}

