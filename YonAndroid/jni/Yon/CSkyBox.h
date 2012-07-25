#ifndef _YON_SCENE_SKY_CSKYBOX_H_
#define _YON_SCENE_SKY_CSKYBOX_H_

#include "IModel.h"
#include "IUnit.h"

namespace yon{
namespace scene{
namespace sky{

	class CSkyBox : public IModel{
	private:
		enum ENUM_FACE{
			ENUM_FACE_FRONT = 0,
			ENUM_FACE_BACK,
			ENUM_FACE_LEFT,
			ENUM_FACE_RIGHT,
			ENUM_FACE_TOP,
			ENUM_FACE_BOTTOM,

			ENUM_FACE_COUNT
		};
		SVertex m_vertices[24];
		u16 m_indices[4];
		video::IMaterial* m_pMaterial;
		video::ITexture* m_pTextures[ENUM_FACE_COUNT];
		//TODO
		//aabbox3df
	public:
		CSkyBox(IModel* parent,video::ITexture* front, video::ITexture* back, video::ITexture* left,
			video::ITexture* right, video::ITexture* top, video::ITexture* bottom);
		~CSkyBox();

		virtual void render(video::IVideoDriver* driver);

		virtual void onRegisterForRender();

		//virtual const core::aabbox3d<f32>& getBoundingBox() const;

		virtual u32 getMaterialCount() const{
			return 1;
		}

		virtual video::IMaterial* getMaterial(u32 num) const{
			if(num>=getMaterialCount())
				return NULL;
			return m_pMaterial;
		}
	};
}
}
}
#endif