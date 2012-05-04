#include "ShadowTexture.h"
#include "config.h"

ShadowTexture::ShadowTexture(const string& name,unsigned int width,unsigned int height):mWidth(width),mHeight(height),mTextureId(-1),mName(name),mDataBuf(NULL)
{
	glGenTextures(1, &mTextureId);
}

ShadowTexture::~ShadowTexture(void)
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
void ShadowTexture::preRender()
{
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, mWidth, mHeight);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, mTextureId);
#if defined(GLES2)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
#elif defined(GL)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
#endif
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
}
void ShadowTexture::postRender()
{
	glCopyTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,0,0,mWidth,mHeight,0);

	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glCullFace(GL_BACK);
	printf("generate shadow texture %s(w:%d,h:%d,id:%d) success\n",mName.c_str(),mWidth,mHeight,mTextureId);
}
void ShadowTexture::bind()
{
	glBindTexture(GL_TEXTURE_2D, mTextureId);
}
