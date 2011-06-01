#include "AsciiBrush.h"

//返回比num大的，并且是最接近num的2的次方的数
int ceilPower(int num)
{
	int rval=1;
	rval*=2;
	while(rval<num)
		rval<<=1;
	return rval;
}

const unsigned char AsciiBrush::vertexIndices[]={0,1,2,3};
AsciiBrush::AsciiBrush(const char* const &fontPath)
{
	int error;

	//Initilize a freetype font library.
	if (FT_Init_FreeType( &this->library )) 
		throw std::runtime_error("FT_Init_FreeType failed");

	//This is where we load in the font information from the file.
	//Of all the places where the code might die, this is the most likely,
	//as FT_New_Face will die if the font file does not exist or is somehow broken.
	if (FT_New_Face( this->library, fontPath, 0, &this->face )) 
		throw std::runtime_error("FT_New_Face failed (there is probably a problem with your font file)");
}
AsciiBrush::AsciiBrush(FT_Byte* &dataBase,unsigned long dataSize)
{
	//TODO
}
AsciiBrush::~AsciiBrush()
{
	FT_Done_FreeType(this->library);
	FT_Done_Face(this->face);
}
AsciiFont& AsciiBrush::generateFont(const AsciiFont& key)
{
	/*int color=key.color;
	unsigned char c=key.c;
	int size=key.size;
	int error;
	
	FT_Set_Pixel_Sizes(face,size,size);
	FT_GlyphSlot slot=face->glyph;
	FT_Glyph_Metrics metrics=face->glyph->metrics;
	FT_Glyph glyph;
	//基线到字符轮廓最高点的距离, 由于是26.6的定点数，因此获取整数部分需要除以64
	int ascender = face->size->metrics.ascender>>6;

	//获取字符c在face的字符映射表中的glyph索引
	int index=FT_Get_Char_Index(face,c);

	if(index==0)
		throw std::runtime_error("\r\nFT_Get_Char_Index(undefined character code):"+c);
	//根据load_flags的设置从face读取glyph并保存到glyph slot中
	//这个函数会设法从face中装载对应的字形图像:如果找到一个对应该字形和象素尺寸的位图，
	//那么它将会被装载到字形槽中。嵌入的位图总是比原生的图像格式优先装载。
	//因为我们假定对一个字形，它有更高质量的版本。这可以用FT_LOAD_NO_BITMAP标志来改变。
	//否则，将装载一个该字形的原生图像，把它伸缩到当前的象素尺寸，并且对应如TrueType和Type1这些格式，也会完成hinted操作。
	if(error=FT_Load_Glyph( face, index, FT_LOAD_DEFAULT ))
		throw std::runtime_error("\r\nFT_Load_Glyph ERROR:"+error);
	//从face->glyph即face的glyph slot中提取glyph对象
	if(error=FT_Get_Glyph(face->glyph, &glyph))
		throw std::runtime_error("\r\nFT_Get_Glyph ERROR:"+error);
	//检测glyph的图像格式并调用相应的渲染器将glyph渲染成位图
	//FT_RENDER_MODE_NORMAL：1像素使用8位灰度级的抗锯齿位图
	//FT_RENDER_MODE_MONO：1像素使用1位灰度级的位图
	if(error=FT_Render_Glyph( face->glyph, FT_RENDER_MODE_LCD))
	//if(error=FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL))
		throw std::runtime_error("\r\nFT_Render_Glyph ERROR:"+error);
	//字段face->glyph->format描述了字形槽中存储的字形图像的格式。如果它的值不是FT_GLYPH_FORMAT_BITMAP，你可以通过FT_Render_Glyph把它直接转换为一个位图。
	//如果glyph图像不是位图
	if (glyph->format != FT_GLYPH_FORMAT_BITMAP)
	{
		//转换glyph对象为位图glyph对象并替换原glyph对象,0表无偏移，1表成功后销毁原glyph
		if(error=FT_Glyph_To_Bitmap( &glyph, FT_RENDER_MODE_NORMAL, 0, 1 ))
			throw std::runtime_error("\r\nFT_Glyph_To_Bitmap ERROR:"+error);
	}
	FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph;
	FT_Bitmap bitmap=bitmapGlyph->bitmap;

	//因为OpenGL只支持纹理长度和宽度是2的n次方，所以这里要转换一下
	int width=bitmap.width;
	int height=bitmap.rows;
	int newWidth=ceilPower(width);
	int newHeight=ceilPower(height);*/

	AsciiFont font;
	/*
	//保存颜色
	font.color=color;
	//保存长宽信息
	font.delta.x=width;
	font.delta.y=height;
	//保存大小
	font.size=size;
	//复制位图数据到灰度缓冲区
	//GLubyte *buffers = new GLubyte[bitmap.width * bitmap.rows];
	//memcpy(buffers,bitmap.buffer,bitmap.width * bitmap.rows);

	//保存偏移
	font.offset.x=bitmapGlyph->left;// 字符距离左边界的距离
	font.offset.y=bitmapGlyph->top;// 字符最高点距离基线的距离
	font.advance.x=slot->advance.x>>6;
	font.advance.y=slot->advance.y>>6;

	//保存纹理坐标，由于newWidth、newHeight与字符的真实轮廓大小不一定相等，所以要调整纹理坐标
	float dx=(float)width/newWidth;
	float dy=(float)height/newHeight;
	dy=1-dy;//因为y被我们反转了，即着色是从上往下的，所以空白是在data块上方
	font.texCoord[0].x=0.0f;
	font.texCoord[0].y=dy;
	font.texCoord[1].x=dx;
	font.texCoord[1].y=dy;
	font.texCoord[2].x=0.0f;
	font.texCoord[2].y=1.0f;
	font.texCoord[3].x=dx;
	font.texCoord[3].y=1.0f;

	//获取颜色分量
	int red=0xFF&(color>>24);
	int green=0xFF&(color>>16);
	int blue=0xFF&(color>>8);
	int alpha=0xFF&color;

	int bufferIndex=0;
	int value;
	int row;
	int column;
	GLubyte* data=new GLubyte[newWidth*newHeight*4];
	for(int i=0; i<newHeight;i++) {
		row=(newHeight-i-1)*newWidth;
		for(int j=0; j<newWidth; j++){
			column=row+j<<2;
			//因为width跟height是经过对bitmap.width,bitmap.rows进行ceilPower扩大后得到的，
			//所以对于扩出的部分其alpha分值用0表示透明
			value=(i>=height||j>=width)?0:bitmap.buffer[bufferIndex++];

			data[column]=red;
			data[column+1]=green;
			data[column+2]=blue;
			data[column+3]=value;
		}
	}

	//生成1个纹理的名称
	glGenTextures(1, &font.texture);
	//设置字体纹理的纹理过滤器
	glBindTexture(GL_TEXTURE_2D, font.texture);
	
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

	delete[] data;*/

	return font;
}
void AsciiBrush::drawChar(const AsciiFont &font,int x,int y)
{
	/*vertex[0].x=x+font.offset.x;
	vertex[0].y=y+font.offset.y-font.delta.y;
	vertex[1].x=x+font.offset.x+font.delta.x;
	vertex[1].y=y+font.offset.y-font.delta.y;
	vertex[2].x=x+font.offset.x;
	vertex[2].y=y+font.offset.y;
	vertex[3].x=x+font.offset.x+font.delta.x;
	vertex[3].y=y+font.offset.y;
	glVertexPointer(2,GL_FLOAT,0,vertex);
	glTexCoordPointer(2, GL_FLOAT, 0, font.texCoord);
	glBindTexture(GL_TEXTURE_2D,font.texture);
	glDrawElements(GL_TRIANGLE_STRIP,4,GL_UNSIGNED_BYTE,vertexIndices);*/
}
void AsciiBrush::drawInLine(const unsigned char* const& str,int color,int size,int x,int y)
{
	/*this->tempKey.color=color;
	this->tempKey.size=size;
	int nextX=x;
	for(int i=0;i<strlen(str);i++)
	{
		this->tempKey.c=str[i];
		if(this->map.find(this->tempKey)== map.end())
		{
			AsciiFont &font=generateFont(this->tempKey);
			this->map[this->tempKey]=font;
		}
		AsciiFont &font=this->map[this->tempKey];
		drawChar(font,nextX,y);
		//步进
		nextX+=font.advance.x;
	}*/
}
