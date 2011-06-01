// jni.cpp : 定义控制台应用程序的入口点。
/*
#define _WIN_
#include "stdafx.h"
#include <iostream>
#include <string.h>
#include <math.h>
//#include "printer.h"
//#include "point3d.h"
//#include "point3f.h"
//#include "cuboid.h"
//#include "point2.h"
#include "freetype.h"
using namespace std;

#define WIDTH   640
#define HEIGHT  480
//origin is the upper left corner 
unsigned char image[HEIGHT][WIDTH];

void draw_bitmap(FT_Bitmap* bitmap,FT_Int x,FT_Int y)
{
	FT_Int i,j,p,q;
	FT_Int x_max = x+bitmap->width;
	FT_Int y_max = y+bitmap->rows;

	for (i=x,p=0;i<x_max;i++,p++ )
	{
		for ( j = y, q = 0; j < y_max; j++, q++ )
		{
			if (i<0||j<0||i>=WIDTH||j>=HEIGHT)
			continue;
			image[j][i] |= bitmap->buffer[q * bitmap->width + p];
		}
	}
}

void show_image(void)
{
  int i,j;
  for(i=0;i<HEIGHT;i++ )
  {
    for (j=0;j<WIDTH;j++ )
      putchar( image[i][j] == 0 ? ' ': image[i][j] < 128 ? '+': '*' );
    putchar( '\n' );
  }
}


int _tmain(int argc, _TCHAR* argv[])
{
	//Cuboid* c=new Cuboid(10.5f,12.66f,30);
	//Printer::print(*c);
	//delete c;
	//char ch[11];
	//ch[0]='\0';
	//cout<<strlen(ch)<<endl;
	//Point2<int> p1;
	//Point2<int> *p2=new Point2<int>(5,5);
	//cout<<p1<<endl;
	//cout<<*p2<<endl;
	//p1-=*p2;
	//cout<<p1<<endl;
	//cout<<&p1<<endl;
	//return 0;
	FT_Library    library;
  FT_Face       face;

  FT_GlyphSlot  slot;
  FT_Matrix     matrix;                 //transformation matrix 
  FT_Vector     pen;                    //untransformed origin  
  FT_Error      error;

  char*         filename;
  char*         text;

  double        angle;
  int           target_height;
  int           n, num_chars;




  filename      = "test.TTF";                          //first argument     
  text          = "china";                           //second argument    
  num_chars     = strlen( text );
  angle         = ( 25.0 / 360 ) * 3.14159 * 2;      //use 25 degrees     
  target_height = HEIGHT;

  error = FT_Init_FreeType( &library );              // initialize library 
  // error handling omitted 

  error = FT_New_Face( library, filename, 0, &face ); // create face object 
  // error handling omitted 

  // use 50pt at 100dpi 
  error = FT_Set_Char_Size( face, 50 * 64, 0,
                            100, 0 );                // set character size 
  // error handling omitted 

  slot = face->glyph;

 // set up matrix 
  matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
  matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
  matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
  matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );

  // the pen position in 26.6 cartesian space coordinates; 
  // start at (300,200) relative to the upper left corner  
  pen.x = 300*64;
  pen.y = (target_height-200) * 64;

  for ( n = 0; n<num_chars; n++ )
  {
   //set transformation 
    FT_Set_Transform( face, &matrix, &pen );

    // load glyph image into the slot (erase previous one) 
    error = FT_Load_Char( face, text[n], FT_LOAD_RENDER );
    if ( error )
      continue;                // ignore errors 

   // now, draw to our target surface (convert position) 
    draw_bitmap( &slot->bitmap,slot->bitmap_left,target_height-slot->bitmap_top );

    // increment pen position 
    pen.x+=slot->advance.x;
    pen.y+=slot->advance.y;
  }

  show_image();

  FT_Done_Face(face);
  FT_Done_FreeType(library);
  return 0;
}

*/



#include <windows.h>		// Header File For Windows
#include <math.h>			// Header File For Windows Math Library
#include <stdio.h>			// Header File For Standard Input/Output
#include <stdarg.h>			// Header File For Variable Argument Routines
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

#include "point2.h"
#include "AsciiBrush.h"
//#include "hashmap.h"

#include "freetype.h"

#include <wtypes.h>  
#include <stdarg.h>  
#include <tchar.h>   

#define WIDTH 640
#define HEIGHT 480



