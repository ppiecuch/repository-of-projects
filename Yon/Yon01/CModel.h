#ifndef _YON_SCENE_CMODEL_H_
#define _YON_SCENE_CMODEL_H_

#include "IModel.h"
#include "IEntity.h"

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
		protected:
			IEntity* m_entity;
		};

	}
}
#endif