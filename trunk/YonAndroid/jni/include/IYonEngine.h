#ifndef _YON_IYONENGINE_H_
#define _YON_IYONENGINE_H_


#include "yonConfig.h"
#include "IReferencable.h"
#include "SYonPlatformInfo.h"
#include "ITimer.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"
//#include "IGUIEnvirenment.h"
#include "IFileSystem.h"
#include "IEventReceiver.h"
#include "IGraphicsAdapter.h"
#include "IAudioDriver.h"
#include "IRandomizer.h"
#include "ICallback.h"

namespace yon{

	class IYonEngine:public virtual core::IReferencable{
	public:
		virtual ~IYonEngine(){};
		virtual video::IVideoDriver* getVideoDriver() const= 0;
		virtual scene::ISceneManager* getSceneManager() const = 0;
		//virtual gui::IGUIEnvirenment* getGUIEnvirentment() const = 0;
		virtual scene::IGraphicsAdapter* getGraphicsAdapter() const = 0;
		virtual io::IFileSystem* getFileSystem() const = 0;
		virtual audio::IAudioDriver* getAudioDriver() const = 0;
		virtual IRandomizer* getRandomizer() = 0;

		virtual bool run() = 0;
		virtual void onResize(u32 w,u32 h) = 0;

		virtual bool callback(const platform::SCallback& cb){return false;}
		//TODO改为继承自IEventSender
		virtual bool postEventFromUser(const event::SEvent& evt) = 0;
		//更新
		//包括更新计时器
		//virtual void update() = 0;

		virtual ITimer* getTimer() = 0;
		//virtual bool getPlatformInfo(SYonPlatformInfo& info) = 0;
	protected:
		virtual void createDriver() = 0;
	};
}

#endif