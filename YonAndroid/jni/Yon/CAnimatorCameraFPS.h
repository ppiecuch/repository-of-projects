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
		core::array<event::SKeyMap> m_keyMap;
		bool m_keyStates[event::ENUM_KEY_ACTION_COUNT];

		u32 m_uLastTime;
		bool m_bFirstUpdated;

		void allKeysUp();
	public:
		CAnimatorCameraFPS(f32 moveSpeed=0.5f,event::SKeyMap* keyMapArray=0,s32 keyMapSize=0);

		virtual f32 getMoveSpeed() const{return m_fMoveSpeed;}
		virtual void setMoveSpeed(f32 moveSpeed){m_fMoveSpeed=moveSpeed;}

		virtual void animateNode(IModel* model, u32 timeMs);

		virtual void setKeyMap(event::SKeyMap *map, s32 count);

		virtual bool onEvent(const event::SEvent& event);
	};
}
}
}
#endif