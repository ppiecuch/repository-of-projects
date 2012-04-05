#ifndef _YON_VIDEO_IMATERIAL_H_
#define _YON_VIDEO_IMATERIAL_H_

#include "IReferencable.h"
#include "ITexture.h"
#include "matrix4.h"

namespace yon{
namespace video{

	enum ENUM_MATERIAL_TYPE{
		ENUM_MATERIAL_TYPE_SOLID = 0,
		ENUM_MATERIAL_TYPE_LIGHTEN,
		ENUM_MATERIAL_TYPE_TRANSPARENT,
		ENUM_MATERIAL_TYPE_TRANSPARENT_BLEND_COLOR,
		ENUM_MATERIAL_TYPE_COAT,
		ENUM_MATERIAL_TYPE_COUNT
	};

	enum ENUM_POLYGON_MODE{
		ENUM_POLYGON_MODE_FILL = 0,
		ENUM_POLYGON_MODE_LINE,
		ENUM_POLYGON_MODE_POINT,
		ENUM_POLYGON_MODE_COUNT
	};

	enum ENUM_WRAP_MODE
	{
		ENUM_WRAP_MODE_REPEAT = 0,
		ENUM_WRAP_MODE_CLAMP_TO_EDGE,
		ENUM_WRAP_MODE_COUNT
	};

	const static u32 MATERIAL_MAX_TEXTURES = YON_MATERIAL_MAX_TEXTURES;

	class IMaterial : public virtual core::IReferencable{
	public:
		virtual ENUM_MATERIAL_TYPE getMaterialType() const = 0;
		virtual void setMaterialType(ENUM_MATERIAL_TYPE type) = 0;

		virtual ENUM_POLYGON_MODE getPolygonMode() const = 0;
		virtual void setPolygonMode(ENUM_POLYGON_MODE mode) = 0;

		virtual void setTexture(u32 i, ITexture* tex) = 0;
		virtual ITexture* getTexture(u32 i) const = 0;

		virtual ENUM_WRAP_MODE getWrapModeU(u32 index) const = 0;
		virtual void setWrapModeU(u32 index,ENUM_WRAP_MODE mode) = 0;
		virtual ENUM_WRAP_MODE getWrapModeV(u32 index) const = 0;
		virtual void setWrapModeV(u32 index,ENUM_WRAP_MODE mode) = 0;

		virtual void setTextureMatrix(u32 index, const core::matrix4f& mat) = 0;
		virtual const core::matrix4f& getTextureMatrix(u32 index) const = 0;
	};

	YON_API extern IMaterial* DEFAULT_MATERIAL;
}
}
#endif