void LogTrace(LPCTSTR pszFormat, ...)   
{
    va_list pArgs;   

    char szMessageBuffer[16380]={0};   
    va_start( pArgs, pszFormat );   
    _vsntprintf( szMessageBuffer, 16380, pszFormat, pArgs );   
    va_end( pArgs );   
  
    OutputDebugString(szMessageBuffer);   
}  

HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

GLuint	base;				// Base Display List For The Font Set
GLfloat	cnt1;				// 1st Counter Used To Move Text & For Coloring
GLfloat	cnt2;				// 2nd Counter Used To Move Text & For Coloring

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default

GLuint textures[256];
GLubyte** buffers;		//灰度缓冲区（即glyph-bitmap->buffer数据，用于创建纹理），每个缓冲区的行数列数可以从deltas中获得
Point2<int> deltas[256];	//字符的长宽
Point2<float>** texCoords;	//字符的纹理坐标,注意有些字符的长宽过大的情况，比如第一个字符长度为4，第二字符的长度为５（经过2的幂扩大后为8）
Point2<int> offsets[256];	//字符的偏移（因为每个字符的宽高不同）
Point2<int> advances[256];	//字符的步进（因为每个字符的宽高不同）
int color[]={128,0,255,255}; //默认颜色为黑色
Point2<int> vertex[4];
const unsigned char vertexIndices[]={0,1,2,3};
char* str="abcdefghijklmnopqrstuvwxyz0123456789";


//返回比num大的，并且是最接近num的2的次方的数
int ceilPower(const int& num)
{
	int rval=1;
	rval*=2;
	while(rval<num)
		rval<<=1;
	return rval;
}

void generateChar(const int& index,const int& x,const int& y)
{

	/*
	 * -----------------------------<---基线与此线距离为top，即offsets[index].y
	 * -----------------------------
	 * 	a...x...z
	 * -----------------------------<---基线
	 * -----------------------------
	 *
	 *	|  |
	 *	| a|
	 *	|  |
	 *	+
	 *	|
	 *	|
	 *	left 为字符与此线的距离，字符在右为负数，在左为正数
	 */

	vertex[0].x=x+offsets[index].x;
	vertex[0].y=y+offsets[index].y-deltas[index].y;
	vertex[1].x=x+offsets[index].x+deltas[index].x;
	vertex[1].y=y+offsets[index].y-deltas[index].y;
	vertex[2].x=x+offsets[index].x;
	vertex[2].y=y+offsets[index].y;
	vertex[3].x=x+offsets[index].x+deltas[index].x;
	vertex[3].y=y+offsets[index].y;

	/*vertex[0].x=x+offsets[index].x;
	vertex[0].y=y+advances[index].y;
	vertex[1].x=x+advances[index].x;
	vertex[1].y=y+advances[index].y;
	vertex[2].x=x+offsets[index].x;
	vertex[2].y=y+offsets[index].y;
	vertex[3].x=x+advances[index].x;
	vertex[3].y=y+offsets[index].y;*/

	/*int width=deltas[index].x;
	int height=deltas[index].y;
	int newWidth=ceilPower(width);
	int newHeight=ceilPower(height);
	vertex[0].x=x+offsets[index].x;
	vertex[0].y=y+offsets[index].y-newHeight;
	vertex[1].x=x+offsets[index].x+newWidth;
	vertex[1].y=y+offsets[index].y-newHeight;
	vertex[2].x=x+offsets[index].x;
	vertex[2].y=y+offsets[index].y;
	vertex[3].x=x+offsets[index].x+newWidth;
	vertex[3].y=y+offsets[index].y;*/

	/*vertex[0].x=x;
	vertex[0].y=y-deltas[index].y;
	vertex[1].x=x+deltas[index].x;
	vertex[1].y=y-deltas[index].y;
	vertex[2].x=x;
	vertex[2].y=y;
	vertex[3].x=x+deltas[index].x;
	vertex[3].y=y;*/
}

