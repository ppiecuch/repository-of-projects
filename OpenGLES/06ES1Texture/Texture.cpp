#include "Texture.h"

CTexture::CTexture(const string& name):mTextureId(-1),mName(name),mDataBuf(NULL)
{
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
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	printf("load texture %s(w:%d,h:%d,id:%d) success\n",name.c_str(),mWidth,mHeight,mTextureId);
	GL_CHECK
}

CTexture::~CTexture(void)
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
void CTexture::bind()
{
	glBindTexture(GL_TEXTURE_2D, mTextureId);
}
