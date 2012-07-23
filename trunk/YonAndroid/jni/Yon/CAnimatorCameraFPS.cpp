
#include "CAnimatorCameraFPS.h"
#include "ICamera.h"

#include "ILogger.h"

namespace yon{
namespace scene{
namespace animator{

	CAnimatorCameraFPS::CAnimatorCameraFPS(platform::ICursorControl* cursorControl,f32 moveSpeed,f32 rotateSpeed,event::SKeyMap* keyMapArray,s32 keyMapSize)
		:m_pCursorControl(cursorControl),m_fMoveSpeed(moveSpeed),m_fRotateSpeed(rotateSpeed),m_uLastTime(0),m_bFirstUpdated(true)
	{
		if(m_pCursorControl)
			m_pCursorControl->grab();

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

	CAnimatorCameraFPS::~CAnimatorCameraFPS()
	{
		if(m_pCursorControl)
			m_pCursorControl->drop();
	}

	void CAnimatorCameraFPS::animateNode(IModel* model, u32 timeMs)
	{
		//TODO check model type

		camera::ICamera* camera=static_cast<camera::ICamera*>(model);

		if(m_bFirstUpdated)
		{
			if (m_pCursorControl)
			{
				m_pCursorControl->setPosition(0.5f, 0.5f);
				m_cursorPos = m_centerCursor = m_pCursorControl->getRelativePosition();
			}
			m_uLastTime=timeMs;
			m_bFirstUpdated=false;
		}
		f32 timeDiff = (f32)(timeMs-m_uLastTime);
		m_uLastTime=timeMs;

		
		core::vector3df pos = camera->getPosition();
		core::vector3df target = camera->getTarget();
		core::vector3df direction = camera->getDirection();
		core::vector3df up = camera->getUpVector();

		if(m_pCursorControl)
		{
			core::vector3df relativeRotation = direction.getHorizontalAngle();
			//core::stringc str("relativeRotation:%.2f,%.2f,%.2f->",relativeRotation.x,relativeRotation.y,relativeRotation.z);
			if(m_cursorPos!=m_centerCursor)
			{
				relativeRotation.y -= (m_cursorPos.x-0.5f) * m_fRotateSpeed;
				relativeRotation.x += (m_cursorPos.y-0.5f) * m_fRotateSpeed;

				const f32 maxAngle=85.0f;
				if(relativeRotation.x>maxAngle&&relativeRotation.x<=90.0f)
					relativeRotation.x=maxAngle;
				else if(relativeRotation.x<180.0f-maxAngle&&relativeRotation.x>=90.0f)
					relativeRotation.x=180.0f-maxAngle;
				else if(relativeRotation.x<(360.0f-maxAngle)&&relativeRotation.x>=270.0f)
					relativeRotation.x=360.0f-maxAngle;
				else if(relativeRotation.x>(180.0f+maxAngle)&&relativeRotation.x<=270.0f)
					relativeRotation.x=180.0f+maxAngle;

				m_pCursorControl->setPosition(0.5f, 0.5f);
				m_cursorPos = m_centerCursor = m_pCursorControl->getRelativePosition();

				core::matrix4f mat(true);
				mat.setRotationDegrees(relativeRotation);
				//TODO�Ż�
				f32 len=direction.getLength();
				direction.set(0,0,1);
				direction*=len;
				mat.transformVect(direction);
				mat.transformVect(up);
			}
			//temp
			//relativeRotation = direction.getHorizontalAngle();
			//Logger->debug("%s%.2f,%.2f,%.2f\r\n",str.c_str(),relativeRotation.x,relativeRotation.y,relativeRotation.z);
		}

		core::vector3df tmp=direction;
		direction.normalize();

		core::vector3df offset=core::ORIGIN_VECTOR3DF;

		if (m_keyStates[event::ENUM_KEY_ACTION_MOVE_FORWARD])
			offset += direction;

		if (m_keyStates[event::ENUM_KEY_ACTION_MOVE_BACKWARD])
			offset -= direction;

		core::vector3df right = tmp;
		right = right.crossProduct(up);

		//if (NoVerticalMovement)
		//right.y = 0.0f;

		right.normalize();

		if (m_keyStates[event::ENUM_KEY_ACTION_MOVE_LEFTWARD])
			offset -= right;

		if (m_keyStates[event::ENUM_KEY_ACTION_MOVE_RIGHTWARD])
			offset += right;

		offset.normalize();
		offset=offset* timeDiff * m_fMoveSpeed;
		pos+=offset;


		camera->setPosition(pos);
		target = tmp+pos;
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
		case event::ENUM_EVENT_TYPE_MOUSE:
			if(evt.mouseInput.type==event::ENUM_MOUSE_INPUT_TYPE_MOVE)
			{
				m_cursorPos=m_pCursorControl->getRelativePosition();
				return true;
			}
		default:
			break;
		}

		return false;
	}
}
}
}