void generateTexture(const int& ch)
{
	GLubyte* buffer=buffers[ch];
	if(buffer==NULL)
	{
		LogTrace("\r\ngenerateTexture(%d,%ulx) fail,for buffer==NULL",ch,color);
		return;
	}
	//获取当前设置颜色值
	GLfloat cc[4];
	glGetFloatv(GL_CURRENT_COLOR,cc);
	LogTrace("\r\ngetColor:%.2f,%.2f,%.2f,%.2f",cc[0],cc[1],cc[2],cc[3]);
	//将float转为int
	for(int i=0;i<4;i++)
	{
		color[i]=cc[i]*255;
		if(color[i]>0xff)
			color[i]=0xff;
		if(color[i]<0)
			color[i]=0;
	}
	LogTrace("\r\ngetColor:%x,%x,%x,%x",color[0],color[1],color[2],color[3]);
	//获取颜色分量
	int red=0xFF&color[0];
	int green=0xFF&color[1];
	int blue=0xFF&color[2];
	int alpha=0xFF&color[3];

	int width=deltas[ch].x;
	int height=deltas[ch].y;
	int newWidth=ceilPower(width);
	int newHeight=ceilPower(height);
	GLubyte* data=new GLubyte[newWidth*newHeight*4];
	int bufferIndex=0;
	int value;
	int row;
	int column;
	for(int i=0; i<newHeight;i++) {
		row=(newHeight-i-1)*newWidth;
		for(int j=0; j<newWidth; j++){
			column=row+j<<2;
			//因为width跟height是经过对bitmap.width,bitmap.rows进行ceilPower扩大后得到的，
			//所以对于扩出的部分应该用0表示
			value=(i>=height||j>=width)?0:buffer[bufferIndex++];
			//data[column]=value*red>>8;
			//data[column+1]=value*green>>8;
			//data[column+2]=value*blue>>8;
			//data[column+3]=value*alpha>>8;

			data[column]=red&0xff;
			data[column+1]=green&0xff;
			data[column+2]=blue&0xff;
			data[column+3]=value;
		}
	}

	//设置字体纹理的纹理过滤器
	glBindTexture(GL_TEXTURE_2D, textures[ch]);
	
	//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//glTexEnvi(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	glTexImage2D( GL_TEXTURE_2D,0,GL_RGBA,newWidth,newHeight,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
	//glTexImage2D( GL_TEXTURE_2D,0,GL_RGBA,newWidth,newHeight,0,GL_LUMINANCE_ALPHA,GL_UNSIGNED_BYTE,data);

	delete[] data;
}

