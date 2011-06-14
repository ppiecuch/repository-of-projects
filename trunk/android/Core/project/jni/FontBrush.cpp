#include "fontbrush.h"

const unsigned char FontBrush::vertexIndices[]={0,1,2,3};
FontBrush::FontBrush(const char* const &fontPath)
{

	//Initilize a freetype font library.
	if (FT_Init_FreeType( &this->library )) 
		throw std::runtime_error("FT_Init_FreeType failed");

	//This is where we load in the font information from the file.
	//Of all the places where the code might die, this is the most likely,
	//as FT_New_Face will die if the font file does not exist or is somehow broken.
	if (FT_New_Face( this->library, fontPath, 0, &this->face )) 
		throw std::runtime_error("FT_New_Face failed (there is probably a problem with your font file)");
	LOGD("FontBrush","constructed,fontPath:%s",fontPath);
}
FontBrush::FontBrush(FT_Byte* &dataBase,unsigned long dataSize)
{
	this->dataBase=new FT_Byte[dataSize];
	for(int i=0;i<dataSize;i++)
	{
		this->dataBase[i]=dataBase[i];
	}
	// ����FreeType��
	if (FT_Init_FreeType( &this->library ))
		throw std::runtime_error("FT_Init_FreeType failed");
	//����FT_Face����FreeType�������ڱ���������Ϣ
	//���ڴ洴��face
	if(FT_New_Memory_Face(library,this->dataBase,dataSize,0,&this->face))
		throw std::runtime_error("FT_New_Memory_Face failed");
	LOGD("FontBrush","constructed,dataBase:%x,dataSize:%lu",dataBase,dataSize);
}
FontBrush::~FontBrush()
{
	FT_Done_FreeType(this->library);
	FT_Done_Face(this->face);
	delete[] this->dataBase;
	LOGD("FontBrush","deconstructed");
}
void FontBrush::generateFont(const Font& key)
{
	//LOGD("FontBrush","=================start generateFont===============");
	int color=key.color;
	unsigned short c=key.c;
	//LOGD("FontBrush","c:%c",c);
	int size=key.size;
	int error;
	
	FT_Set_Pixel_Sizes(face,size,size);
	FT_GlyphSlot slot=face->glyph;
	FT_Glyph_Metrics metrics=face->glyph->metrics;
	FT_Glyph glyph;
	//���ߵ��ַ�������ߵ�ľ���, ������26.6�Ķ���������˻�ȡ����������Ҫ����64
	int ascender = face->size->metrics.ascender>>6;

	//��ȡ�ַ�c��face���ַ�ӳ����е�glyph����
	int index=FT_Get_Char_Index(face,c);
	//LOGD("FontBrush","index:%d",index);

	if(index==0)
	{
		LOGW("FontBrush","FT_Get_Char_Index(undefined character code):%c",c);
		throw std::runtime_error("FT_Get_Char_Index(undefined character code)");
	}
	//����load_flags�����ô�face��ȡglyph�����浽glyph slot��
	//����������跨��face��װ�ض�Ӧ������ͼ��:����ҵ�һ����Ӧ�����κ����سߴ��λͼ��
	//��ô�����ᱻװ�ص����β��С�Ƕ���λͼ���Ǳ�ԭ����ͼ���ʽ����װ�ء�
	//��Ϊ���Ǽٶ���һ�����Σ����и��������İ汾���������FT_LOAD_NO_BITMAP��־���ı䡣
	//���򣬽�װ��һ�������ε�ԭ��ͼ�񣬰�����������ǰ�����سߴ磬���Ҷ�Ӧ��TrueType��Type1��Щ��ʽ��Ҳ�����hinted������
	if(error=FT_Load_Glyph( face, index, FT_LOAD_DEFAULT|FT_LOAD_NO_BITMAP))
	{
		LOGE("FontBrush","FT_Load_Glyph ERROR:%x",error);
		throw std::runtime_error("\r\nFT_Load_Glyph ERROR:"+error);
	}
	//��face->glyph��face��glyph slot����ȡglyph����
	if(error=FT_Get_Glyph(face->glyph, &glyph))
		throw std::runtime_error("\r\nFT_Get_Glyph ERROR:"+error);
	//���glyph��ͼ���ʽ��������Ӧ����Ⱦ����glyph��Ⱦ��λͼ
	//FT_RENDER_MODE_NORMAL��1����ʹ��8λ�Ҷȼ��Ŀ����λͼ
	//FT_RENDER_MODE_MONO��1����ʹ��1λ�Ҷȼ���λͼ
	//if(error=FT_Render_Glyph( face->glyph, FT_RENDER_MODE_LCD))
	if(error=FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL))
		throw std::runtime_error("\r\nFT_Render_Glyph ERROR:"+error);
	//�ֶ�face->glyph->format���������β��д洢������ͼ��ĸ�ʽ���������ֵ����FT_GLYPH_FORMAT_BITMAP�������ͨ��FT_Render_Glyph����ֱ��ת��Ϊһ��λͼ��
	//���glyphͼ����λͼ
	if (glyph->format != FT_GLYPH_FORMAT_BITMAP)
	{
		//ת��glyph����Ϊλͼglyph�����滻ԭglyph����,0����ƫ�ƣ�1��ɹ�������ԭglyph
		if(error=FT_Glyph_To_Bitmap( &glyph, FT_RENDER_MODE_NORMAL, 0, 1 ))
			throw std::runtime_error("\r\nFT_Glyph_To_Bitmap ERROR:"+error);
	}
	FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph;
	FT_Bitmap bitmap=bitmapGlyph->bitmap;
	//��ΪOpenGLֻ֧�������ȺͿ����2��n�η�����������Ҫת��һ��
	int width=bitmap.width;
	int height=bitmap.rows;
	int newWidth=ceilPower(width);
	int newHeight=ceilPower(height);

	//LOGD("FontBrush","generateFont:middle");

	Font *font=new Font();
	
	//�����ַ�
	font->c=c;
	//������ɫ
	font->color=color;
	//���泤����Ϣ
	font->delta.x=width;
	font->delta.y=height;
	//�����С
	font->size=size;
	//����λͼ���ݵ��ҶȻ�����
	//GLubyte *buffers = new GLubyte[bitmap.width * bitmap.rows];
	//memcpy(buffers,bitmap.buffer,bitmap.width * bitmap.rows);
	//����ƫ��
	font->offset.x=bitmapGlyph->left;// �ַ�������߽�ľ���
	font->offset.y=bitmapGlyph->top;// �ַ���ߵ������ߵľ���
	font->advance.x=slot->advance.x>>6;
	font->advance.y=slot->advance.y>>6;

	//�����������꣬����newWidth��newHeight���ַ�����ʵ������С��һ����ȣ�����Ҫ������������
	float dx=(float)width/newWidth;
	float dy=(float)height/newHeight;
	dy=1-dy;//��Ϊy�����Ƿ�ת�ˣ�����ɫ�Ǵ������µģ����Կհ�����data���Ϸ�
	font->texCoord[0].x=0.0f;
	font->texCoord[0].y=dy;
	font->texCoord[1].x=dx;
	font->texCoord[1].y=dy;
	font->texCoord[2].x=0.0f;
	font->texCoord[2].y=1.0f;
	font->texCoord[3].x=dx;
	font->texCoord[3].y=1.0f;

	//��ȡ��ɫ����
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
			//��Ϊwidth��height�Ǿ�����bitmap.width,bitmap.rows����ceilPower�����õ��ģ�
			//���Զ��������Ĳ�����alpha��ֵ��0��ʾ͸��
			value=(i>=height||j>=width)?0:bitmap.buffer[bufferIndex++];

			data[column]=red;
			data[column+1]=green;
			data[column+2]=blue;
			data[column+3]=value;
		}
	}
	//����1�����������
	glGenTextures(1, &font->texture);
	//����������������������
	glBindTexture(GL_TEXTURE_2D, font->texture);
	
	//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//glTexEnvi(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	glTexImage2D( GL_TEXTURE_2D,0,GL_RGBA,newWidth,newHeight,0,GL_RGBA,GL_UNSIGNED_BYTE,data);

	delete[] data;

	//��font���󱣴��set��
	this->set.insert(font);
	LOGI("FontBrush","generateFont:{char:%c,color:%x,size:%d}",font->c,font->color,font->size);
	//LOGD("FontBrush","=================end generateFont===============");
}
void FontBrush::drawChar(const FontSet::iterator &iterator,int x,int y)
{
	//LOGD("FontBrush","start drawChar:%c",(*iterator)->c);
	vertex[0].x=x+(*iterator)->offset.x;
	vertex[0].y=y+(*iterator)->offset.y-(*iterator)->delta.y;
	vertex[1].x=x+(*iterator)->offset.x+(*iterator)->delta.x;
	vertex[1].y=y+(*iterator)->offset.y-(*iterator)->delta.y;
	vertex[2].x=x+(*iterator)->offset.x;
	vertex[2].y=y+(*iterator)->offset.y;
	vertex[3].x=x+(*iterator)->offset.x+(*iterator)->delta.x;
	vertex[3].y=y+(*iterator)->offset.y;
	//LOGD("FontBrush","vertex[]:{{%.2f,%.2f},{%.2f,%.2f},{%.2f,%.2f},{%.2f,%.2f}}",vertex[0].x,vertex[0].y,vertex[1].x,vertex[1].y,vertex[2].x,vertex[2].y,vertex[3].x,vertex[3].y);
	glVertexPointer(2,GL_FLOAT,0,vertex);
	//LOGD("FontBrush","texCoord:%d",(*iterator)->texture);
	glTexCoordPointer(2, GL_FLOAT, 0, (*iterator)->texCoord);
	glBindTexture(GL_TEXTURE_2D,(*iterator)->texture);
	glDrawElements(GL_TRIANGLE_STRIP,4,GL_UNSIGNED_BYTE,vertexIndices);
	//LOGD("FontBrush","drawChar:%c",(*iterator)->c);
}
void FontBrush::drawInLine(const char* const& str,int color,int size,int x,int y)
{
	this->tempKey.color=color;
	this->tempKey.size=size;
	int nextX=x;
	for(int i=0;i<strlen(str);i++)
	{
		this->tempKey.c=str[i];
		//LOGD("FontBrush","test tempKey:%c",this->tempKey.c);
		if(this->set.find(&this->tempKey)== set.end())
		{
			try{
				generateFont(this->tempKey);
			}catch(const runtime_error &r){
				LOGE("FontBrush","runtime_error:%s",r.what());
				continue;
			}
		}
		const FontSet::iterator iterator=this->set.find(&this->tempKey);
		drawChar(iterator,nextX,y);
		//����
		nextX+=(*iterator)->advance.x;
	}
	//LOGD("FontBrush","drawInLine:%s",str);
}
void FontBrush::drawInLine(const unsigned short* const& str,int len,int color,int size,int x,int y)
{
	this->tempKey.color=color;
	this->tempKey.size=size;
	int nextX=x;
	for(int i=0;i<len;i++)
	{
		this->tempKey.c=str[i];
		//LOGD("FontBrush","test tempKey:%c",this->tempKey.c);
		if(this->set.find(&this->tempKey)== set.end())
		{
			try{
				generateFont(this->tempKey);
			}catch(const runtime_error &r){
				LOGE("FontBrush","runtime_error:%s",r.what());
				continue;
			}
		}
		const FontSet::iterator iterator=this->set.find(&this->tempKey);
		drawChar(iterator,nextX,y);
		//����
		nextX+=(*iterator)->advance.x;
	}
	//LOGD("FontBrush","drawInLine:%s",str);
}
