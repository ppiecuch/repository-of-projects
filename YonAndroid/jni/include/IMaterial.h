#ifndef _YON_VIDEO_IMATERIAL_H_
#define _YON_VIDEO_IMATERIAL_H_

#include "IReferencable.h"
#include "ITexture.h"

namespace yon{
namespace video{

	enum ENUM_MATERIAL_TYPE{
		ENUM_MATERIAL_TYPE_SOLID = 0,
		ENUM_MATERIAL_TYPE_LIGHTEN,
		ENUM_MATERIAL_TYPE_TRANSPARENT,
		ENUM_MATERIAL_TYPE_TRANSPARENT_BLEND_COLOR,
		ENUM_MATERIAL_TYPE_COUNT
	};

	const static u32 MATERIAL_MAX_TEXTURES = YON_MATERIAL_MAX_TEXTURES;

	class IMaterial : public virtual core::IReferencable{
	public:
		virtual void setTexture(u32 i, ITexture* tex) = 0;
		virtual ITexture* getTexture(u32 i) const = 0;
		virtual ENUM_MATERIAL_TYPE getMaterialType() const = 0;
		virtual void setMaterialType(ENUM_MATERIAL_TYPE type) = 0;
	};
}
}
#endif