void init()
{
	//char* fname="times.ttf";
	char* fname="Swiss 721 Light Condensed Italic BT.ttf";
	int h=13;
	int error;
	

	//Create and initilize a freetype font library.
	FT_Library library;
	if (FT_Init_FreeType( &library )) 
		throw std::runtime_error("FT_Init_FreeType failed");

	//The object in which Freetype holds information on a given
	//font is called a "face".
	FT_Face face;

	//This is where we load in the font information from the file.
	//Of all the places where the code might die, this is the most likely,
	//as FT_New_Face will die if the font file does not exist or is somehow broken.
	if (FT_New_Face( library, fname, 0, &face )) 
	{
		// 释放FreeType库
		FT_Done_FreeType(library);
		throw std::runtime_error("FT_New_Face failed (there is probably a problem with your font file)");
	}

	//For some twisted reason, Freetype measures font size
	//in terms of 1/64ths of pixels.  Thus, to make a font
	//h pixels high, we need to request a size of h*64.
	//(h << 6 is just a prettier way of writting h*64)
	//96为设备水平分辨率与设备垂直分辨率,为0时表示使用默认值72dpi。
	//这个函数计算对应字符宽度、高度和设备分辨率的字符象素大小。
	//如果你想自己指定象素大小，你可以简单地调用FT_Set_Pixel_Sizes
	/*if(FT_Set_Char_Size( face, h << 6, h << 6, h*WIDTH/72, h*HEIGHT/72))
	//if(FT_Set_Char_Size( face, h << 6, h << 6, 96,96))
	{
		// 释放face类
		FT_Done_Face(face);
		// 释放FreeType库
		FT_Done_FreeType(library);
		throw std::runtime_error("FT_Set_Char_Size failed ");
	}*/

	FT_Set_Pixel_Sizes(face,18,18);

	int index;
	FT_GlyphSlot slot=face->glyph;
	FT_Glyph_Metrics metrics=face->glyph->metrics;
	FT_Glyph glyph;
	//基线到字符轮廓最高点的距离, 由于是26.6的定点数，因此获取整数部分需要除以64
	int ascender = face->size->metrics.ascender>>6;

	//生成256个ASCII码字符纹理的名称
	glGenTextures(256, textures);
	//生成256个纹理坐标
	texCoords=(Point2<float>**)new Point2<float>*[256];
	//创建灰度缓冲
	buffers=(GLubyte**)new GLubyte*[256];

	//生成256个字符的纹理
	for(int c=0;c<256;c++)
	{
		//获取字符c在face的字符映射表中的glyph索引
		index=FT_Get_Char_Index(face,c);
		//将灰度缓冲预设为NULL
		buffers[c]=NULL;
		if(index==0)
		{
			//释放纹理名称
			glDeleteTextures(1,&textures[c]);
			textures[c]=-1;
			texCoords[c]=NULL;
			LogTrace("\r\nFT_Get_Char_Index:%d-->%d(undefined character code),so delete texture",c,index);
			continue;
		}
		//根据load_flags的设置从face读取glyph并保存到glyph slot中
		//这个函数会设法从face中装载对应的字形图像:如果找到一个对应该字形和象素尺寸的位图，
		//那么它将会被装载到字形槽中。嵌入的位图总是比原生的图像格式优先装载。
		//因为我们假定对一个字形，它有更高质量的版本。这可以用FT_LOAD_NO_BITMAP标志来改变。
		//否则，将装载一个该字形的原生图像，把它伸缩到当前的象素尺寸，并且对应如TrueType和Type1这些格式，也会完成hinted操作。
		if(error=FT_Load_Glyph( face, index, FT_LOAD_DEFAULT ))
		{
			LogTrace("\r\nFT_Load_Glyph ERROR:%d",error);
			continue;
		}
		//从face->glyph即face的glyph slot中提取glyph对象
		if(error=FT_Get_Glyph(face->glyph, &glyph))
		{
			LogTrace("\r\nFT_Get_Glyph ERROR:%d",error);
			continue;
		}
		//检测glyph的图像格式并调用相应的渲染器将glyph渲染成位图
		//FT_RENDER_MODE_NORMAL：1像素使用8位灰度级的抗锯齿位图
		//FT_RENDER_MODE_MONO：1像素使用1位灰度级的位图
		if(error=FT_Render_Glyph( face->glyph, FT_RENDER_MODE_LCD))
		//if(error=FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL))
		{
			LogTrace("\r\nFT_Render_Glyph ERROR:%d",error);
			continue;
		}
		//字段face->glyph->format描述了字形槽中存储的字形图像的格式。如果它的值不是FT_GLYPH_FORMAT_BITMAP，你可以通过FT_Render_Glyph把它直接转换为一个位图。
		//如果glyph图像不是位图
		if ( glyph->format != FT_GLYPH_FORMAT_BITMAP )
		{
			//转换glyph对象为位图glyph对象并替换原glyph对象,0表无偏移，1表成功后销毁原glyph
			if(error=FT_Glyph_To_Bitmap( &glyph, FT_RENDER_MODE_NORMAL, 0, 1 ))
			{
				LogTrace("\r\nFT_Glyph_To_Bitmap ERROR:%d",error);
				continue;
			}
		}


		FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph;
		FT_Bitmap bitmap=bitmapGlyph->bitmap;
		//因为OpenGL只支持纹理长度和宽度是2的n次方，所以这里要转换一下
		int width=ceilPower(bitmap.width);
		int height=ceilPower(bitmap.rows);

		//bitmap.pixel_mode = FT_PIXEL_MODE_GRAY;

		//复制位图数据到灰度缓冲区
		buffers[c] = new GLubyte[bitmap.width * bitmap.rows];
		memcpy(buffers[c],bitmap.buffer,bitmap.width * bitmap.rows);

		//保存长宽信息
		deltas[c].x=bitmap.width;
		deltas[c].y=bitmap.rows;

		//保存偏移
		offsets[c].x=bitmapGlyph->left;// 字符距离左边界的距离
		offsets[c].y=bitmapGlyph->top;// 字符最高点距离基线的距离
		advances[c].x=slot->advance.x>>6;
		advances[c].y=slot->advance.y>>6;
		//LogTrace("\r\n----------%c--------",c);
		//LogTrace("\r\ndeltas[c]:%d,%d",deltas[c].x,deltas[c].y);
		//LogTrace("\r\noffsets[c]:%d,%d",offsets[c].x,offsets[c].y);
		//LogTrace("\r\nslot:%d,%d,%d,%d",slot->bitmap_left,slot->bitmap_top,advances[c].x,advances[c].y);


		//保存纹理坐标，由于width、height与字符的真实轮廓大小不一定相等，所以要调整纹理坐标
		float dx=(float)bitmap.width/width;
		float dy=(float)bitmap.rows/height;
		dy=1-dy;//因为y被我们反转了，即着色是从上往下的，所以空白是在data块上方
		texCoords[c]=new Point2<float>[4];
		texCoords[c][0].x=0.0f;
		texCoords[c][0].y=dy;
		texCoords[c][1].x=dx;
		texCoords[c][1].y=dy;
		texCoords[c][2].x=0.0f;
		texCoords[c][2].y=1.0f;
		texCoords[c][3].x=dx;
		texCoords[c][3].y=1.0f;

		//生成纹理
		generateTexture(c);
	}
	//释放face类
	FT_Done_Face(face);

	//释放FreeType库
	FT_Done_FreeType(library);
}

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	//gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
	//glOrtho(-(GLfloat)width/2,(GLfloat)width/2,-(GLfloat)height/2,(GLfloat)height/2,100,-100);
	GLfloat left=-(GLfloat)width/2;
	GLfloat right=width+left;
	GLfloat bottom=-(GLfloat)height/2;
	GLfloat top=height+bottom;
	glOrtho(left,right,bottom,top,100,-100);
	

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix

	LogTrace("\r\nresize:%d,%d",width,height);
}
/*
class HashMapFontKey:public HashMapKeyInterface{
protected:
	int color;
	int size;
	char c;
public:
	HashMapFontKey(char c,int size,int color)
	{
		this->color=color;
		this->c=c;
		this->size=size;
	}
	int hashCode() const
	{
		return this->c;
	}
	bool equal(const HashMapKeyInterface* const &i) const
	{
		try
		{
			HashMapFontKey* p = dynamic_cast<HashMapFontKey*>(const_cast<HashMapKeyInterface*>(i));
			return this->color==p->hashCode()&&this->size==p->size&&this->c==p->c;
		}
		catch(...)
		{
			return false;
		}
		
	}
};*/
int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	glShadeModel(GL_SMOOTH|GL_FLAT);					// Enable Smooth Shading
	glClearColor(0.2f, 0.2f, 0.2f, 0.5f);				// Black Background
	//glClearDepth(1.0f);									// Depth Buffer Setup
	//glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	//glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	//glEnable(GL_LINE_SMOOTH);   
	//glHint(GL_POINT_SMOOTH, GL_NICEST);
	//glHint(GL_LINE_SMOOTH, GL_NICEST);
	//glHint(GL_POLYGON_SMOOTH, GL_NICEST);
	//glHint(GL_LINE_SMOOTH_HINT,GL_NICEST); 
	//glLineWidth(1.5);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.0f,1,1.0f,1);

	/*CHashTable<string,int>z;
 
 z.Insert("www.baidu.com","www.baidu.com","192.124,343,3434",80);
 z.Insert("www.google.com","www.google.com","192.124,343,3434",80);
 z.Insert("www.baidu.com","www.baidu.com/index.html","192.124,343,3434",80);
 
 cout<<z<<endl;*/


	//BuildFont();										// Build The Font
	//init();
	//LogTrace("\r\ninitGL:%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f",vertex[0].x,vertex[0].y,vertex[1].x,vertex[1].y,vertex[2].x,vertex[2].y,vertex[3].x,vertex[3].y);


	/*HashMap<HashMapFontKey*,int> map;
	HashMapFontKey *k1=new HashMapFontKey(1,1,1);
	HashMapFontKey *k2=new HashMapFontKey(2,2,2);
	map.put(k1,1);
	map.put(k2,2);
	map.put(k1,3);
	LogTrace("\r\nvalue:%d",map.get(k1));
	LogTrace("\r\nvalue:%d",map.get(k2));
	LogTrace("\r\nvalue:%d",map.size());
	delete k1;
	delete k2;*/
	/*HashMap<string,int> map;
	map.put("abc",3);
	map.put("abc",5);
	map.put("www",2);
	//map.clear();
	LogTrace("\r\nvalue:%d",map.get("abc"));
	LogTrace("\r\nvalue:%d",map.get("www"));
	LogTrace("\r\nvalue:%d",map.remove("www"));
	LogTrace("\r\nvalue:%d",map.remove("www"));
	LogTrace("\r\nvalue:%d",map.size());*/
	//HashMapFontKey *k1=new HashMapFontKey(1,1,1);
	//const HashMapFontKey* const& kp=k1;
	//const HashMapKeyInterface* p = kp;
	//LogTrace("\r\nvalue:%d",p->hashCode());
	//delete k1;
	/*HashMap<unsigned int,int> map;
	map.put(1,1);
	map.put(1,2);
	map.put(1025,1025);
	LogTrace("\r\nvalue:%d",*map.get(1));
	LogTrace("\r\nvalue:%d",*map.get(1025));
	LogTrace("\r\nvalue:%d",*map.get(103));
	map.remove(1);
	LogTrace("\r\nvalue:%d",map.get(1));
	LogTrace("\r\nvalue:%d",map.get(1025));
	map.remove(1025);
	LogTrace("\r\nvalue:%d",map.get(1));
	LogTrace("\r\nvalue:%d",map.get(1025));*/

	return TRUE;										// Initialization Went OK
}

