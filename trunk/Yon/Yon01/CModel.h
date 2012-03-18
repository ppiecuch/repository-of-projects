#ifndef _YON_SCENE_CMODEL_H_
#define _YON_SCENE_CMODEL_H_

#include "IModel.h"
#include "IEntity.h"

namespace yon{
	namespace scene{

		class CModel : public IModel{
		public:
			CModel(IEntity* entity);

			virtual void render();
			virtual void setEntity(IEntity* entity);
		protected:
			IEntity* m_entity;
		};

	}
}
#endif