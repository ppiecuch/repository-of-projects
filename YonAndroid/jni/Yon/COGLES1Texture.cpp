#include "COGLES1Texture.h"
#include "COGLES1Driver.h"

#include "ILogger.h"

namespace yon{
namespace video{
namespace ogles1{

	COGLES1Texture::COGLES1Texture(video::IImage* image,const io::path& name,COGLES1Driver* driver)
		:ITexture(name), m_pDriver(driver), m_pImage(image),m_textureSize(image->getDimension()),
		m_textureId(-1){
			glGenTextures(1, &m_textureId);
			m_pImage->grab();

			uploadTexture();

			Logger->debug(YON_LOG_SUCCEED_FORMAT,"Instance COGLES1Texture");
	}
	COGLES1Texture::~COGLES1Texture(){
		glDeleteTextures(1, &m_textureId);
		m_pImage->drop();

		Logger->debug(YON_LOG_SUCCEED_FORMAT,"Release COGLES1Texture");
	}

	void COGLES1Texture::uploadTexture(){

		GLenum format;
		GLenum pixelType;

		switch (m_pImage->getColorFormat())
		{
		case ENUM_COLOR_FORMAT_R5G5B5A1:
			format=GL_RGBA;
			pixelType=GL_UNSIGNED_SHORT_5_5_5_1;
			break;
		case ENUM_COLOR_FORMAT_R5G6B5:
			format=GL_RGB;
			pixelType=GL_UNSIGNED_SHORT_5_6_5;
			break;
		case ENUM_COLOR_FORMAT_R8G8B8:
			format=GL_RGB;
			pixelType=GL_UNSIGNED_BYTE;
			break;
		case ENUM_COLOR_FORMAT_R8G8B8A8:
			format=GL_RGBA;
			pixelType=GL_UNSIGNED_BYTE;
			break;
		default:
			Logger->error(YON_LOG_FAILED_FORMAT,"Unsupported texture format");
			break;
		}

		//Opengl ES1.1
		//void glTexImage2D(GLenum target,GLint level,GLint internalformat,GLsizei width,GLsizei height,GLint border,GLenum format,GLenum type,const GLvoid * pixels);
		//target 
		//Specifies the target texture. Must be GL_TEXTURE_2D.
		//level
		//Specifies the level-of-detail number. Level 0 is the base image level. Level n is the nth mipmap reduction image. Must be greater or equal 0.
		//internalformat
		//Specifies the color components in the texture. Must be same as format. The following symbolic values are accepted: GL_ALPHA, GL_RGB, GL_RGBA, GL_LUMINANCE, or GL_LUMINANCE_ALPHA.
		//width
		//Specifies the width of the texture image. Must be 2n for some integer n. All implementations support texture images that are at least 64 texels wide.
		//height
		//Specifies the height of the texture image. Must be 2m for some integer m. All implementations support texture images that are at least 64 texels high.
		//border
		//Specifies the width of the border. Must be 0.
		//format
		//Specifies the format of the pixel data. Must be same as internalformat. The following symbolic values are accepted: GL_ALPHA, GL_RGB, GL_RGBA, GL_LUMINANCE, and GL_LUMINANCE_ALPHA.
		//type
		//Specifies the data type of the pixel data. The following symbolic values are accepted: GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_4_4_4_4, and GL_UNSIGNED_SHORT_5_5_5_1.
		//pixels
		//Specifies a pointer to the image data in memory.

		GLint tmpTexture;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &tmpTexture);

		void* source = m_pImage->lock();
		Logger->debug("glTexImage2D:%d,%d,%d,%s\n", m_textureId,m_pImage->getDimension().w,m_pImage->getDimension().h,COLOR_FORMAT_NAME[m_pImage->getColorFormat()]);
		glBindTexture(GL_TEXTURE_2D, m_textureId);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_pImage->getDimension().w,m_pImage->getDimension().h, 0, format, pixelType, source);
		m_pImage->unlock();

		//m_pDriver->checkGLError(__FILE__,__LINE__);

		glBindTexture(GL_TEXTURE_2D, tmpTexture);


		//TODO testGLError

	}
}//ogles1
}//video
}//yon