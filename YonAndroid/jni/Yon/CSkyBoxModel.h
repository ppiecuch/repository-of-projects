#ifndef _YON_SCENE_SKY_CSKYBOXMODEL_H_
#define _YON_SCENE_SKY_CSKYBOXMODEL_H_

#include "IModel.h"
#include "IUnit.h"

namespace yon{
namespace scene{
namespace sky{

	class CSkyBoxModel : public IModel{
	private:
		SVertex m_vertices[24];
		u16 m_indices[4];
		video::IMaterial* m_pMaterials;
		//TODO
		//aabbox3df
	public:
		CSkyBoxModel(IModel* parent,video::ITexture* top, video::ITexture* bottom, video::ITexture* left,
			video::ITexture* right, video::ITexture* front, video::ITexture* back);

		virtual void render(video::IVideoDriver* driver);

		virtual void onRegisterForRender(ISceneManager* manager);

		virtual u32 getMaterialCount() const{
			return 6;
		}

		virtual video::IMaterial* getMaterial(u32 num) const{
			if(num>=getMaterialCount())
				return NULL;
			return m_pMaterials[num];
		}
	};
}
}
}
#endif