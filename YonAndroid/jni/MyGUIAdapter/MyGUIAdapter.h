#ifndef _MYGUI_MYGUIADAPTER_H_
#define _MYGUI_MYGUIADAPTER_H_

#ifdef MYGUI_ADAPTER_EXPORTS
#define MYGUI_ADAPTER_API __declspec(dllexport)
#else
#define MYGUI_ADAPTER_API __declspec(dllimport)
#endif

#ifdef ANDROID_NDK
#undef MYGUI_ADAPTER_API
#define MYGUI_ADAPTER_API
#endif

#include <MyGUI.h>
#include "MyGUI_Prerequest.h"
#include "MyGUIRenderManager.h"
#include "MyGUIDataManager.h"
#include "MyGUI_LogManager.h"

#include "IFileSystem.h"
#include "IVideoDriver.h"
#include "IEventReceiver.h"
#include "IGeometryFactory.h"

using namespace yon;

namespace MyGUI{

	class MYGUI_ADAPTER_API MyGUIAdapter{
	private:
		MyGUIDataManager* m_pDataManager;
		MyGUI::LogManager* m_pLogManager;
		MyGUIRenderManager* m_pRenderManager;
		MyGUI::Gui* m_pGUI;
		std::string m_resourceFileName;
		std::string m_rootMedia;
	public:
		MyGUIAdapter(const io::IFileSystem* fs,video::IVideoDriver* driver,ITimer* timer,const scene::IGeometryFactory* geometryFty);
		virtual ~MyGUIAdapter();

		MyGUIRenderManager* getRenderManagerPtr(){
			return m_pRenderManager;
		}

		MyGUIDataManager* getDataManagerPtr(){
			return m_pDataManager;
		}

		virtual void render();

		virtual void onResize(const core::dimension2du& size);

		//virtual bool postEventFromUser(const event::SEvent& evt) = 0;
	};

	MYGUI_ADAPTER_API MyGUIAdapter* createMyGUIAdapter(const io::IFileSystem* fs,video::IVideoDriver* driver,ITimer* timer,const scene::IGeometryFactory* geometryFty);

}
#endif