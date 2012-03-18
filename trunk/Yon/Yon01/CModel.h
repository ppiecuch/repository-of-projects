#ifndef _YON_SCENE_CMODEL_H_
#define _YON_SCENE_CMODEL_H_

#include "IModel.h"
#include "IEntity.h"

namespace yon{
	namespace scene{

		class CModel : public IModel{
		public:
			CModel(IEntity* entity,IModel* parent);
			~CModel();

			virtual void render(video::IVideoDriver* driver);
			virtual void setEntity(IEntity* entity);
		protected:
			IEntity* m_entity;
		};

	}
}
#endif