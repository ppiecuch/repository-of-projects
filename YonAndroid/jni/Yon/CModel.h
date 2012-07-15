#ifndef _YON_SCENE_CMODEL_H_
#define _YON_SCENE_CMODEL_H_

#include "IModel.h"
#include "IEntity.h"
#include "ISceneManager.h"

namespace yon{
	namespace scene{

		class CModel : public IModel{
		public:
			CModel(IModel* parent,IEntity* entity,
				const core::vector3df& pos=core::vector3df(0,0,0),
				const core::vector3df& rot=core::vector3df(0,0,0),
				const core::vector3df& scale=core::vector3df(1,1,1));
			~CModel();

			virtual void render(video::IVideoDriver* driver);
			virtual void setEntity(IEntity* entity);

			virtual IEntity* getEntity(){
				return m_entity;
			}

			virtual void onRegisterForRender(ISceneManager* manager)
			{
				if(m_bVisible)
				{
					manager->registerForRender(this);

					IModel::onRegisterForRender(manager);
				}
			}

			virtual video::IMaterial* getMaterial(u32 i) const{
				if(i>=getMaterialCount())
					return NULL;
				return m_entity->getUnit(i)->getMaterial();
			}
			virtual u32 getMaterialCount() const{
				return m_entity->getUnitCount();
			}

		protected:
			IEntity* m_entity;
		};

	}
}
#endif