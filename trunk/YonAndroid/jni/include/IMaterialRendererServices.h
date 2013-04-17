#ifndef _YON_VIDEO_IMATERIALRENDERERSERVICES_H_
#define _YON_VIDEO_IMATERIALRENDERERSERVICES_H_

#include "SMaterial.h"
#include "SVertex.h"

namespace yon{
namespace video{

	class IMaterialRendererServices{
	public:
		virtual ~IMaterialRendererServices() {}

		virtual void setBasicRenderStates(const SMaterial& material,const SMaterial& lastMaterial,bool resetAllRenderstates) = 0;

		//virtual bool setVertexShaderConstant(const c8* name, const f32* floats, int count) = 0;
		//virtual void setVertexShaderConstant(const f32* data, s32 startRegister, s32 constantAmount=1) = 0;

		//virtual bool setFragmentShaderConstant(const c8* name, const f32* floats, int count) = 0;
		//virtual void setFragmentShaderConstant(const f32* data, s32 startRegister, s32 constantAmount=1) = 0;

		//virtual IVideoDriver* getVideoDriver() = 0;
	};
}
}
#endif