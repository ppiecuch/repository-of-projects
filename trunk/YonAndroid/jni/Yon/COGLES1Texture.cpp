#include "COGLES1Texture.h"
#include "COGLES1Driver.h"

#include "CImage.h"
#include "CColorConverter.h"

#include "ILogger.h"

namespace yon{
namespace video{
namespace ogles1{

	COGLES1Texture::COGLES1Texture(const core::dimension2du& size,const io::path& name, COGLES1Driver* driver)
		:ITexture(name),m_pDriver(driver), m_pImage(NULL),
		m_textureId(0),m_bIsRenderTarget(false),m_textureSize(size),m_bHasMipMap(false){}

	COGLES1Texture::COGLES1Texture(video::IImage* image,const io::path& name,COGLES1Driver* driver,bool mipmap)
		:ITexture(name), m_pDriver(driver), m_pImage(image),m_textureSize(image->getDimension()),
		m_textureId(0),m_bIsRenderTarget(false),m_bHasMipMap(mipmap){
			glGenTextures(1, &m_textureId);
			m_pImage->grab();

			//临时
			/*if(image->getColorFormat()==ENUM_COLOR_FORMAT_R8G8B8A8)
			{
				void* data=image->lock();
				u32 temp;
				memcpy(&temp,data,4);
				image->unlock();

				m_pImage=new CImage(ENUM_COLOR_FORMAT_R5G5B5A1,image->getDimension());
				CColorConverter::convert_A8B8G8R8toR5G5B5A1(image->lock(),image->getImageDataSizeInPixels(),m_pImage->lock());
				data=m_pImage->lock();
				u16 temp1;
				memcpy(&temp1,data,2);

				image->unlock();
				m_pImage->unlock();
				Logger->debug(YON_LOG_SUCCEED_FORMAT,core::stringc("convert%08X->%04X",temp,temp1).c_str());
			}
			else if(image->getColorFormat()==ENUM_COLOR_FORMAT_R8G8B8)
			{
				m_pImage=new CImage(ENUM_COLOR_FORMAT_R5G6B5,image->getDimension());
				CColorConverter::convert_R8G8B8toR5G6B5(image->lock(),image->getImageDataSizeInPixels(),m_pImage->lock());
			}
			else
			{
				m_pImage->grab();
				void* data=m_pImage->lock();
				u32 temp;
				memcpy(&temp,data,4);
				Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("warn%08X",temp).c_str());
				m_pImage->unlock();
			}*/

			uploadTexture();

			Logger->debug(YON_LOG_SUCCEED_FORMAT,core::stringc("Instance COGLES1Texture:%d",m_textureId).c_str());
	}
	COGLES1Texture::~COGLES1Texture(){
		if(m_textureId)
			glDeleteTextures(1, &m_textureId);
		if(m_pImage)
			m_pImage->drop();

		Logger->debug(YON_LOG_SUCCEED_FORMAT,core::stringc("Release COGLES1Texture:%d",m_textureId).c_str());
	}

	void getGLFormat(ENUM_COLOR_FORMAT colorFormat,GLenum& format,GLenum& pixelType){
		switch(colorFormat)
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
		case ENUM_COLOR_FORMAT_L8:
			format=GL_LUMINANCE;
			pixelType=GL_UNSIGNED_BYTE;
			break;
		case ENUM_COLOR_FORMAT_A8:
			format=GL_ALPHA;
			pixelType=GL_UNSIGNED_BYTE;
			break;
		case ENUM_COLOR_FORMAT_L8A8:
			format=GL_LUMINANCE_ALPHA;
			pixelType=GL_UNSIGNED_BYTE;
			break;
		default:
			Logger->error(YON_LOG_FAILED_FORMAT,"Unsupported texture format");
			break;
		}
	}

