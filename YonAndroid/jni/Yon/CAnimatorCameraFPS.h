#ifndef _YON_SCENE_ANIMATOR_CANIMATORCAMERAFPS_H_
#define _YON_SCENE_ANIMATOR_CANIMATORCAMERAFPS_H_

#include "IAnimatorCameraFPS.h"
#include "yonArray.h"
#include "SKeyMap.h"

namespace yon{
namespace scene{
namespace animator{

	class CAnimatorCameraFPS : public IAnimatorCameraFPS{
	protected:
		f32 m_fMoveSpeed;
		core::array<SKeyMap> m_keyMap;

		void allKeysUp();
	public:
		CAnimatorCameraFPS(f32 moveSpeed=0.5f,SKeyMap* keyMapArray=0);
		~CAnimatorCameraFPS();

		virtual f32 getMoveSpeed() const{return m_fMoveSpeed;}
		virtual void setMoveSpeed(f32 moveSpeed){m_fMoveSpeed=moveSpeed;}

		virtual void setKeyMap(SKeyMap *map, u32 count);
	};
}
}
}
#endif