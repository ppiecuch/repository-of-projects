#include "macro.h"
#ifndef _SHADOW_TEXTURE_H_
#define _SHADOW_TEXTURE_H_

#include "matrix4x4.h"
#include <string>
#include <stdio.h>
using namespace std;

class TEXTURE_API ShadowTexture
{
private:
	const string mName;
	unsigned int mTextureId;
	unsigned int mWidth, mHeight;
	unsigned char* mDataBuf;
public:
	ShadowTexture(const string& name,unsigned int width,unsigned int height);
	~ShadowTexture(void);
	void preRender();
	void postRender();
	unsigned int getWidth() const{return mWidth;}
	unsigned int getHeight() const{return mHeight;}
	void bind();
};
#endif
