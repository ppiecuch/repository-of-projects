#include "UnicodeFont.h"

const unsigned char UnicodeFont::vertexIndices[]={0,1,2,3};
UnicodeFont::UnicodeFont(FT_Byte* &dataBase,const unsigned long& dataSize,
		const FT_F26Dot6& charWidth,const FT_F26Dot6& charHeight):charSpacing(0),spaceWidth(0){
	//默认颜色为黑色
	color[0]=0;
	color[1]=0;
	color[2]=0;
	color[3]=255;
	int error;
	// 创建FreeType库
	FT_Library library;
	if (error=FT_Init_FreeType( &library ))
	{
		LOGE("UnicodeFont","FT_Init_FreeType ERROR:%d",error);
		return;
	}
	//创建FT_Face，在FreeType库中用于保存字体信息
	FT_Face face;
	//从内存创建face
	if(error=FT_New_Memory_Face(library,dataBase,dataSize,0,&face))
	{
		LOGE("UnicodeFont","FT_New_Memory_Face ERROR:%d",error);
		// 释放FreeType库
		FT_Done_FreeType(library);
		return;
	}
	// 设定为UNICODE，默认也是
	if(error=FT_Select_Charmap(face, FT_ENCODING_UNICODE))
	{
		LOGE("UnicodeFont","FT_Select_Charmap ERROR:%d",error);
		// 释放FreeType库
		FT_Done_FreeType(library);
		return;
	}
	//在FreeType中使用1/64作为一个像素的高度所以我们需要缩放charWidth,charHeight来满足这个要求，
	//横向分辨率和垂直分辨率都使用96
	if(error=FT_Set_Char_Size( face, charWidth << 6, charHeight << 6, 96,96))
	{
		LOGE("UnicodeFont","FT_Set_Char_Size ERROR:%d",error);
		// 释放face类
		FT_Done_Face(face);
		// 释放FreeType库
		FT_Done_FreeType(library);
		return;
	}
	int index;
	FT_Glyph glyph;
	//基线到字符轮廓最高点的距离, 由于是26.6的定点数，因此获取整数部分需要除以64
	int ascender = face->size->metrics.ascender>>6;
	LOGI("UnicodeFont","ascender:%d",ascender);
	//设置行间距
	this->lineSpacing=ascender;
	//设置空格宽度为最大宽度/2
	this->spaceWidth=face->size->metrics.max_advance>>7;
	LOGI("AndroidGL","spaceWidth:%d",this->spaceWidth);
	//生成256个Unicode码字符纹理的名称
	glGenTextures(256, textures);
	//生成256个纹理坐标
	texCoords=(Point2f**)new Point2f*[256];
	//创建灰度缓冲
	buffers=(GLubyte**)new GLubyte*[256];
	char b[]="中华人民共和国";
	std::cout<<b<<std::endl;
	unsigned short* unicode;
	int len=gb2Unicode(b,unicode);
	//生成256个字符的纹理
	for(int c=0;c<7;c++)
	{
		LOGI("UnicodeFont","unicode[%d]:%x",c,unicode[c]);
		//获取字符c在face的字符映射表中的glyph索引
		index=FT_Get_Char_Index(face,unicode[c]);
		//将灰度缓冲预设为NULL
		buffers[c]=NULL;
		if(index==0)
		{
			//释放纹理名称
			glDeleteTextures(1,&textures[c]);
			textures[c]=-1;
			texCoords[c]=NULL;
			LOGW("UnicodeFont","FT_Get_Char_Index:%d-->%d(undefined character code),so delete texture",c,index);
			continue;
		}
		//根据load_flags的设置从face读取glyph并保存到glyph slot中
		//函数在load时，从字体文件中load的时bitmapped image，而不是outline，
		//而可能是字体中的bitmapped image有问题，从而造成无法正常画出中文。
		//因此这里我们使用FT_LOAD_DEFAULT|FT_LOAD_NO_BITMAP
		//以强制在load时，loadoutline，然后再根据outline得到bitmapped image
		if(error=FT_Load_Glyph( face, index, FT_LOAD_DEFAULT|FT_LOAD_NO_BITMAP))
		{
			LOGW("UnicodeFont","FT_Load_Glyph ERROR:%d",error);
			continue;
		}
		//从face->glyph即face的glyph slot中提取glyph对象
		if(error=FT_Get_Glyph(face->glyph, &glyph))
		{
			LOGW("UnicodeFont","FT_Get_Glyph ERROR:%d",error);
			continue;
		}
		//检测glyph的图像格式并调用相应的渲染器将glyph渲染成位图
		//FT_RENDER_MODE_NORMAL：1像素使用8位灰度级的抗锯齿位图
		//FT_RENDER_MODE_MONO：1像素使用1位灰度级的位图
		if(error=FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL))
		{
			LOGW("UnicodeFont","FT_Render_Glyph ERROR:%d",error);
			continue;
		}
		//如果glyph图像不是位图
		if ( glyph->format != FT_GLYPH_FORMAT_BITMAP )
		{
			//转换glyph对象为位图glyph对象并替换原glyph对象,0表无偏移，1表成功后销毁原glyph
			if(error=FT_Glyph_To_Bitmap( &glyph, FT_RENDER_MODE_NORMAL, 0, 1 ))
			{
				LOGW("UnicodeFont","FT_Glyph_To_Bitmap ERROR:%d",error);
				continue;
			}
		}
		FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph;
		FT_Bitmap bitmap=bitmapGlyph->bitmap;
		//因为OpenGL只支持纹理长度和宽度是2的n次方，所以这里要转换一下
		int width=ceilPower(bitmap.width);
		int height=ceilPower(bitmap.rows);

		//复制位图数据到灰度缓冲区
		buffers[c] = new GLubyte[bitmap.width * bitmap.rows];
		memcpy(buffers[c],bitmap.buffer,bitmap.width * bitmap.rows);

		//保存长宽信息
		deltas[c].x=bitmap.width;
		deltas[c].y=bitmap.rows;

		//保存偏移
		offsets[c].x=bitmapGlyph->left;// 字符距离左边界的距离
		offsets[c].y=bitmapGlyph->top;// 字符最高点距离基线的距离

		LOGI("Unicode","width:%d,height:%d,left:%d,top:%d",width,height,offsets[c].x,offsets[c].y);

		//保存纹理坐标，由于width、height与字符的真实轮廓大小不一定相等，所以要调整纹理坐标
		float dx=(float)bitmap.width/width;
		float dy=(float)bitmap.rows/height;
		dy=1-dy;//因为y被我们反转了，即着色是从上往下的，所以空白是在data块上方
		texCoords[c]=new Point2f[4];
		texCoords[c][0].x=0.0f;
		texCoords[c][0].y=dy;
		texCoords[c][1].x=dx;
		texCoords[c][1].y=dy;
		texCoords[c][2].x=0.0f;
		texCoords[c][2].y=1.0f;
		texCoords[c][3].x=dx;
		texCoords[c][3].y=1.0f;

		//生成纹理
		generateTexture(c,this->color);
	}
	//释放face类
	FT_Done_Face(face);

	//释放FreeType库
	FT_Done_FreeType(library);

	delete glyph;
	LOGI("UnicodeFont","initialize finish",NULL);
}
void UnicodeFont::draw(const char* (&str),float x,float y)
{
	//记录x，用于换行用
//	float orighnalX=x;
//	int size=strlen(str);
//	//统计字符个数，除去换行符跟空格符，用于创建动态数组
//	int count=0;
//	int ch;
	int count=7;
	int ch=0;
//	for(int i=0;i<size;i++)
//	{
//		ch=str[i];
//		switch(ch)
//		{
//		case 10:
//		case 32:
//			break;
//		default:
//			if(this->getTexture(ch)==-1)
//			{
//				//LOGI("UnicodeFont","no texture then continue:%d",i);
//				continue;
//			}
//			count++;
//		}
//	}
	Point2f** vertexs=(Point2f**)new Point2f*[count];
	GLuint texture;

	int index=0;
//	texture=this->textures[ch];
//	if(texture==-1)
//		return;
	for(int i=0,index=0;i<count;i++)
	{
//		ch=str[i];
		texture=this->textures[ch];
//		switch(ch)
//		{
//		case 10:
//			y-=this->lineSpacing;
//			x=orighnalX;
//			//LOGI("UnicodeFont","new lineBase and new charBase:%.2f,%.2f",y,x);
//			continue;
//		case 32:
//			x+=this->charSpacing+this->spaceWidth+this->charSpacing;
//			//LOGI("UnicodeFont","new charBase:%.2f",x);
//			continue;
//		default:
//			if(texture==-1)
//			{
//				x+=this->charSpacing+this->spaceWidth+this->charSpacing;
//				//LOGI("UnicodeFont","no texture and new charBase ch:%d",ch);
//				continue;
//			}
//		}
//		ch=index;
		vertexs[index]=new Point2f[4];
		//创建顶点坐标及纹理坐标
		this->generateChar(vertexs[index],ch,x,y);
		glVertexPointer(2,GL_FLOAT,0,vertexs[index]);
		glTexCoordPointer(2, GL_FLOAT, 0, this->texCoords[ch]);
		glBindTexture(GL_TEXTURE_2D,texture);
		glDrawElements(GL_TRIANGLE_STRIP,4,GL_UNSIGNED_BYTE,vertexIndices);
		x+=deltas[ch].x;
		x+=this->charSpacing;
		y-=this->lineSpacing;
		index++;
		ch++;
	}
	delete[] vertexs;
}
void UnicodeFont::generateChar(Point2f* &vertex,const int& index,const float& x,const float& y)
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

	vertex[0].x=x+this->offsets[index].x;
	vertex[0].y=y+this->offsets[index].y-this->deltas[index].y;

	vertex[1].x=x+this->offsets[index].x+this->deltas[index].x;
	vertex[1].y=y+this->offsets[index].y-this->deltas[index].y;

	vertex[2].x=x+this->offsets[index].x;
	vertex[2].y=y+this->offsets[index].y;

	vertex[3].x=x+this->offsets[index].x+this->deltas[index].x;
	vertex[3].y=y+this->offsets[index].y;
}
void UnicodeFont::setCharSpacing(const float& distance)
{
	this->charSpacing=distance;
}
void UnicodeFont::setLineSpacing(const float& distance)
{
	this->lineSpacing=distance;
}
void UnicodeFont::setColor(const unsigned char (&color)[4])
{
	if(this->color[0]==color[0]&&this->color[1]==color[1]&&this->color[2]==color[2]&&this->color[3]==color[3])
		return;
	this->color[0]=color[0];
	this->color[1]=color[1];
	this->color[2]=color[2];
	this->color[3]=color[3];
	//全部纹理重建
	for(int i=0;i<256;i++)
	{
		if(this->buffers[i]!=NULL)
			generateTexture(i,this->color);
	}
}
void UnicodeFont::generateTexture(const int& ch,const unsigned char (&color)[4])
{
	GLubyte* buffer=this->buffers[ch];
	if(buffer==NULL)
	{
		LOGW("UnicodeFont","generateTexture(%d,%ulx) fail,for buffer==NULL",ch,color);
		return;
	}
	//获取颜色分量
	int red=0xFF&color[0];
	int green=0xFF&color[1];
	int blue=0xFF&color[2];
	int alpha=0xFF&color[3];

	int width=this->deltas[ch].x;
	int height=this->deltas[ch].y;
	int newWidth=ceilPower(width);
	int newHeight=ceilPower(height);
	GLubyte* data=new GLubyte[newWidth*newHeight];
	int bufferIndex=0;
	int value;
	int row;
	int column;
	for(int i=0; i<newHeight;i++) {
		row=(newHeight-i-1)*newWidth;
		for(int j=0; j<newWidth; j++){
			column=row+j;
			//因为width跟height是经过对bitmap.width,bitmap.rows进行ceilPower扩大后得到的，
			//所以对于扩出的部分应该用0表示
			value=(i>=height||j>=width)?0:buffer[bufferIndex++];
			data[column]=value;
//			LOGI("UnicodeFont","%d-%d:%d",i,j,value);
		}
	}

	//设置字体纹理的纹理过滤器
	glBindTexture(GL_TEXTURE_2D, textures[ch]);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D( GL_TEXTURE_2D,0,GL_LUMINANCE,newWidth,newHeight,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,data);

	delete[] data;
}
