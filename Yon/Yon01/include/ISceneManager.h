#ifndef _YON_SCENE_ISCENEMANAGER_H_
#define _YON_SCENE_ISCENEMANAGER_H_

#include "IReferencable.h"

namespace yon{
	namespace scene{

		class ISceneManager : public virtual core::IReferencable{
		public:
			virtual ~ISceneManager() {}

			virtual void render() = 0;
		};

	}//scene
}//yon
#endif