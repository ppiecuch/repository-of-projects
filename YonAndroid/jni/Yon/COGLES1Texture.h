#ifndef _YON_VIDEO_OGLES1_COGLES1TEXTURE_H_
#define _YON_VIDEO_OGLES1_COGLES1TEXTURE_H_

#include "ITexture.h"
#include "COGLES1Driver.h"

namespace yon{
namespace video{
namespace ogles1{

	class COGLES1Texture : public ITexture{
	protected:
		core::dimension2d<u32> m_textureSize;

		COGLES1Driver* m_pDriver;
		IImage* m_pImage;

		GLuint m_textureId;
		
		bool m_bIsRenderTarget;
		bool m_bHasMipMap;
		bool m_bReserveImage;
		bool m_bUse16Bit1Alpha;
		bool m_bUse16Bit4Alpha;

		core::recti m_viewport;

		//返回图像信息是否正常
		bool checkImageInfo(video::IImage* image);
		//返回是否执行转换处理
		bool convertImageFormat(video::IImage* image);
		virtual void uploadTexture();

		//用于子类构造
		COGLES1Texture(const core::dimension2du& size,const io::path& name, COGLES1Driver* driver);
	public:
		COGLES1Texture(video::IImage* image,const io::path& name,COGLES1Driver* driver);
		virtual ~COGLES1Texture();

		GLuint getTextureId() const{
			return m_textureId;
		}

		virtual const core::dimension2du& getSize() const{
			return m_textureSize;
		}

		virtual ENUM_COLOR_FORMAT getColorFormat() const{
			return m_pImage->getColorFormat();
		}

		virtual bool hasMipMap() const{
			return m_bHasMipMap;
		}

		virtual void* lock();
		virtual void unlock();
		virtual void upsidedown();

		virtual void logon();
		virtual void logoff();

		virtual bool isRenderTarget() const{return m_bIsRenderTarget;}
		virtual void setIsRenderTarget(bool on){m_bIsRenderTarget=on;}
		//deprecated
		//virtual void beginRTT(bool clearBackBuffer, bool clearZBuffer,video::SColor color);
		//virtual void endRTT(bool willRenderFrameBuffer);
		virtual void beginRTT();
		virtual void endRTT();
	};
}//ogles1
}//video
}//yon
#endif