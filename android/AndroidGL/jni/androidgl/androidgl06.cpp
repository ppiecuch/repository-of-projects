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
#include "log.h"
#include "math/rectangle.h"
#include "androidgl.h"
#include "math/cuboid.h"
#include "util.h"

/***************************************/
/*           FreeType文字显示           */
/***************************************/

float cameraPosition[]={0,10,40};
static float cameraAngle=0;
static float cameraSin;
static float cameraCos;
static void rotateCamera()
{
	cameraAngle-=1;
	if(cameraAngle<=0)
		cameraAngle+=360;
	cameraSin=sinf(cameraAngle*M_PI/180.0);
	cameraCos=cosf(cameraAngle*M_PI/180.0);
	float _x= cameraCos - 40*cameraSin;
	float _z= cameraSin + 40*cameraCos;
	cameraPosition[0]=_x;
	cameraPosition[2]=_z;
}

static GLfloat axeVertexArray[][3] = {
	{0,0,0},
	{10,0,0},
	{0,0,0},
	{0,10,0},
	{0,0,0},
	{0,0,10}
};
static GLubyte axeColorArray[][3] = {
  255, 0, 0, 255,
  255, 0, 0, 255,
  0, 255, 0, 255,
  0, 255, 0, 255,
  0, 0, 255, 255,
  0, 0, 255, 255
};

#define CUBOID_LENGTH 8
#define CUBOID_WIDTH 8
#define CUBOID_HEIGHT 8

Rectangle* rectangle;
Cuboid* cuboid;
zip* APKArchive;
GLuint textures[2];
float cuboidPos[]={0,-8,0};
const static char* fontPath="assets/font/BankGothic.TTF";




static int  sWindowWidth  = 320;
static int  sWindowHeight = 480;
static float angle=0;


// 这个函数返回比a大的，并且是最接近a的2的次方的数
inline int next_p2 (int a ){
	int rval=1;	// rval<<=1 Is A Prettier Way Of Writing
	rval*=2;
	while(rval<a)
		rval<<=1;
	return rval;
}

void appInit(JNIEnv*  env, jobject thiz, jstring apkPath)
{
	LOGI("AndroidGL","apkPath:s%",apkPath);
	glClearColorx((GLfixed)(0.1f * 65536),(GLfixed)(0.2f * 65536),(GLfixed)(0.3f * 65536), 0x10000);

	//创建长方形
	rectangle=new Rectangle(8,10);

	/*创建长方体*/
	cuboid=new Cuboid(CUBOID_LENGTH,CUBOID_WIDTH,CUBOID_HEIGHT);
	cuboid->toString();

	const char* str;
	jboolean isCopy;
	str = env->GetStringUTFChars(apkPath, &isCopy);
	LOGI("AndroidGL","str:s%",*str);
	APKArchive=loadAPK(str);

	int width,height,colorType;
	png_byte* _data;
	LOGI("AndroidGL","apply %d texture name",sizeof(textures)/sizeof(textures[0]));
	glGenTextures(sizeof(textures)/sizeof(textures[0]), textures);
	/*创建纹理2*/
	_data=readPngData("assets/androidgl/wood.png",width,height,colorType,APKArchive);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, colorType, width, height, 0, colorType, GL_UNSIGNED_BYTE, (GLvoid*) _data);
	delete[] _data;

	zip_file* file;
	file=zip_fopen(APKArchive, fontPath, 0);
	if(!file)
	{
		LOGE("AndroidGL","Error opening %s from APK",fontPath);
		return;
	}

	unsigned long file_size=file->bytes_left;
	LOGI("AndroidGL","file_size:%ld",file_size);
	FT_Byte* file_base=new FT_Byte[file_size];
	LOGI("AndroidGL","new FT_Byte",NULL);
	zip_fread(file, file_base, file_size);
	LOGI("AndroidGL","zip_fread",NULL);

	char ch='A';
	int h=16;
	int error;

	// 创建FreeType库
	FT_Library library;
	if (FT_Init_FreeType( &library ))
		LOGI("AndroidGL","FT_Init_FreeType ERROR",NULL);

	// 在FreeType库中保存字体信息的类叫做face
	FT_Face face;

	// 使用你输入的Freetype字符文件初始化face类
	//if (FT_New_Face( library, "宋体", 0, &face ))
	//	LOGI("AndroidGL","FT_New_Face ERROR",NULL);
	if(error=FT_New_Memory_Face(library,file_base,file_size,0,&face))
	{
		LOGI("AndroidGL","FT_New_Memory_Face ERROR:%d",error);
		return;
	}

	// 在FreeType中使用1/64作为一个像素的高度所以我们需要缩放h来满足这个要求
	FT_Set_Char_Size( face, h << 6, h << 6, 96, 96);


	if(FT_Load_Char( face, FT_Get_Char_Index( face, ch ), FT_LOAD_RENDER ))
			LOGI("AndroidGL","FT_Load_Char ERROR",NULL);

	FT_GlyphSlot  slot = face->glyph;
	FT_Bitmap bitmap=slot->bitmap;

	/*// 载入给定字符的轮廓
	if(FT_Load_Glyph( face, FT_Get_Char_Index( face, ch ), FT_LOAD_DEFAULT ))
		LOGI("AndroidGL","FT_Load_Glyph ERROR",NULL);

	// 保存轮廓对象
	FT_Glyph glyph;
	if(FT_Get_Glyph(face->glyph, &glyph ))
		LOGI("AndroidGL","FT_Get_Glyph ERROR",NULL);

	// 把轮廓转化为位图
	FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
	LOGI("AndroidGL","FT_Glyph_To_Bitmap",NULL);

	// 保存位图
	FT_Bitmap& bitmap=bitmap_glyph->bitmap;*/

	// 转化为OpenGl可以使用的大小
	width = next_p2( bitmap.width );
	height = next_p2( bitmap.rows );
	LOGI("AndroidGL","width:%d,height:%d",width,height);

	// 保存位图数据
