#ifndef _YON_VIDEO_OGLES1_COGLES1TEXTURE_H_
#define _YON_VIDEO_OGLES1_COGLES1TEXTURE_H_

#include "ITexture.h"
#include "COGLES1Driver.h"

namespace yon{
namespace video{
namespace ogles1{

	class COGLES1Texture : public ITexture{
	private:
		core::dimension2d<u32> m_textureSize;

		COGLES1Driver* m_pDriver;
		IImage* m_pImage;

		GLuint m_textureId;
		

		virtual void uploadTexture();
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

		
	};
}//ogles1
}//video
}//yon
#endif