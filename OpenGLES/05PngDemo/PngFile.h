#ifndef _PNG_FILE_
#define _PNG_FILE_

#include "png.h"
#include <stdio.h>

class CPngFile
{
private:
	static void user_error_fn(png_structp png_ptr, png_const_charp error_message);
	static void user_warning_fn(png_structp png_ptr, png_const_charp warning_message);
public:
	CPngFile(void);
	~CPngFile(void);

	bool static load(const char* file,png_bytepp dataBuf,unsigned int *imgWidth, unsigned int *imgHeight, int *components);
};

#endif
