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
#include "math/rectangle.h"
#include "androidgl.h"
#include "math/cuboid.h"
#include "util.h"

/***************************************/
/*           FreeType������ʾ           */
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
GLuint textures[1];
float cuboidPos[]={0,-8,0};
const static char* fontPath="assets/font/arial.ttf";




static int  sWindowWidth  = 320;
static int  sWindowHeight = 480;
static float angle=0;


// ����������ر�a��ģ���������ӽ�a��2�Ĵη�����
inline int next_p2 (int a ){
	int rval=1;	// rval<<=1 Is A Prettier Way Of Writing
	rval*=2;
	while(rval<a)
		rval<<=1;
	return rval;
}

//auxiliary function
void* alloc(FT_Memory p1, long p2){
    return malloc(p2);
}

//auxiliary function
void free_1(FT_Memory p1, void* p2){
    free(p2);
}

//auxiliary function
void* realloc_1(FT_Memory p1,long p2,long p3,void* p4){
    return realloc(p4, p3);
}


void appInit(JNIEnv*  env, jobject thiz, jstring apkPath)
{
	LOGI("AndroidGL","apkPath:s%",apkPath);
	glClearColorx((GLfixed)(0.1f * 65536),(GLfixed)(0.2f * 65536),(GLfixed)(0.3f * 65536), 0x10000);

	//����������
	rectangle=new Rectangle(8,10);

	/*����������*/
	cuboid=new Cuboid(CUBOID_LENGTH,CUBOID_WIDTH,CUBOID_HEIGHT);
	cuboid->toString();

	const char* str;
	jboolean isCopy;
	str = env->GetStringUTFChars(apkPath, &isCopy);
	LOGI("AndroidGL","str:s%",*str);
	APKArchive=loadAPK(str);

	int width,height,colorType;
	glGenTextures(sizeof(textures)/sizeof(textures[0]), textures);
	/*png_byte* _data;
	LOGI("AndroidGL","apply %d texture name",sizeof(textures)/sizeof(textures[0]));
	//��������2
	_data=readPngData("assets/androidgl/wood.png",width,height,colorType,APKArchive);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, colorType, width, height, 0, colorType, GL_UNSIGNED_BYTE, (GLvoid*) _data);
	delete[] _data;*/

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

	char ch='8';
	int h=16;
	int error;

	// ����FreeType��
	FT_Library library;
	/*
	 * FT_Init_FreeType
	 * Initialize a new FreeType library object.  The set of modules
	 * hat are registered by this function is determined at build time.
	 * Note:
	 * In case you want to provide your own memory allocating routines
	 * use @FT_New_Library instead, followed by a call to
	 * @FT_Add_Default_Modules (or a series of calls to @FT_Add_Module).
	 */
	if (FT_Init_FreeType( &library ))
		LOGI("AndroidGL","FT_Init_FreeType ERROR",NULL);

	/*
	 * FT_New_Library
	 * This function is used to create a new FreeType library instance
	 * from a given memory object.  It is thus possible to use libraries
	 * with distinct memory allocators within the same program.
	 *
	 * Normally, you would call this function (followed by a call to
	 * @FT_Add_Default_Modules or a series of calls to @FT_Add_Module)
	 * instead of @FT_Init_FreeType to initialize the FreeType library.
	 *
	 * Don't use @FT_Done_FreeType but @FT_Done_Library to destroy a
	 *  library instance.
	 */
	/*FT_Memory memory;
	memory->user = file_base;
	memory->alloc = &alloc;
	memory->free = &free_1;
	memory->realloc = &realloc_1;
	if(FT_New_Library(memory, library))
	{
		LOGE("AndroidGL","FT_New_Library ERROR:%d",error);
		return;
	}*/

	/*
	 * FT_Add_Default_Modules
	 * Add the set of default drivers to a given library object.
	 * This is only useful when you create a library object with
	 * @FT_New_Library (usually to plug a custom memory manager)*/

	//FT_Add_Default_Modules(library);


	// ��FreeType���б���������Ϣ�������face
	FT_Face face;

	// ʹ���������Freetype�ַ��ļ���ʼ��face��
	//if (FT_New_Face( library, "����", 0, &face ))
	//	LOGI("AndroidGL","FT_New_Face ERROR",NULL);
	/*
	 * This function calls @FT_Open_Face to open a font which has been
	 * loaded into memory.
	 */
	if(error=FT_New_Memory_Face(library,file_base,file_size,0,&face))
	{
		LOGI("AndroidGL","FT_New_Memory_Face ERROR:%d",error);
		return;
	}

	// ��FreeType��ʹ��1/64��Ϊһ�����صĸ߶�����������Ҫ����h���������Ҫ��
	FT_Set_Char_Size( face, h << 6, h << 6, 96,96);
	//FT_Set_Pixel_Sizes(face,h,h);//This function calls @FT_Request_Size to request the nominal size (in pixels).


	if(FT_Load_Char( face, FT_Get_Char_Index( face, ch ), FT_LOAD_DEFAULT ))
	//if(FT_Load_Char( face, FT_Get_Char_Index( face, ch ), FT_LOAD_RENDER ))
	//if(FT_Load_Char(face, ch,FT_LOAD_RENDER|FT_LOAD_FORCE_AUTOHINT|
		//	(true ? FT_LOAD_TARGET_NORMAL : FT_LOAD_MONOCHROME | FT_LOAD_TARGET_MONO)))
			LOGI("AndroidGL","FT_Load_Char ERROR",NULL);

	// ��������ַ�������
	/*if(FT_Load_Glyph( face, FT_Get_Char_Index( face, ch ), FT_LOAD_DEFAULT ))
		LOGI("AndroidGL","FT_Load_Glyph ERROR",NULL);*/

	// ������������
	/*FT_Glyph glyph;
	if(FT_Get_Glyph(face->glyph, &glyph ))
		LOGI("AndroidGL","FT_Get_Glyph ERROR",NULL);*/

	// ������ת��Ϊλͼ
	//FT_Render_Glyph( face->glyph,   FT_RENDER_MODE_NORMAL );//FT_RENDER_MODE_LCD  );
	FT_Render_Glyph( face->glyph,   FT_RENDER_MODE_MONO );
	/*FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
	LOGI("AndroidGL","FT_Glyph_To_Bitmap",NULL);*/

	FT_GlyphSlot  slot = face->glyph;
	FT_Bitmap bitmap=slot->bitmap;

	// ����λͼ
	//FT_Bitmap& bitmap=bitmap_glyph->bitmap;

	// ת��ΪOpenGl����ʹ�õĴ�С
	width = next_p2( bitmap.width );
	height = next_p2( bitmap.rows );
	LOGI("AndroidGL","width:%d,height:%d,pixel_mode:%d",width,height,bitmap.pixel_mode);

	// ����λͼ����
//	GLubyte* data = new GLubyte[ 2 * width * height];

	// ��������ʹ��8λ��ʾ����8λ��ʾalphaֵ
//	for(int j=0; j <height;j++) {
//		for(int i=0; i < width; i++){
//			data[2*(i+j*width)]= data[2*(i+j*width)+1] =(i>=bitmap.width||j>=bitmap.rows)?0:bitmap.buffer[i + bitmap.width*j];
//			LOGI("AndroidGL","%d,%d",data[2*(i+j*width)],bitmap.buffer[i + bitmap.width*j]);
//		}
//	}


	unsigned char color;
	GLubyte* data = new GLubyte[width * height*4];
	for(int j=0; j <height;j++) {
		for(int i=0; i < width; i++){
			if((bitmap.buffer[j * bitmap.pitch + i/8] & (0xC0 >> (i % 8))) == 0)
				color=0xff;
			else
				color=0x00;
			data[(i+j*width)*4]=color;
			data[(i+j*width)*4+1]=color;
			data[(i+j*width)*4+2]=color;
			data[(i+j*width)*4+3]=0xff;

//			for(int k=0;k<8;k++){
//				if(bitmap.buffer[i + bitmap.width*j]&1<<(7-k)){
//					color=0x00;
//				}else{
//					color=0xff;
//				}

//				data[((i+j*width)*8+k)*4]=color;
//				data[((i+j*width)*8+k)*4+1]=color;
//				data[((i+j*width)*8+k)*4+2]=color;
//				data[((i+j*width)*8+k)*4+3]=0xff;
//				data[(i+j*width)*4]=bitmap.buffer[i + bitmap.pitch*j];
//				data[(i+j*width)*4+1]=bitmap.buffer[i + bitmap.pitch*j];
//				data[(i+j*width)*4+2]=bitmap.buffer[i + bitmap.pitch*j];
//				data[(i+j*width)*4+3]=0xff;

//			}
			//data[i+j*width]=bitmap.buffer[i + bitmap.width*j];
			//LOGI("AndroidGL","%d,%d",data[i+j*width],bitmap.buffer[i + bitmap.width*j]);
		}
	}
	//http://hi.baidu.com/s_rlzheng/blog/item/0e3ef452c0d7f7060df3e3a3.html

	/*char* data = new char[width*height*4];
	for(int j=0; j  < height ; j++)
	{
		unsigned char* str=new unsigned char[width+1];
		for(int i=0; i < width; i++)
		{
			unsigned char _vl =  (i>=bitmap.width || j>=bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width*j];
			data[(4*i + (height - j - 1) * width * 4)  ] = 0xff;
			data[(4*i + (height - j - 1) * width * 4)+1] = 0xff;
			data[(4*i + (height - j - 1) * width * 4)+2] = 0xff;
			data[(4*i + (height - j - 1) * width * 4)+3] = _vl;
			str[i]=_vl;
		}
		str[width]='\0';
		LOGI("AndroidGL","%s",str);
	}*/

	/*switch (bitmap.pixel_mode)
	     {
	      case FT_PIXEL_MODE_GRAY:
	       {
	        for(int k = 0; k < bitmap.width; k++ )
	        {
	         pixelclr = buffer[j * face->glyph->bitmap.pitch + k];
	          // ����ʹ��pixelclr��Ϊalphaֵ

	         // pDest����ʾ�õ�image�ڴ�ָ��
	         *pDest++= pixelclr;
	       }
	       break;

	      case FT_PIXEL_MODE_MONO:

	      //�ܶ��˲�֪�������Ӧ�ľ���fix sizeʱ��ͼƬ��ʽ��ÿ��bit��Ӧһ���㣬�Ǻڼ���

	       for(int k = 0; k <bitmap.width; k++ )
	       {
	        pixelclr = (src [k / 8] & (0x80 >> (k & 7))) ? 0xFFFFFFFF : 0x00000000;
	        // pDest����ʾ�õ�image�ڴ�ָ��
	         *pDest++= pixelclr;
	       }
	       break;

	      default:
	       //throw InvalidRequestException("Error - unsupported pixel mode");
	       break;
	     }*/


	//��������CSDN���ͣ�ת�������������http://blog.csdn.net/vrix/archive/2009/09/28/4601832.aspx

	// ����������������������
	//glGenTextures(sizeof(textures)/sizeof(textures[0]), textures);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// �����
	//glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, data);
	glTexImage2D( GL_TEXTURE_2D,0,GL_RGBA,width, height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
	//glTexImage2D( GL_TEXTURE_2D,0,GL_ALPHA,width, height,0,GL_ALPHA,GL_UNSIGNED_BYTE,data);

	// �ͷŷ�����ڴ�
	delete[] data;

	// �ͷ�face��
	FT_Done_Face(face);

	// �ͷ�FreeType��
	FT_Done_FreeType(library);

	zip_fclose(file);

	LOGI("AndroidGL","Init",NULL);
}

void appResize(int width,int height)
{
	/*�ӿڱ任*/
	glViewport(0, 0, width, height);
	/*ͶӰ�任*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0,(float)width/(float)height,0.1f,200);
	/*ģ�;���*/
	glMatrixMode(GL_MODELVIEW);
	/*�����޳�����*/
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
//	glRotatef(angle,1,1,0);
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
//	angle+=1.5f;
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

