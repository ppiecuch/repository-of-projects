#ifndef _YON_VIDEO_OGLES1_COGLES1FBOTEXTURE_H_
#define _YON_VIDEO_OGLES1_COGLES1FBOTEXTURE_H_

#include "COGLES1Texture.h"

namespace yon{
namespace video{
namespace ogles1{

	class COGLES1FBOTexture : public COGLES1Texture{
		GLuint m_fboId;
		GLuint m_rboId;
		ENUM_COLOR_FORMAT m_format;
		bool checkFBOError();
	public:
		COGLES1FBOTexture(const core::dimension2du& size,
			const io::path& name,COGLES1Driver* driver, const ENUM_COLOR_FORMAT format);
		~COGLES1FBOTexture();

		virtual ENUM_COLOR_FORMAT getColorFormat() const{
			return m_format;
		}

		virtual void beginRTT(bool clearBackBuffer, bool clearZBuffer,video::SColor color);
		virtual void endRTT(bool willRenderFrameBuffer);
	};
}
}
}
#endif