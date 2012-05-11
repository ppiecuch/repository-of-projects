#ifndef _YON_GUI_MYGUI_IMYGUIENVIRENMENT_H_
#define _YON_GUI_MYGUI_IMYGUIENVIRENMENT_H_

#include <MyGUI.h>
#include "MyGUI_Prerequest.h"
#include "IMyGUIRenderManager.h"
#include "MyGUIDataManager.h"
#include "MyGUI_LogManager.h"

#include "IFileSystem.h"
#include "IVideoDriver.h"
#include "IEventReceiver.h"
#include "IGUIEnvirenment.h"

namespace yon{
namespace gui{
namespace mygui{

	class IMyGUIEnvirenment : public IGUIEnvirenment{
	protected:
		MyGUIDataManager* m_pDataManager;
		MyGUI::LogManager* m_pLogManager;
		MyGUI::Gui* m_pGUI;
		std::string m_resourceFileName;
	public:
		IMyGUIEnvirenment(io::IFileSystem* fs,video::IVideoDriver* driver,ITimer* timer)
			:m_pDataManager(NULL),m_pLogManager(new MyGUI::LogManager()),m_resourceFileName("MyGUI_Core.xml"){
				MyGUI::LogManager::getInstance().createDefaultSource("MyGUI.log");
		}
		virtual ~IMyGUIEnvirenment(){
			delete m_pLogManager;
		}

		//virtual bool postEventFromUser(const event::SEvent& evt) = 0;

		virtual IMyGUIRenderManager* getRenderManagerPtr() = 0;

		MyGUIDataManager* getDataManagerPtr(){
			return m_pDataManager;
		}
	};

}
	IGUIEnvirenment* createGUIEnvirenment(io::IFileSystem* fs,video::IVideoDriver* driver,ITimer* timer);
}
}
#endif