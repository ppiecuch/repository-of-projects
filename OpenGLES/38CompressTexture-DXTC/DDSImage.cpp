// DDSImage.cpp : 定义 DLL 应用程序的导出函数。
//

#include "DDSImage.h"
#include "nv_dds.h"

#include <gl\glew.h>
#include <gl\gl.h>
#include <gl\glu.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")

unsigned BuildDDS(char *filePath, unsigned &texID)
{
	nv_dds::CDDSImage image;
	if(!image.load(string(filePath)))
	{
		return 0;
	}

	glGenTextures(1, &texID);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID);

	glCompressedTexImage2D(GL_TEXTURE_2D, 0, image.get_format(), 
		image.get_width(), image.get_height(), 0, image.get_size(), image);

	for (int i = 0; i < image.get_num_mipmaps(); i++)
	{
		glCompressedTexImage2D(GL_TEXTURE_2D, i+1, image.get_format(), 
			image.get_mipmap(i).get_width(), image.get_mipmap(i).get_height(), 0, 
			image.get_mipmap(i).get_size(), image.get_mipmap(i));
	} 
	return image.get_components() < 4 ? 1 : 2;
}
