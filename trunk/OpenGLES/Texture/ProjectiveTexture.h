#include "macro.h"
#if defined(GL)
#ifndef _PROJECTIVE_TEXTURE_H_
#define _PROJECTIVE_TEXTURE_H_

#include "matrix4x4.h"
#include <string>
#include <stdio.h>
using namespace std;

class TEXTURE_API CProjectiveTexture
{
private:
	const string mName;
	unsigned int mTextureId;
	unsigned int mWidth, mHeight;
	unsigned char* mDataBuf;
public:
	CProjectiveTexture(const string& name);
	~CProjectiveTexture(void);

	void textureProjection(Matrix4x4f &mv);
	void bind();
};
#endif
#endif