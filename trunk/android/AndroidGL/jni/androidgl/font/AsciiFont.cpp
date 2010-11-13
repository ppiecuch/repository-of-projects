#include "AsciiFont.h"

const unsigned char AsciiFont::vertexIndices[]={0,1,2,3};
AsciiFont::AsciiFont(FT_Byte* &dataBase,const unsigned long& dataSize,
		const FT_F26Dot6& charWidth,const FT_F26Dot6& charHeight):charSpacing(0),spaceWidth(0){
	//Ĭ����ɫΪ��ɫ
	color[0]=0;
	color[1]=0;
	color[2]=0;
	color[3]=255;
	int error;
	// ����FreeType��
	FT_Library library;
	if (error=FT_Init_FreeType( &library ))
	{
		LOGE("AsciiFont","FT_Init_FreeType ERROR:%d",error);
		return;
	}
	//����FT_Face����FreeType�������ڱ���������Ϣ
	FT_Face face;
	//���ڴ洴��face
	if(error=FT_New_Memory_Face(library,dataBase,dataSize,0,&face))
	{
		LOGE("AsciiFont","FT_New_Memory_Face ERROR:%d",error);
		// �ͷ�FreeType��
		FT_Done_FreeType(library);
		return;
	}
	//��FreeType��ʹ��1/64��Ϊһ�����صĸ߶�����������Ҫ����charWidth,charHeight���������Ҫ��
	//����ֱ��ʺʹ�ֱ�ֱ��ʶ�ʹ��96
	if(error=FT_Set_Char_Size( face, charWidth << 6, charHeight << 6, 96,96))
	{
		LOGE("AsciiFont","FT_Set_Char_Size ERROR:%d",error);
		// �ͷ�face��
		FT_Done_Face(face);
		// �ͷ�FreeType��
		FT_Done_FreeType(library);
		return;
	}
	int index;
	FT_Glyph glyph;
	//���ߵ��ַ�������ߵ�ľ���, ������26.6�Ķ���������˻�ȡ����������Ҫ����64
	int ascender = face->size->metrics.ascender>>6;
	LOGI("AsciiFont","ascender:%d",ascender);
	//�����м��
	this->lineSpacing=ascender;
	//���ÿո���Ϊ�����/2
	this->spaceWidth=face->size->metrics.max_advance>>7;
	LOGI("AndroidGL","spaceWidth:%d",this->spaceWidth);
	//����256��ASCII���ַ����������
	glGenTextures(256, textures);
	//����256����������
	texCoords=(Point2f**)new Point2f*[256];
	//�����ҶȻ���
	buffers=(GLubyte**)new GLubyte*[256];
	//����256���ַ�������
	for(int c=0;c<256;c++)
	{
		//��ȡ�ַ�c��face���ַ�ӳ����е�glyph����
		index=FT_Get_Char_Index(face,c);
		//���ҶȻ���Ԥ��ΪNULL
		buffers[c]=NULL;
		if(index==0)
		{
			//�ͷ���������
			glDeleteTextures(1,&textures[c]);
			textures[c]=-1;
			texCoords[c]=NULL;
			LOGW("AsciiFont","FT_Get_Char_Index:%d-->%d(undefined character code),so delete texture",c,index);
			continue;
		}
		//����load_flags�����ô�face��ȡglyph�����浽glyph slot��
		if(error=FT_Load_Glyph( face, index, FT_LOAD_DEFAULT ))
		{
			LOGW("AsciiFont","FT_Load_Glyph ERROR:%d",error);
			continue;
		}
		//��face->glyph��face��glyph slot����ȡglyph����
		if(error=FT_Get_Glyph(face->glyph, &glyph))
		{
			LOGW("AsciiFont","FT_Get_Glyph ERROR:%d",error);
			continue;
		}
		//���glyph��ͼ���ʽ��������Ӧ����Ⱦ����glyph��Ⱦ��λͼ
		//FT_RENDER_MODE_NORMAL��1����ʹ��8λ�Ҷȼ��Ŀ����λͼ
		//FT_RENDER_MODE_MONO��1����ʹ��1λ�Ҷȼ���λͼ
		if(error=FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL))
		{
			LOGW("AsciiFont","FT_Render_Glyph ERROR:%d",error);
			continue;
		}
		//���glyphͼ����λͼ
		if ( glyph->format != FT_GLYPH_FORMAT_BITMAP )
		{
			//ת��glyph����Ϊλͼglyph�����滻ԭglyph����,0����ƫ�ƣ�1��ɹ�������ԭglyph
			if(error=FT_Glyph_To_Bitmap( &glyph, FT_RENDER_MODE_NORMAL, 0, 1 ))
			{
				LOGW("AsciiFont","FT_Glyph_To_Bitmap ERROR:%d",error);
				continue;
			}
		}
		FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph;
		FT_Bitmap bitmap=bitmapGlyph->bitmap;
		//��ΪOpenGLֻ֧�������ȺͿ����2��n�η�����������Ҫת��һ��
		int width=ceilPower(bitmap.width);
		int height=ceilPower(bitmap.rows);

		//����λͼ���ݵ��ҶȻ�����
		buffers[c] = new GLubyte[bitmap.width * bitmap.rows];
		memcpy(buffers[c],bitmap.buffer,bitmap.width * bitmap.rows);

		//���泤����Ϣ
		deltas[c].x=bitmap.width;
		deltas[c].y=bitmap.rows;

		//����ƫ��
		offsets[c].x=bitmapGlyph->left;// �ַ�������߽�ľ���
		offsets[c].y=bitmapGlyph->top;// �ַ���ߵ������ߵľ���

		//�����������꣬����width��height���ַ�����ʵ������С��һ����ȣ�����Ҫ������������
		float dx=(float)bitmap.width/width;
		float dy=(float)bitmap.rows/height;
		dy=1-dy;//��Ϊy�����Ƿ�ת�ˣ�����ɫ�Ǵ������µģ����Կհ�����data���Ϸ�
		texCoords[c]=new Point2f[4];
		texCoords[c][0].x=0.0f;
		texCoords[c][0].y=dy;
		texCoords[c][1].x=dx;
		texCoords[c][1].y=dy;
		texCoords[c][2].x=0.0f;
		texCoords[c][2].y=1.0f;
		texCoords[c][3].x=dx;
		texCoords[c][3].y=1.0f;

		//��������
		generateTexture(c,this->color);
	}
	//�ͷ�face��
	FT_Done_Face(face);

	//�ͷ�FreeType��
	FT_Done_FreeType(library);

	delete glyph;
}
GLuint AsciiFont::getTexture(const int& index)
{
	return this->textures[index];
}
void AsciiFont::draw(const char* (&str),float x,float y)
{
	//��¼x�����ڻ�����
	float orighnalX=x;
	int size=strlen(str);
	//ͳ���ַ���������ȥ���з����ո�������ڴ�����̬����
	int count=0;
	int ch;
	for(int i=0;i<size;i++)
	{
		ch=str[i];
		switch(ch)
		{
		case 10:
		case 32:
			break;
		default:
			if(this->getTexture(ch)==-1)
			{
				//LOGI("AsciiFont","no texture then continue:%d",i);
				continue;
			}
			count++;
		}
	}
	Point2f** vertexs=(Point2f**)new Point2f*[count];
	GLuint texture;
	for(int i=0,index=0;i<size;i++)
	{
		ch=str[i];
		texture=this->getTexture(ch);
		switch(ch)
		{
		case 10:
			y-=this->lineSpacing;
			x=orighnalX;
			//LOGI("AsciiFont","new lineBase and new charBase:%.2f,%.2f",y,x);
			continue;
		case 32:
			x+=this->charSpacing+this->spaceWidth+this->charSpacing;
			//LOGI("AsciiFont","new charBase:%.2f",x);
			continue;
		default:
			if(texture==-1)
			{
				x+=this->charSpacing+this->spaceWidth+this->charSpacing;
				//LOGI("AsciiFont","no texture and new charBase ch:%d",ch);
				continue;
			}
		}
		vertexs[index]=new Point2f[4];
		//�����������꼰��������
		this->generateChar(vertexs[index],ch,x,y);
		glVertexPointer(2,GL_FLOAT,0,vertexs[index]);
		glTexCoordPointer(2, GL_FLOAT, 0, this->texCoords[ch]);
		glBindTexture(GL_TEXTURE_2D,texture);
		glDrawElements(GL_TRIANGLE_STRIP,4,GL_UNSIGNED_BYTE,vertexIndices);
		x+=deltas[ch].x;
		x+=this->charSpacing;
		index++;
	}
	delete[] vertexs;
}
void AsciiFont::generateChar(Point2f* &vertex,const int& index,const float& x,const float& y)
{

	/*
	 * -----------------------------<---��������߾���Ϊtop����offsets[index].y
	 * -----------------------------
	 * 	a...x...z
	 * -----------------------------<---����
	 * -----------------------------
	 *
	 *	|  |
	 *	| a|
	 *	|  |
	 *	+
	 *	|
	 *	|
	 *	left Ϊ�ַ�����ߵľ��룬�ַ�����Ϊ����������Ϊ����
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
void AsciiFont::setCharSpacing(const float& distance)
{
	this->charSpacing=distance;
}
void AsciiFont::setLineSpacing(const float& distance)
{
	this->lineSpacing=distance;
}
void AsciiFont::setColor(const unsigned char (&color)[4])
{
	if(this->color[0]==color[0]&&this->color[1]==color[1]&&this->color[2]==color[2]&&this->color[3]==color[3])
		return;
	this->color[0]=color[0];
	this->color[1]=color[1];
	this->color[2]=color[2];
	this->color[3]=color[3];
	//ȫ�������ؽ�
	for(int i=0;i<256;i++)
	{
		if(this->buffers[i]!=NULL)
			generateTexture(i,this->color);
	}
}
void AsciiFont::generateTexture(const int& ch,const unsigned char (&color)[4])
{
	GLubyte* buffer=this->buffers[ch];
	if(buffer==NULL)
	{
		LOGW("AsciiFont","generateTexture(%d,%ulx) fail,for buffer==NULL",ch,color);
		return;
	}
	//��ȡ��ɫ����
	int red=0xFF&color[0];
	int green=0xFF&color[1];
	int blue=0xFF&color[2];
	int alpha=0xFF&color[3];

	int width=this->deltas[ch].x;
	int height=this->deltas[ch].y;
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
			//��Ϊwidth��height�Ǿ�����bitmap.width,bitmap.rows����ceilPower�����õ��ģ�
			//���Զ��������Ĳ���Ӧ����0��ʾ
			value=(i>=height||j>=width)?0:buffer[bufferIndex++];
			data[column]=value*red>>8;
			data[column+1]=value*green>>8;
			data[column+2]=value*blue>>8;
			data[column+3]=value*alpha>>8;
		}
	}

	//����������������������
	glBindTexture(GL_TEXTURE_2D, textures[ch]);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D( GL_TEXTURE_2D,0,GL_RGBA,newWidth,newHeight,0,GL_RGBA,GL_UNSIGNED_BYTE,data);

	delete[] data;
}
