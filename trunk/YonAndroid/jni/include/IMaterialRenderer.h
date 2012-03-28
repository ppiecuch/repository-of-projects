#ifndef _YON_VIDEO_IMATERIALRENDERER_H_
#define _YON_VIDEO_IMATERIALRENDERER_H_

#include "IReferencable.h"
#include "IMaterial.h"

namespace yon{
namespace video{

	class IVideoDriver;
	class IMaterialRenderer;
	IMaterialRenderer* createMaterialRendererSolid(IVideoDriver* driver);

	class IMaterialRenderer : public virtual core::IReferencable{
	public:
		virtual void onSetMaterial(const IMaterial* material) {}
		virtual void onUnsetMaterial() {}
	};
}//video
}//yon
#endif