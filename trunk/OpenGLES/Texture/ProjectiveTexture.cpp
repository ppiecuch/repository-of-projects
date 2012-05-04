#include "macro.h"
#if defined(GL)
#include "config.h"
#include "ProjectiveTexture.h"

#include <gl/glaux.h>
#pragma comment(lib, "glaux.lib")

CProjectiveTexture::CProjectiveTexture(const string& name):mTextureId(-1),mName(name),mDataBuf(NULL)
{
#if 1
	int components;
	if(CPngFile::load(name.c_str(),&mDataBuf, &mWidth, &mHeight, &components)==false)
	{
		return;
	}

	GLenum format;
	switch(components)
	{
	case 1:
		format = GL_LUMINANCE;
		break;
	case 2:
		format = GL_LUMINANCE_ALPHA;
		break;
	case 3:
		format = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		break;
	}

	

	glGenTextures(1, &mTextureId);
	glBindTexture(GL_TEXTURE_2D, mTextureId);
	glTexImage2D(GL_TEXTURE_2D,0,format,mWidth,mHeight,0,format,GL_UNSIGNED_BYTE,mDataBuf);

	//glBindTexture之后再调用以下的函数，否则会出现一些奇怪现象
	GLfloat borderColor[4] = {0.6f, 0.6f, 0.6f, 1.0f};
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);//使用后贴图不显示了，估计跟硬件支持有关
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,borderColor);


	GLfloat eyePlaneS[] =  {1.0f, 0.0f, 0.0f, 0.0f};
	GLfloat eyePlaneT[] =  {0.0f, 1.0f, 0.0f, 0.0f};
	GLfloat eyePlaneR[] =  {0.0f, 0.0f, 1.0f, 0.0f};
	GLfloat eyePlaneQ[] =  {0.0f, 0.0f, 0.0f, 1.0f};
	
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

	//set up texture generation mode and set the corresponding planes
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
	glTexGenfv(GL_S,GL_EYE_PLANE,eyePlaneS);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
	glTexGenfv(GL_T,GL_EYE_PLANE,eyePlaneT);
	glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
	glTexGenfv(GL_R,GL_EYE_PLANE,eyePlaneR);
	glTexGeni(GL_Q,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
	glTexGenfv(GL_Q,GL_EYE_PLANE,eyePlaneQ);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_GEN_Q);


	

	printf("load texture %s(components:%d,w:%d,h:%d,id:%d) success\n",name.c_str(),components,mWidth,mHeight,mTextureId);

#else

	GLfloat eyePlaneS[] =  {1.0f, 0.0f, 0.0f, 0.0f};
	GLfloat eyePlaneT[] =  {0.0f, 1.0f, 0.0f, 0.0f};
	GLfloat eyePlaneR[] =  {0.0f, 0.0f, 1.0f, 0.0f};
	GLfloat eyePlaneQ[] =  {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat borderColor[4] = {0.6f, 0.6f, 0.6f, 1.0f};

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

	//set up texture generation mode and set the corresponding planes
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
	glTexGenfv(GL_S,GL_EYE_PLANE,eyePlaneS);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
	glTexGenfv(GL_T,GL_EYE_PLANE,eyePlaneT);
	glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
	glTexGenfv(GL_R,GL_EYE_PLANE,eyePlaneR);
	glTexGeni(GL_Q,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
	glTexGenfv(GL_Q,GL_EYE_PLANE,eyePlaneQ);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_GEN_Q);

	AUX_RGBImageRec *Image;
	wchar_t* text=c2w(name.c_str());
	Image = auxDIBImageLoad(text);
	free(text);
	glGenTextures(1,&mTextureId);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,borderColor);

	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,Image->sizeX,Image->sizeY,GL_RGB,GL_UNSIGNED_BYTE,Image->data);

	printf("load texture %s(w:%d,h:%d,id:%d) success\n",name.c_str(),Image->sizeX,Image->sizeY,mTextureId);

	if(Image)
	{
		if(Image->data)
			free(Image->data);
		free(Image);
	}

#endif
	
}

CProjectiveTexture::~CProjectiveTexture(void)
{
	if(mDataBuf)
	{
		delete[] mDataBuf;
	}
	if(mTextureId>-1)
	{
		glDeleteTextures(1, &mTextureId);
		printf("delete texture %s success\n",mName.c_str());
	}
}
void CProjectiveTexture::textureProjection(Matrix4x4f &mv)
{
	Matrix4x4f &inverseMV = Matrix4x4f::invertMatrix(mv);
	//float MVmatrix[16];

	//here is where we do the transformations on the texture matrix for the lightmap
	//the basic equation here is M = Bias * Scale * ModelView for light map * Inverse Modelview 
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glTranslatef(0.5f,0.5f,0.5f); //Bias
	glScalef(0.5f,0.5f,0.5f);		//Scale
	//glGetFloatv(GL_TEXTURE_MATRIX,MVmatrix);
	//printf("%.2f\t%.2f\t%.2f\t%.2f\n%.2f\t%.2f\t%.2f\t%.2f\n%.2f\t%.2f\t%.2f\t%.2f\n%.2f\t%.2f\t%.2f\t%.2f\n\n",MVmatrix[0],MVmatrix[4],MVmatrix[8],MVmatrix[12],MVmatrix[1],MVmatrix[5],MVmatrix[9],MVmatrix[13],MVmatrix[2],MVmatrix[6],MVmatrix[10],MVmatrix[14],MVmatrix[3],MVmatrix[7],MVmatrix[11],MVmatrix[15]);
	//glFrustum(-0.015,0.015,-0.015,0.015,0.1,1.9); //MV for light map
	glOrtho(-0.135,0.135,-0.135,0.135,0.1,0.2); //MV for light map
	//glOrtho(-0.235f,0.235f,-0.235f,0.235f,0.1f,100);
	//glGetFloatv(GL_TEXTURE_MATRIX,MVmatrix);
	//printf("%.2f\t%.2f\t%.2f\t%.2f\n%.2f\t%.2f\t%.2f\t%.2f\n%.2f\t%.2f\t%.2f\t%.2f\n%.2f\t%.2f\t%.2f\t%.2f\n\n",MVmatrix[0],MVmatrix[4],MVmatrix[8],MVmatrix[12],MVmatrix[1],MVmatrix[5],MVmatrix[9],MVmatrix[13],MVmatrix[2],MVmatrix[6],MVmatrix[10],MVmatrix[14],MVmatrix[3],MVmatrix[7],MVmatrix[11],MVmatrix[15]);
	gluLookAt(1,0,1,-1,0,-1,0,1,0);

	//glGetFloatv(GL_TEXTURE_MATRIX,MVmatrix);
	//printf("%.2f\t%.2f\t%.2f\t%.2f\n%.2f\t%.2f\t%.2f\t%.2f\n%.2f\t%.2f\t%.2f\t%.2f\n%.2f\t%.2f\t%.2f\t%.2f\n\n",MVmatrix[0],MVmatrix[4],MVmatrix[8],MVmatrix[12],MVmatrix[1],MVmatrix[5],MVmatrix[9],MVmatrix[13],MVmatrix[2],MVmatrix[6],MVmatrix[10],MVmatrix[14],MVmatrix[3],MVmatrix[7],MVmatrix[11],MVmatrix[15]);
	
	//glMultMatrixf(inverseMV.getMatrix());	//Inverse ModelView
	glMatrixMode(GL_MODELVIEW);
}
void CProjectiveTexture::bind()
{
	glBindTexture(GL_TEXTURE_2D, mTextureId);
}
#endif