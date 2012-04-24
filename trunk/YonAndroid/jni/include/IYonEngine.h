#ifndef _YON_IYONENGINE_H_
#define _YON_IYONENGINE_H_


#include "config.h"
#include "IReferencable.h"
#include "SYonPlatformInfo.h"
#include "ITimer.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "IFileSystem.h"
#include "IEventReceiver.h"
#include "IGraphicsAdapter.h"
#include "IAudioDriver.h"


namespace yon{

	class IYonEngine:public virtual core::IReferencable{
	public:
		virtual ~IYonEngine(){};
		virtual video::IVideoDriver* getVideoDriver() const= 0;
		virtual scene::ISceneManager* getSceneManager() const = 0;
		virtual scene::IGraphicsAdapter* getGraphicsAdapter() const = 0;
		virtual io::IFileSystem* getFileSystem() const = 0;
		virtual audio::IAudioDriver* getAudioDriver() const = 0;
		virtual bool run() = 0;
		virtual void onResize(u32 w,u32 h) = 0;

		//TODO��Ϊ�̳���IEventSender
		virtual bool postEventFromUser(const event::SEvent& evt) = 0;
		//����
		//�������¼�ʱ��
		//virtual void update() = 0;

		virtual ITimer* getTimer() = 0;
		//virtual bool getPlatformInfo(SYonPlatformInfo& info) = 0;
	protected:
		virtual void createDriver() = 0;
	};
}

#endif