	//TODO GL_DEPTH_COMPONENT支持
	void COGLES1Texture::uploadTexture(){

		GLenum format=0;
		GLenum pixelType=0;

		getGLFormat(m_pImage->getColorFormat(),format,pixelType);

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
		glBindTexture(GL_TEXTURE_2D, m_textureId);
		if(m_bHasMipMap)
		{
			if(m_pDriver->queryFeature(ENUM_VIDEO_FEATURE_GENERATE_MIPMAP))
			{
				glTexParameteri(GL_TEXTURE_2D,GL_GENERATE_MIPMAP,GL_TRUE);
				// enable bilinear mipmap filter
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				Logger->debug("glTexImage2D:%d,%d,%d,mipmap:%d,%s\n", m_textureId,m_pImage->getDimension().w,m_pImage->getDimension().h,hasMipMap(),COLOR_FORMAT_NAME[m_pImage->getColorFormat()]);
			}
			else
			{
				//TODO generate mipmap manually
				// temporarily for has no mipmap
				// just enable bilinear filter without mipmaps
				m_bHasMipMap=false;
				Logger->warn(YON_LOG_WARN_FORMAT,"Currently do not support hw mipmap generation,disable it!");
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				Logger->debug("glTexImage2D:%d,%d,%d,mipmap:%d,%s\n", m_textureId,m_pImage->getDimension().w,m_pImage->getDimension().h,hasMipMap(),COLOR_FORMAT_NAME[m_pImage->getColorFormat()]);
			}
		}
		else
		{
			// enable bilinear filter without mipmaps
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			Logger->debug("glTexImage2D:%d,%d,%d,mipmap:%d,%s\n", m_textureId,m_pImage->getDimension().w,m_pImage->getDimension().h,hasMipMap(),COLOR_FORMAT_NAME[m_pImage->getColorFormat()]);
		}
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_pImage->getDimension().w,m_pImage->getDimension().h, 0, format, pixelType, source);
		m_pImage->unlock();

		//m_pDriver->checkGLError(__FILE__,__LINE__);

		glBindTexture(GL_TEXTURE_2D, tmpTexture);


		//TODO testGLError

	}

	void* COGLES1Texture::lock(){
		glBindTexture(GL_TEXTURE_2D, m_textureId);
		return m_pImage->lock();
	}
	void COGLES1Texture::unlock(){
		GLenum format=0;
		GLenum pixelType=0;

		getGLFormat(m_pImage->getColorFormat(),format,pixelType);

		void* source = m_pImage->lock();
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_pImage->getDimension().w,m_pImage->getDimension().h, format, pixelType, source);
		
		//m_pDriver->checkGLError(__FILE__,__LINE__);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void COGLES1Texture::upsidedown(){
		lock();
		m_pImage->upsidedown();
		unlock();
	}

	void COGLES1Texture::beginRTT(bool clearBackBuffer, bool clearZBuffer,video::SColor color)
	{
		glViewport(0, 0, m_textureSize.w,m_textureSize.h);
		m_pDriver->clearView(clearBackBuffer,clearZBuffer,color);
	}
	void COGLES1Texture::endRTT(bool willRenderFrameBuffer)
	{
		//因为Driver的currentTexture记录的还是之前的textureId，所以这里要做一下恢复
		GLint tmpTexture;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &tmpTexture);

		glBindTexture(GL_TEXTURE_2D, m_textureId);
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, m_textureSize.w,m_textureSize.h);
		//glCopyTexImage2D(GL_TEXTURE_2D,0,GL_RGB,0,0,m_textureSize.w,m_textureSize.h,0);


		//m_pDriver->checkGLError(__FILE__,__LINE__);

		if(willRenderFrameBuffer){
			const video::SClearSetting& setting=m_pDriver->getClearSetting();
			glViewport(0,0,m_pDriver->getCurrentRenderTargetSize().w,m_pDriver->getCurrentRenderTargetSize().h);
			m_pDriver->clearView(setting.clearBackBuffer,setting.clearZBuffer,setting.color);
			glBindTexture(GL_TEXTURE_2D, tmpTexture);
		}
	}
}//ogles1
}//video
}//yon