//	GLubyte* data = new GLubyte[ 2 * width * height];

	// 这里我们使用8位表示亮度8位表示alpha值
//	for(int j=0; j <height;j++) {
//		for(int i=0; i < width; i++){
//			data[2*(i+j*width)]= data[2*(i+j*width)+1] =(i>=bitmap.width||j>=bitmap.rows)?0:bitmap.buffer[i + bitmap.width*j];
//			LOGI("AndroidGL","%d,%d",data[2*(i+j*width)],bitmap.buffer[i + bitmap.width*j]);
//		}
//	}

	GLubyte* data = new GLubyte[width * height];
	for(int j=0; j <height;j++) {
		for(int i=0; i < width; i++){
			data[i+j*width]=bitmap.buffer[i + bitmap.width*j];
			//LOGI("AndroidGL","%d,%d",data[2*(i+j*width)],bitmap.buffer[i + bitmap.width*j]);
		}
	}

	// 设置字体纹理的纹理过滤器
	//glGenTextures(sizeof(textures)/sizeof(textures[0]), textures);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	// 邦定纹理
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,GL_RGB, GL_UNSIGNED_BYTE, data);

	// 释放分配的内存
	delete[] data;

	// 释放face类
	FT_Done_Face(face);

	// 释放FreeType库
	FT_Done_FreeType(library);

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
	gluPerspective(60.0,(float)width/(float)height,0.1f,200);
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
//
//	glVertexPointer(2,GL_FLOAT,0,rectangle->getVertexs());
//	glTexCoordPointer(2, GL_FLOAT, 0, rectangle->texCoords);
//	glBindTexture(GL_TEXTURE_2D,textures[1]);
//	glDrawElements(GL_TRIANGLE_STRIP,4,GL_UNSIGNED_BYTE,rectangle->vertexIndices[0]);

	glTranslatef(cuboidPos[0],cuboidPos[1],cuboidPos[2]);
	glRotatef(angle,1,1,0);
	glVertexPointer(3,GL_FLOAT,0,cuboid->getVertexs());
	glTexCoordPointer(2, GL_SHORT, 0, cuboid->texCoords);
	glBindTexture(GL_TEXTURE_2D,textures[0]);
//	glNormalPointer(GL_SHORT,0,cuboid->normals);
//	glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);
//	glMaterialfv(GL_FRONT,GL_SHININESS,materialShininess);
//	glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffuse);
	for(int i=0;i<6;i++)
			glDrawElements(GL_TRIANGLE_STRIP,4,GL_UNSIGNED_BYTE,cuboid->vertexIndices[i]);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisable(GL_TEXTURE_2D);
	angle+=1.5f;
	//rotateCamera();
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

