#ifndef _YON_VIDEO_SDUMMYTEXTURE_H_
#define _YON_VIDEO_SDUMMYTEXTURE_H_

#include "ITexture.h"

namespace yon{
namespace video{

	struct SDummyTexture : public ITexture{
		SDummyTexture(const io::path& p) : ITexture(p){}
		void setName(const io::path& p){m_namePath.setPath(p);}
		virtual const core::dimension2du& getSize() const{return m_size;}
		virtual bool hasMipMap() const{return false;}
		virtual ENUM_COLOR_FORMAT getColorFormat() const{return ENUM_COLOR_FORMAT_R8G8B8A8;}
		virtual void* lock(){return NULL;}
		virtual void unlock(){}
		virtual void upsidedown(){}
		virtual bool isRenderTarget() const{return false;}
		virtual void beginRTT(bool clearBackBuffer, bool clearZBuffer,video::SColor color){}
		virtual void endRTT(bool willRenderFrameBuffer){}
		core::dimension2du m_size;
	};
}
}
#endif