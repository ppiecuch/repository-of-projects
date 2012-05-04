#include "macro.h"
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>
#include <stdio.h>
using namespace std;

class TEXTURE_API CTexture
{
private:
	const string mName;
	unsigned int mTextureId;
	unsigned int mWidth, mHeight;
	unsigned char* mDataBuf;
public:
	CTexture(const string& name);
	~CTexture(void);

	void bind();
};

#endif
