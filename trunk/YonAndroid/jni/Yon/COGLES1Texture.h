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

		virtual bool isRenderTarget() const{return m_bIsRenderTarget;}
		virtual void setIsRenderTarget(bool on){m_bIsRenderTarget=on;}
		virtual void beginRTT(bool clearBackBuffer, bool clearZBuffer,video::SColor color);
		virtual void endRTT(bool willRenderFrameBuffer);
	};
}//ogles1
}//video
}//yon
#endif