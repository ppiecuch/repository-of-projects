#include "UnicodePainter.h"

UnicodePainter::UnicodePainter(FT_Byte* &dataBase,const unsigned long dataSize,const unsigned int size)
	:charSpacing(0),spaceWidth(0),color(0x000000FF)
{

	this->size=size;
	this->dataBase=dataBase;
//	this->dataSize=dataSize;
	int error;
	// 创建FreeType库
	if (error=FT_Init_FreeType( &library ))
	{
		LOGE("UnicodePainter","FT_Init_FreeType ERROR:%d",error);
		return;
	}
	//创建FT_Face，在FreeType库中用于保存字体信息
	//从内存创建face
	if(error=FT_New_Memory_Face(library,dataBase,dataSize,0,&face))
	{
		LOGE("UnicodePainter","FT_New_Memory_Face ERROR:%d",error);
		// 释放FreeType库
		FT_Done_FreeType(library);
		return;
	}
	// 设定为UNICODE，默认也是
	if(error=FT_Select_Charmap(face, FT_ENCODING_UNICODE))
	{
		LOGE("UnicodePainter","FT_Select_Charmap ERROR:%d",error);
		// 释放FreeType库
		FT_Done_FreeType(library);
		return;
	}
	//在FreeType中使用1/64作为一个像素的高度所以我们需要缩放charWidth,charHeight来满足这个要求，
	//横向分辨率和垂直分辨率都使用96
	if(error=FT_Set_Char_Size( face, size << 6, size << 6, 96,96))
	{
		LOGE("UnicodePainter","FT_Set_Char_Size ERROR:%d",error);
		// 释放face类
		FT_Done_Face(face);
		// 释放FreeType库
		FT_Done_FreeType(library);
		return;
	}
//	int index;
//	FT_Glyph glyph;
	//设置行间距为ascender，即基线到字符轮廓最高点的距离, 由于是26.6的定点数，因此获取整数部分需要除以64
	this->lineSpacing = face->size->metrics.ascender>>6;
	LOGI("UnicodePainter","lineSpacing:%d",this->lineSpacing);
	//设置空格宽度为最大宽度/2
	this->spaceWidth=face->size->metrics.max_advance>>7;
	LOGI("UnicodePainter","spaceWidth:%d",this->spaceWidth);
}

UnicodePainter::~UnicodePainter()
{
	//释放FreeType库
	if(library!=NULL)
	{
		LOGI("UnicodePainter","FT_Done_FreeType",NULL);
		FT_Done_FreeType(library);
	}
	//释放face类
	if(face!=NULL)
	{
		LOGI("UnicodePainter","FT_Done_Face",NULL);
		FT_Done_Face(face);
	}
	delete[] dataBase;
}

void UnicodePainter::draw(unsigned short* (&str),int len,float x,float y)
{
	unsigned short unicode;
	for(int i=0;i<len;i++)
	{
		unicode=str[i];
		LOGI("UnicodePainter","unicode:%lx",unicode);
		//迭代器
		HASH::iterator iterator=fonts.find(unicode);
		//如果不存在此字符，创建之
		if(iterator==fonts.end())
		{
			LOGI("UnicodePainter","unicode[%lx] not found,creating",unicode);
			//获取编码unicode在face的字符映射表中的glyph索引
			int index=FT_Get_Char_Index(face,unicode);
			int error;
			FT_Glyph glyph;
			if(index==0)
			{
				LOGW("UnicodePainter","FT_Get_Char_Index:%lx-->%d(undefined character code)",unicode,index);
				continue;
			}
			//根据load_flags的设置从face读取glyph并保存到glyph slot中
			//函数在load时，从字体文件中load的时bitmapped image，而不是outline，
			//而可能是字体中的bitmapped image有问题，从而造成无法正常画出中文。
			//因此这里我们使用FT_LOAD_DEFAULT|FT_LOAD_NO_BITMAP
			//以强制在load时，loadoutline，然后再根据outline得到bitmapped image
			if(error=FT_Load_Glyph( face, index, FT_LOAD_DEFAULT|FT_LOAD_NO_BITMAP))
			{
				LOGW("UnicodePainter","FT_Load_Glyph ERROR:%d",error);
				continue;
			}
			//从face->glyph即face的glyph slot中提取glyph对象
			if(error=FT_Get_Glyph(face->glyph, &glyph))
			{
				LOGW("UnicodePainter","FT_Get_Glyph ERROR:%d",error);
				continue;
			}
			//检测glyph的图像格式并调用相应的渲染器将glyph渲染成位图
			//FT_RENDER_MODE_NORMAL：1像素使用8位灰度级的抗锯齿位图
			//FT_RENDER_MODE_MONO：1像素使用1位灰度级的位图
			if(error=FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL))
			{
				LOGW("UnicodePainter","FT_Render_Glyph ERROR:%d",error);
				continue;
			}
			//如果glyph图像不是位图
			if ( glyph->format != FT_GLYPH_FORMAT_BITMAP )
			{
				//转换glyph对象为位图glyph对象并替换原glyph对象,0表无偏移，1表成功后销毁原glyph
				if(error=FT_Glyph_To_Bitmap( &glyph, FT_RENDER_MODE_NORMAL, 0, 1 ))
				{
					LOGW("UnicodePainter","FT_Glyph_To_Bitmap ERROR:%d",error);
					continue;
				}
			}
			FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph;
			FT_Bitmap bitmap=bitmapGlyph->bitmap;

			//复制位图数据到灰度缓冲区
			GLubyte* buffer = new GLubyte[bitmap.width * bitmap.rows];
			memcpy(buffer,bitmap.buffer,bitmap.width * bitmap.rows);

			//bitmapGlyph->left;// 字符距离左边界的距离
			//bitmapGlyph->top;// 字符最高点距离基线的距离

			Font* font=new Font(buffer,size,color,bitmap.width,bitmap.rows,bitmapGlyph->left,bitmapGlyph->top);

			//因为OpenGL只支持纹理长度和宽度是2的n次方，所以这里要转换一下
			int width=ceilPower(bitmap.width);
			int height=ceilPower(bitmap.rows);
			LOGI("UnicodePainter","width:%d,height:%d,left:%d,top:%d",width,height,bitmapGlyph->left,bitmapGlyph->top);
			//保存纹理坐标，由于width、height与字符的真实轮廓大小不一定相等，所以要调整纹理坐标
			float dx=(float)bitmap.width/width;
			float dy=(float)bitmap.rows/height;
			dy=1-dy;//因为y被我们反转了，即着色是从上往下的，所以空白是在data块上方
			font->texCoords[0].x=0.0f;
			font->texCoords[0].y=dy;
			font->texCoords[1].x=dx;
			font->texCoords[1].y=dy;
			font->texCoords[2].x=0.0f;
			font->texCoords[2].y=1.0f;
			font->texCoords[3].x=dx;
			font->texCoords[3].y=1.0f;

			//fonts[unicode]=font;
			fonts.insert(HASH::value_type(unicode,*font));

			font->draw(x,y);

			delete glyph;
		}
		else
		{
			(*iterator).second.draw(x,y);
		}
	}
}

void UnicodePainter::setCharSpacing(float distance)
{
	this->charSpacing=distance;
}

void UnicodePainter::setLineSpacing(float distance)
{
	this->lineSpacing=distance;
}

void UnicodePainter::setColor(long color)
{
	this->color=color;
	//TODO
}

void UnicodePainter::setSize(int size)
{
	this->size=size;
	//TODO
}
