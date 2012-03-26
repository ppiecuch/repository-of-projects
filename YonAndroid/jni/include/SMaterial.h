#ifndef _YON_VIDEO_SMATERIAL_H_
#define _YON_VIDEO_SMATERIAL_H_

#include "config.h"
#include "SMaterialLayer.h"

namespace yon{
namespace video{

	enum ENUM_MATERIAL_TYPE{
		ENUM_MATERIAL_TYPE_SOLID = 0,
		ENUM_MATERIAL_TYPE_COUNT
	};

	const u32 MATERIAL_MAX_TEXTURES = YON_MATERIAL_MAX_TEXTURES;

	struct SMaterial{
		ENUM_MATERIAL_TYPE materialType;
		SMaterialLayer textureLayers[MATERIAL_MAX_TEXTURES];

		void setTexture(u32 i, ITexture* tex)
		{
			if (i>=MATERIAL_MAX_TEXTURES)
				return;
			textureLayers[i].texture = tex;
		}
		ITexture* getTexture(u32 i) const
		{
			return i < MATERIAL_MAX_TEXTURES ? textureLayers[i].texture : 0;
		}

		inline bool operator!=(const SMaterial& b) const
		{
			bool different =materialType != b.materialType;
			for (u32 i=0; (i<MATERIAL_MAX_TEXTURES) && !different; ++i)
			{
				different |= (textureLayers[i] != b.textureLayers[i]);
			}
			return different;
		}
		inline bool operator==(const SMaterial& b) const
		{ 
			return !(b!=*this);
		}
	};

	YON_API extern const SMaterial DEFAULT_MATERIAL;

}//video
}//yon
#endif