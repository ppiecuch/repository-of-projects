#ifndef _YON_SCENE_IGRAPHICSADAPTER_H_
#define _YON_SCENE_IGRAPHICSADAPTER_H_

#include "IReferencable.h"
#include "SColor.h"

namespace yon{
namespace scene{

	class IGraphicsAdapter : public virtual core::IReferencable{
	public:
		virtual ~IGraphicsAdapter(){}

		virtual void beginBatch(u32 layerId) = 0;
		virtual void endBatch() = 0;

		//绘制成功返回true，否则返回false
		virtual bool drawImage(c8* imageName, s32 srcX, s32 srcY, u32 srcWidth, u32 srcHeight, s32 destX, s32 destY,bool useAlpha=false, u32 color=0xFFFFFFFF) = 0;
	};
}
}
#endif