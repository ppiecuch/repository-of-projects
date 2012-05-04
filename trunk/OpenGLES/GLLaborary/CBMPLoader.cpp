//======================================================================
/**
*  @file      CBMPLoader.cpp
*
*  ��Ŀ������ ����ӳ��
*  �ļ�����:  ����λͼ�� 
*  ����ƽ̨�� Windows98/2000/NT/XP
*  
*  ���ߣ�     WWBOSS
*  �����ʼ�:  wwboss123@gmail.com
*  �������ڣ� 2006-11-23	
*  �޸����ڣ� 2007-07-10
*	
*/																			
//======================================================================
#include "stdafx.h"
#include"CBMPLoader.h"              /**< ����ͷ�ļ� */

/** ���캯�� */
CBMPLoader::CBMPLoader()
{
   /** ��ʼ����ԱֵΪ0 */
	image = 0;
	imageWidth = 0;
	imageHeight = 0;
}

/** �������� */
CBMPLoader::~CBMPLoader()
{
   FreeImage(); /**< �ͷ�ͼ������ռ�ݵ��ڴ� */
}

/** װ��һ��λͼ�ļ� */
bool CBMPLoader::LoadBitmap(const char *file)
{
	FILE *pFile = 0; /**< �ļ�ָ�� */
	
	/** ����λͼ�ļ���Ϣ��λͼ�ļ�ͷ�ṹ */
	BITMAPINFOHEADER bitmapInfoHeader;
	BITMAPFILEHEADER header;
  
	unsigned char textureColors = 0;/**< ���ڽ�ͼ����ɫ��BGR�任��RGB */

   /** ���ļ�,�������� */
	pFile = fopen(file, "rb");
		if(pFile == 0) return false;

	/** ����λͼ�ļ�ͷ��Ϣ */ 
	fread(&header, sizeof(BITMAPFILEHEADER), 1, pFile);
	
	/** �����ļ��Ƿ�Ϊλͼ�ļ� */
	if(header.bfType != BITMAP_ID)
	   {
		   fclose(pFile);             /**< ������λͼ�ļ�,��ر��ļ������� */
		   return false;
	   }

	/** ����λͼ�ļ���Ϣ */
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);

	/** ����ͼ��Ŀ�Ⱥ͸߶� */
	imageWidth = bitmapInfoHeader.biWidth;
    imageHeight = bitmapInfoHeader.biHeight;

    /** ȷ����ȡ���ݵĴ�С */
   if(bitmapInfoHeader.biSizeImage == 0)
      bitmapInfoHeader.biSizeImage = bitmapInfoHeader.biWidth *
      bitmapInfoHeader.biHeight * 3;

	/** ��ָ���Ƶ����ݿ�ʼλ�� */
	fseek(pFile, header.bfOffBits, SEEK_SET);

	/** �����ڴ� */
	image = new unsigned char[bitmapInfoHeader.biSizeImage];

	/** ����ڴ�����Ƿ�ɹ� */
	if(!image)                        /**< �������ڴ�ʧ���򷵻� */
	   {
		   delete[] image;
		   fclose(pFile);
		   return false;
	   }

	/** ��ȡͼ������ */
	fread(image, 1, bitmapInfoHeader.biSizeImage, pFile);

	/** ��ͼ����ɫ���ݸ�ʽ���н���,��BGRת��ΪRGB */
	for(int index = 0; index < (int)bitmapInfoHeader.biSizeImage; index+=3)
	   {
		   textureColors = image[index];
		   image[index] = image[index + 2];
		   image[index + 2] = textureColors;
	   }
  
	fclose(pFile);       /**< �ر��ļ� */
	return true;         /**< �ɹ����� */
}

/** ����λͼ�ļ������������� */
bool CBMPLoader::Load(const char* fileName)
{
	if(!LoadBitmap(fileName))
	{
		MessageBox(NULL,_T("����λͼ�ļ�ʧ��!"),_T("����"),MB_OK);
		exit(0);
	}
	/** ��������������� */
	glGenTextures(1, &ID);
   
    /** ����������� */
    glBindTexture(GL_TEXTURE_2D, ID);
	
	/** �����˲� */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
   
	/** �������� */
   	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, imageWidth,
	                  imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
	                  image);
   return true;
}
bool CBMPLoader::LoadTemp()
{
	if(!LoadBitmap("media/blend.bmp"))
	{
		MessageBox(NULL,_T("����λͼ�ļ�ʧ��!"),_T("����"),MB_OK);
		exit(0);
	}
// 	imageWidth=imageHeight=5;
// 	image=new unsigned char[imageHeight*imageWidth*4];
// 	for(int i=0;i<imageWidth*imageHeight*4;i++)
// 	{
// 		image[i]=0;
// 	}
	//image[0]=image[1]=image[2]=image[3]=255;

	unsigned char* temp=new unsigned char[imageHeight*imageWidth*4];
	for(int i=0;i<imageWidth*imageHeight;i++)
	{
		temp[i*4]=image[i*3];
		temp[i*4+1]=image[i*3];
		temp[i*4+2]=image[i*3];
		temp[i*4+3]=0;
	}
	FreeImage();
	image=temp;
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, imageWidth,
		imageHeight, GL_RGBA, GL_UNSIGNED_BYTE,
		image);
	return true;
}

/** �ͷ��ڴ� */
void CBMPLoader::FreeImage()
{
   /** �ͷŷ�����ڴ� */
   if(image)
      {
         delete[] image;
         image = 0;
      }
}
