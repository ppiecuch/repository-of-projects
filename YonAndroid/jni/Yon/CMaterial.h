#ifndef _YON_VIDEO_CMATERIAL_H_
#define _YON_VIDEO_CMATERIAL_H_

#include "IMaterial.h"
#include "SMaterialLayer.h"

#include "ILogger.h"

namespace yon{
namespace video{

	class CMaterial : public IMaterial{
	protected:
		ENUM_MATERIAL_TYPE m_materialType;
		ENUM_POLYGON_MODE m_polygonMode;
		SMaterialLayer m_textureLayers[MATERIAL_MAX_TEXTURES];
	public:

		CMaterial()
			:m_materialType(ENUM_MATERIAL_TYPE_SOLID),m_polygonMode(ENUM_POLYGON_MODE_FILL){
				//Logger->debug(YON_LOG_SUCCEED_FORMAT,"Instance CMaterial");
		}
		~CMaterial(){
			//Logger->debug(YON_LOG_SUCCEED_FORMAT,"Release CMaterial");
		}

		virtual ENUM_MATERIAL_TYPE getMaterialType() const{
			return m_materialType;
		}
		virtual void setMaterialType(ENUM_MATERIAL_TYPE type){
			m_materialType=type;
		}

		virtual ENUM_POLYGON_MODE getPolygonMode() const{
			return m_polygonMode;
		}
		virtual void setPolygonMode(ENUM_POLYGON_MODE mode){
			m_polygonMode= mode;
		}

		virtual void setTexture(u32 i, ITexture* tex)
		{
			if (i>=MATERIAL_MAX_TEXTURES)
				return;
			m_textureLayers[i].texture = tex;
		}
		ITexture* getTexture(u32 i) const
		{
			return i < MATERIAL_MAX_TEXTURES ? m_textureLayers[i].texture : 0;
		}
	};
}//video
}//yon
#endif