#ifndef _YON_SCENE_ANIMATOR_CANIMATORCAMERAFPS_H_
#define _YON_SCENE_ANIMATOR_CANIMATORCAMERAFPS_H_

#include "IAnimatorCameraFPS.h"
#include "yonArray.h"
#include "SKeyMap.h"
#include "ICursorControl.h"

namespace yon{
namespace scene{
namespace animator{

	class CAnimatorCameraFPS : public IAnimatorCameraFPS{
	protected:
		f32 m_fMoveSpeed;
		f32 m_fRotateSpeed;
		core::array<event::SKeyMap> m_keyMap;
		bool m_keyStates[event::ENUM_KEY_ACTION_COUNT];

		core::position2df m_centerCursor, m_cursorPos;

		u32 m_uLastTime;
		bool m_bFirstUpdated;

		platform::ICursorControl* m_pCursorControl;

		void allKeysUp();
	public:
		CAnimatorCameraFPS(platform::ICursorControl* cursorControl,f32 moveSpeed=0.5f,f32 rotateSpeed=0.001f,event::SKeyMap* keyMapArray=0,s32 keyMapSize=0);
		~CAnimatorCameraFPS();

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