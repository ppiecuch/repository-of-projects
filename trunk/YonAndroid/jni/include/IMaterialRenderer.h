#ifndef _YON_VIDEO_IMATERIALRENDERER_H_
#define _YON_VIDEO_IMATERIALRENDERER_H_

#include "IReferencable.h"
//#include "IMaterial.h"
#include "SMaterial.h"

namespace yon{
namespace video{

	class IMaterialRendererServices;

	struct SInnerMaterial;
	class IMaterialRenderer : public virtual core::IReferencable{
	public:
		//virtual void onSetMaterial(const IMaterial* current,const IMaterial* last) {}
		virtual void preprocess(SInnerMaterial& im){}
		virtual void onSetMaterial(const SInnerMaterial& current,const SInnerMaterial& last) {}
		virtual void onSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,bool resetAllRenderstates, IMaterialRendererServices* services) {}
		virtual void onUnsetMaterial() {}
	};
}
}
#endif