int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix
	//glTranslatef(0.0f,0.0f,-1.0f);						// Move One Unit Into The Screen
	// Pulsing Colors Based On Text Position
	//glColor3f(1.0f*float(cos(cnt1)),1.0f*float(sin(cnt2)),1.0f-0.5f*float(cos(cnt1+cnt2)));
	// Position The Text On The Screen
	//glRasterPos2f(-0.45f+0.05f*float(cos(cnt1)), 0.32f*float(sin(cnt2)));
 	//glPrint("Active OpenGL Text With NeHe - %7.2f", cnt1);	// Print GL Text To The Screen
	//cnt1+=0.051f;										// Increase The First Counter
	//cnt2+=0.005f;										// Increase The First Counter

	/*
	glTranslatef(-50.0f,0.0f,0.0f);
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);


	int x,y;
	x=y=0;
	int ch;
	for(int i=0;i<strlen(str);i++)
	{
		ch=str[i];
		generateChar(ch,x,y);
		
		glVertexPointer(2,GL_INT,0,vertex);
		glTexCoordPointer(2, GL_FLOAT, 0, texCoords[ch]);
		glBindTexture(GL_TEXTURE_2D,textures[ch]);
		glDrawElements(GL_TRIANGLE_STRIP,4,GL_UNSIGNED_BYTE,vertexIndices);

		//x+=deltas[ch].x;
		//x+=offsets[ch].x;
		x+=advances[ch].x;
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);

	*/

	return TRUE;										// Everything Went OK
}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}

	//KillFont();
}

