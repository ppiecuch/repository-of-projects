#ifndef _YON_VIDEO_IMATERIALRENDERER_H_
#define _YON_VIDEO_IMATERIALRENDERER_H_

#include "IReferencable.h"
#include "IMaterial.h"

namespace yon{
namespace video{

	class IMaterialRenderer : public virtual core::IReferencable{
	public:
		virtual void onSetMaterial(const IMaterial* current,const IMaterial* last) {}
		virtual void onUnsetMaterial() {}
	};
}
}
#endif