
#include "CAnimatorCameraFPS.h"
#include "ICamera.h"

namespace yon{
namespace scene{
namespace animator{

	CAnimatorCameraFPS::CAnimatorCameraFPS(f32 moveSpeed,event::SKeyMap* keyMapArray,s32 keyMapSize):
		m_fMoveSpeed(moveSpeed),m_uLastTime(0),m_bFirstUpdated(true)
	{
		allKeysUp();

		if(!keyMapArray||keyMapSize==0)
		{
			m_keyMap.push_back(event::SKeyMap(event::ENUM_KEY_ACTION_MOVE_FORWARD,event::ENUM_KEY_W));
			m_keyMap.push_back(event::SKeyMap(event::ENUM_KEY_ACTION_MOVE_BACKWARD,event::ENUM_KEY_S));
			m_keyMap.push_back(event::SKeyMap(event::ENUM_KEY_ACTION_MOVE_LEFTWARD,event::ENUM_KEY_A));
			m_keyMap.push_back(event::SKeyMap(event::ENUM_KEY_ACTION_MOVE_RIGHTWARD,event::ENUM_KEY_D));
		}
		else
		{
			setKeyMap(keyMapArray, keyMapSize);
		}
	}

	void CAnimatorCameraFPS::animateNode(IModel* model, u32 timeMs)
	{
		//TODO check model type

		camera::ICamera* camera=static_cast<camera::ICamera*>(model);

		if(m_bFirstUpdated)
		{
			m_uLastTime=timeMs;
			m_bFirstUpdated=false;
		}
		f32 timeDiff = (f32)(timeMs-m_uLastTime);
		m_uLastTime=timeMs;

		core::vector3df pos = camera->getPosition();
		core::vector3df target = camera->getTarget();
		core::vector3df direction = camera->getDirection();

		direction.normalize();

		core::vector3df offset=core::ORIGIN_VECTOR3DF;

		if (m_keyStates[event::ENUM_KEY_ACTION_MOVE_FORWARD])
			offset += direction;

		if (m_keyStates[event::ENUM_KEY_ACTION_MOVE_BACKWARD])
			offset -= direction;

		core::vector3df right = camera->getDirection();
		right = right.crossProduct(camera->getUpVector());

		//if (NoVerticalMovement)
		right.y = 0.0f;

		right.normalize();

		if (m_keyStates[event::ENUM_KEY_ACTION_MOVE_LEFTWARD])
			offset -= right;

		if (m_keyStates[event::ENUM_KEY_ACTION_MOVE_RIGHTWARD])
			offset += right;

		offset.normalize();
		offset=offset* timeDiff * m_fMoveSpeed;
		pos+=offset;


		camera->setPosition(pos);
		target = camera->getDirection()+pos;
		camera->setTarget(target);
	}

	void CAnimatorCameraFPS::allKeysUp()
	{
		for (u32 i=0; i<event::ENUM_KEY_ACTION_COUNT; ++i)
			m_keyStates[i] = false;
	}


	void CAnimatorCameraFPS::setKeyMap(event::SKeyMap *map, s32 count)
	{
		// clear the keymap
		m_keyMap.clear();

		// add actions
		for (s32 i=0; i<count; ++i)
		{
			switch(map[i].action)
			{
			case event::ENUM_KEY_ACTION_MOVE_FORWARD: 
				m_keyMap.push_back(event::SKeyMap(event::ENUM_KEY_ACTION_MOVE_FORWARD, map[i].keycode));
				break;
			case event::ENUM_KEY_ACTION_MOVE_BACKWARD: 
				m_keyMap.push_back(event::SKeyMap(event::ENUM_KEY_ACTION_MOVE_BACKWARD, map[i].keycode));
				break;
			case event::ENUM_KEY_ACTION_MOVE_LEFTWARD: 
				m_keyMap.push_back(event::SKeyMap(event::ENUM_KEY_ACTION_MOVE_LEFTWARD, map[i].keycode));
				break;
			case event::ENUM_KEY_ACTION_MOVE_RIGHTWARD: 
				m_keyMap.push_back(event::SKeyMap(event::ENUM_KEY_ACTION_MOVE_RIGHTWARD, map[i].keycode));
				break;
			default:
				break;
			}
		}
	}


	bool CAnimatorCameraFPS::onEvent(const event::SEvent& evt)
	{
		switch(evt.type)
		{
		case event::ENUM_EVENT_TYPE_KEY:
			for (u32 i=0; i<m_keyMap.size(); ++i)
			{
				if (m_keyMap[i].keycode == evt.keyInput.key)
				{
					m_keyStates[m_keyMap[i].action] = evt.keyInput.pressDown;
					return true;
				}
			}
			break;

		default:
			break;
		}

		return false;
	}
}
}
}