#ifndef _YON_VIDEO_ITEXTURE_H_
#define _YON_VIDEO_ITEXTURE_H_

#include "IReferencable.h"
#include "path.h"
#include "dimension2d.h"
#include "IImage.h"
#include "SColor.h"

namespace yon{
namespace video{

	enum MASK_TEXTURE_CREATION_CONFIG{
		MASK_TEXTURE_CREATION_CONFIG_NONE			= 0,
		MASK_TEXTURE_CREATION_CONFIG_RESERVE_IMAGE	= 0x00000001,
		MASK_TEXTURE_CREATION_CONFIG_16BIT_1ALPHA	= 0x00000002,//与MASK_TEXTURE_CREATION_CONFIG_16BIT_4ALPHA互斥
		MASK_TEXTURE_CREATION_CONFIG_16BIT_4ALPHA	= 0x00000004,//与MASK_TEXTURE_CREATION_CONFIG_16BIT_1ALPHA互斥
		MASK_TEXTURE_CREATION_CONFIG_MIPMAPS		= 0x00000008
	};

	class ITexture : public virtual core::IReferencable{
	protected:
		io::SNamedPath m_namePath;
	public:
		ITexture(const io::path& p) : m_namePath(p)
		{}
		const io::path& getName() const { return m_namePath.getInternalName(); }

		virtual const core::dimension2du& getSize() const = 0;

		virtual bool hasMipMap() const = 0;

		virtual ENUM_COLOR_FORMAT getColorFormat() const = 0;

		virtual void* lock() = 0;
		virtual void unlock() = 0;
		virtual void upsidedown() = 0;

		//释放GPU上的纹理对象
		virtual void logon() = 0;
		virtual void logoff() = 0;

		//RTT相关
		virtual bool isRenderTarget() const = 0;
		virtual void beginRTT(bool clearBackBuffer=true, bool clearZBuffer=true,video::SColor color=video::COLOR_ZERO) = 0;
		//参数renderFrameBuffer：是否接下来的操作重新指向帧缓冲
		virtual void endRTT(bool willRenderFrameBuffer) = 0;
	};
}
}
#endif