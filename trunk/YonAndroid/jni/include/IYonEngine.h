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
//#include "IRandomizer.h"
#include "ICallback.h"
#include "II18NManager.h"
#include "INetManager.h"
#include "ITextSystem.h"

namespace yon{

	struct SYonEngineParameters;

	class IYonEngine:public virtual core::IReferencable{
	public:
		virtual ~IYonEngine(){};
		virtual const SYonEngineParameters& getCreateParameters()=0;
		virtual video::IVideoDriver* getVideoDriver() const= 0;
		virtual scene::ISceneManager* getSceneManager() const = 0;
		//virtual gui::IGUIEnvirenment* getGUIEnvirentment() const = 0;
		virtual scene::IGraphicsAdapter* getGraphicsAdapter() const = 0;
		virtual scene::IGraphicsAdapter* getGraphicsAdapterWindow() const = 0;
		virtual io::IFileSystem* getFileSystem() const = 0;
		virtual audio::IAudioDriver* getAudioDriver() const = 0;
		//virtual IRandomizer* getRandomizer() = 0;
		virtual i18n::II18NManager* getI18NManager() = 0;
		virtual net::INetManager* getNetManager() = 0;
		virtual text::ITextSystem* getTextSystem() = 0;

		//�������ʼ��������ʧ�ܺ�available�᷵��false(����crash����)
		virtual bool available(){return true;}
		//�����Ƿ���ʾ�У���������С��ʱ�����治��ʾ
		virtual bool displaying(){return true;}
		virtual bool run() = 0;
		virtual void onResize(u32 w,u32 h) = 0;

		virtual bool callback(const platform::SCallback& cb){return false;}
		//TODO��Ϊ�̳���IEventSender
		virtual bool postEventFromUser(const event::SEvent& evt) = 0;
		//����
		//�������¼�ʱ��
		//virtual void update() = 0;

		virtual ITimer* getTimer() = 0;
		//virtual bool getPlatformInfo(SYonPlatformInfo& info) = 0;
	};
}

#endif