#include "UnicodePainter.h"

UnicodePainter::UnicodePainter(FT_Byte* &dataBase,const unsigned long dataSize,const unsigned int size)
	:charSpacing(0),spaceWidth(0),color(0x000000FF)
{

	this->size=size;
	this->dataBase=dataBase;
//	this->dataSize=dataSize;
	int error;
	// ����FreeType��
	if (error=FT_Init_FreeType( &library ))
	{
		LOGE("UnicodePainter","FT_Init_FreeType ERROR:%d",error);
		return;
	}
	//����FT_Face����FreeType�������ڱ���������Ϣ
	//���ڴ洴��face
	if(error=FT_New_Memory_Face(library,dataBase,dataSize,0,&face))
	{
		LOGE("UnicodePainter","FT_New_Memory_Face ERROR:%d",error);
		// �ͷ�FreeType��
		FT_Done_FreeType(library);
		return;
	}
	// �趨ΪUNICODE��Ĭ��Ҳ��
	if(error=FT_Select_Charmap(face, FT_ENCODING_UNICODE))
	{
		LOGE("UnicodePainter","FT_Select_Charmap ERROR:%d",error);
		// �ͷ�FreeType��
		FT_Done_FreeType(library);
		return;
	}
	//��FreeType��ʹ��1/64��Ϊһ�����صĸ߶�����������Ҫ����charWidth,charHeight���������Ҫ��
	//����ֱ��ʺʹ�ֱ�ֱ��ʶ�ʹ��96
	if(error=FT_Set_Char_Size( face, size << 6, size << 6, 96,96))
	{
		LOGE("UnicodePainter","FT_Set_Char_Size ERROR:%d",error);
		// �ͷ�face��
		FT_Done_Face(face);
		// �ͷ�FreeType��
		FT_Done_FreeType(library);
		return;
	}
//	int index;
//	FT_Glyph glyph;
	//�����м��Ϊascender�������ߵ��ַ�������ߵ�ľ���, ������26.6�Ķ���������˻�ȡ����������Ҫ����64
	this->lineSpacing = face->size->metrics.ascender>>6;
	LOGI("UnicodePainter","lineSpacing:%d",this->lineSpacing);
	//���ÿո���Ϊ�����/2
	this->spaceWidth=face->size->metrics.max_advance>>7;
	LOGI("UnicodePainter","spaceWidth:%d",this->spaceWidth);
}

UnicodePainter::~UnicodePainter()
{
	//�ͷ�FreeType��
	if(library!=NULL)
	{
		LOGI("UnicodePainter","FT_Done_FreeType",NULL);
		FT_Done_FreeType(library);
	}
	//�ͷ�face��
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
		//������
		HASH::iterator iterator=fonts.find(unicode);
		//��������ڴ��ַ�������֮
		if(iterator==fonts.end())
		{
			LOGI("UnicodePainter","unicode[%lx] not found,creating",unicode);
			//��ȡ����unicode��face���ַ�ӳ����е�glyph����
			int index=FT_Get_Char_Index(face,unicode);
			int error;
			FT_Glyph glyph;
			if(index==0)
			{
				LOGW("UnicodePainter","FT_Get_Char_Index:%lx-->%d(undefined character code)",unicode,index);
				continue;
			}
			//����load_flags�����ô�face��ȡglyph�����浽glyph slot��
			//������loadʱ���������ļ���load��ʱbitmapped image��������outline��
			//�������������е�bitmapped image�����⣬�Ӷ�����޷������������ġ�
			//�����������ʹ��FT_LOAD_DEFAULT|FT_LOAD_NO_BITMAP
			//��ǿ����loadʱ��loadoutline��Ȼ���ٸ���outline�õ�bitmapped image
			if(error=FT_Load_Glyph( face, index, FT_LOAD_DEFAULT|FT_LOAD_NO_BITMAP))
			{
				LOGW("UnicodePainter","FT_Load_Glyph ERROR:%d",error);
				continue;
			}
			//��face->glyph��face��glyph slot����ȡglyph����
			if(error=FT_Get_Glyph(face->glyph, &glyph))
			{
				LOGW("UnicodePainter","FT_Get_Glyph ERROR:%d",error);
				continue;
			}
			//���glyph��ͼ���ʽ��������Ӧ����Ⱦ����glyph��Ⱦ��λͼ
			//FT_RENDER_MODE_NORMAL��1����ʹ��8λ�Ҷȼ��Ŀ����λͼ
			//FT_RENDER_MODE_MONO��1����ʹ��1λ�Ҷȼ���λͼ
			if(error=FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL))
			{
				LOGW("UnicodePainter","FT_Render_Glyph ERROR:%d",error);
				continue;
			}
			//���glyphͼ����λͼ
			if ( glyph->format != FT_GLYPH_FORMAT_BITMAP )
			{
				//ת��glyph����Ϊλͼglyph�����滻ԭglyph����,0����ƫ�ƣ�1��ɹ�������ԭglyph
				if(error=FT_Glyph_To_Bitmap( &glyph, FT_RENDER_MODE_NORMAL, 0, 1 ))
				{
					LOGW("UnicodePainter","FT_Glyph_To_Bitmap ERROR:%d",error);
					continue;
				}
			}
			FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph;
			FT_Bitmap bitmap=bitmapGlyph->bitmap;

			//����λͼ���ݵ��ҶȻ�����
			GLubyte* buffer = new GLubyte[bitmap.width * bitmap.rows];
			memcpy(buffer,bitmap.buffer,bitmap.width * bitmap.rows);

			//bitmapGlyph->left;// �ַ�������߽�ľ���
			//bitmapGlyph->top;// �ַ���ߵ������ߵľ���

			Font* font=new Font(buffer,size,color,bitmap.width,bitmap.rows,bitmapGlyph->left,bitmapGlyph->top);

			//��ΪOpenGLֻ֧�������ȺͿ����2��n�η�����������Ҫת��һ��
			int width=ceilPower(bitmap.width);
			int height=ceilPower(bitmap.rows);
			LOGI("UnicodePainter","width:%d,height:%d,left:%d,top:%d",width,height,bitmapGlyph->left,bitmapGlyph->top);
			//�����������꣬����width��height���ַ�����ʵ������С��һ����ȣ�����Ҫ������������
			float dx=(float)bitmap.width/width;
			float dy=(float)bitmap.rows/height;
			dy=1-dy;//��Ϊy�����Ƿ�ת�ˣ�����ɫ�Ǵ������µģ����Կհ�����data���Ϸ�
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
