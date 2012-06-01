#include "config.h"
#include "Texture.h"
#include <stdio.h>
#include <wtypes.h>
#include <stdarg.h>
#include <tchar.h>

inline void TRACE(const char * pszFormat, ...)
{
	va_list pArgs;
	char szMessageBuffer[16380]={0};
	va_start( pArgs, pszFormat );
	vsnprintf_s( szMessageBuffer, 16380,16380-1,pszFormat, pArgs );
	va_end( pArgs );   
	OutputDebugStringA(szMessageBuffer);   
}


CTexture::CTexture(const string& name):mTextureId(0),mName(name),mDataBuf(NULL)
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
		printf("GL_LUMINANCE\n");
		break;
	case 2:
		format = GL_LUMINANCE_ALPHA;
		printf("GL_LUMINANCE_ALPHA\n");
		break;
	case 3:
		format = GL_RGB;
		printf("GL_RGB\n");
		break;
	case 4:
		format = GL_RGBA;
		printf("GL_RGBA\n");
		break;
	}
	glGenTextures(1, &mTextureId);
	glBindTexture(GL_TEXTURE_2D, mTextureId);
	glTexImage2D(GL_TEXTURE_2D,0,format,mWidth,mHeight,0,format,GL_UNSIGNED_BYTE,mDataBuf);
	//gluBuild2DMipmaps(GL_TEXTURE_2D,format,mWidth,mHeight,format,GL_UNSIGNED_BYTE,mDataBuf);
	//2.0²»Ö§³ÖGL_PERSPECTIVE_CORRECTION_HINT
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
#if defined(GLES2)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
#elif defined(GL)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
#endif
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	TRACE("load texture %s(w:%d,h:%d,id:%d) success\n",name.c_str(),mWidth,mHeight,mTextureId);
}

CTexture::~CTexture(void)
{
	if(mDataBuf)
	{
		delete[] mDataBuf;
	}
	if(mTextureId)
	{
		glDeleteTextures(1, &mTextureId);
		TRACE("delete texture %s success\n",mName.c_str());
	}
	TRACE("~CTexture\n");
}
void CTexture::bind()
{
	glBindTexture(GL_TEXTURE_2D, mTextureId);
}