//	This Code Creates Our OpenGL Window.  Parameters Are:					
//	title			- Title To Appear At The Top Of The Window				
//	width			- Width Of The GL Window Or Fullscreen Mode			
//	height			- Height Of The GL Window Or Fullscreen Mode			
//	bits			- Number Of Bits To Use For Color (8/16/24/32)			
//	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	// Ask The User Which Screen Mode They Prefer
	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							// Windowed Mode
	}

	// Create Our OpenGL Window
	if (!CreateGLWindow("NeHe's Bitmap Font Tutorial",WIDTH,HEIGHT,16,fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if ((active && !DrawGLScene()) || keys[VK_ESCAPE])	// Active?  Was There A Quit Received?
			{
				done=TRUE;							// ESC or DrawGLScene Signalled A Quit
			}
			else									// Not Time To Quit, Update Screen
			{
				SwapBuffers(hDC);					// Swap Buffers (Double Buffering)
			}

			if (keys[VK_F1])						// Is F1 Being Pressed?
			{
				keys[VK_F1]=FALSE;					// If So Make Key FALSE
				KillGLWindow();						// Kill Our Current Window
				fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow("NeHe's Bitmap Font Tutorial",640,480,16,fullscreen))
				{
					return 0;						// Quit If Window Was Not Created
				}
			}
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}



/*GLvoid BuildFont(GLvoid)								// Build Our Bitmap Font
{
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	base = glGenLists(96);								// Storage For 96 Characters

	font = CreateFont(	-24,							// Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						FW_BOLD,						// Font Weight
						FALSE,							// Italic
						FALSE,							// Underline
						FALSE,							// Strikeout
						ANSI_CHARSET,					// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						"Lucida Console");					// Font Name

	oldfont = (HFONT)SelectObject(hDC, font);           // Selects The Font We Want
	wglUseFontBitmaps(hDC, 32, 96, base);				// Builds 96 Characters Starting At Character 32
	SelectObject(hDC, oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
}

GLvoid KillFont(GLvoid)									// Delete The Font List
{
	glDeleteLists(base, 96);							// Delete All 96 Characters
}

GLvoid glPrint(const char *fmt, ...)					// Custom GL "Print" Routine
{
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	    vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base - 32);								// Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}*/