#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "config.h"
#include <string>
#include <stdio.h>
using namespace std;

class CTexture
{
private:
	const string mName;
	GLuint mTextureId;
	unsigned int mWidth, mHeight;
	unsigned char* mDataBuf;
public:
	CTexture(const string& name);
	~CTexture(void);

	void bind();
};

#endif
