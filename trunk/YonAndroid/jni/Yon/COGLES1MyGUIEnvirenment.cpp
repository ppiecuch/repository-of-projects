
#include "COGLES1MyGUIEnvirenment.h"

#include "COGLES1MyGUIRenderManager.h"
#include "MyGUIDataManager.h"

#include "ILogger.h"

namespace yon{
namespace gui{
namespace mygui{
namespace ogles1{

	COGLES1MyGUIEnvirenment::COGLES1MyGUIEnvirenment(io::IFileSystem* fs,video::IVideoDriver* driver,ITimer* timer)
		:m_bIsInitialise(false),IMyGUIEnvirenment(fs,driver,timer){
			m_pRenderManager = new COGLES1MyGUIRenderManager(static_cast<video::ogles1::COGLES1Driver*>(driver),timer);
			m_pDataManager = new MyGUIDataManager(fs);
			//initialise();

			m_pGUI = new MyGUI::Gui();

			Logger->info(YON_LOG_SUCCEED_FORMAT,"Instance COGLES1MyGUIEnvirenment");
	}
	COGLES1MyGUIEnvirenment::~COGLES1MyGUIEnvirenment(){
		m_pGUI->shutdown();
		delete m_pGUI;
		m_pGUI=nullptr;

		shutdown();
		delete m_pRenderManager;
		delete m_pDataManager;
		m_pRenderManager=nullptr;
		m_pDataManager=nullptr;

		Logger->info(YON_LOG_SUCCEED_FORMAT,"Release COGLES1MyGUIEnvirenment");
	}

	//void COGLES1MyGUIEnvirenment::setViewSize(int _width, int _height){
	//	m_pRenderManager->setViewSize(_width,_height);
	//}
	//bool COGLES1MyGUIEnvirenment::postEventFromUser(const event::SEvent& evt)
	void COGLES1MyGUIEnvirenment::onResize(const core::dimension2du& size){
		m_pRenderManager->setViewSize(size.w,size.h);
	}

	void COGLES1MyGUIEnvirenment::render(){
		//glLoadIdentity();
		m_pRenderManager->drawOneFrame();
	}

	void COGLES1MyGUIEnvirenment::setupResources()
	{
		MyGUI::xml::Document doc;

		if (!doc.open(std::string("resources.xml"))){
			doc.getLastError();
			Logger->error(YON_LOG_FAILED_FORMAT,"open resources.xml");
		}

		MyGUI::xml::ElementPtr root = doc.getRoot();
		if (root == nullptr || root->getName() != "Paths")
			return;

		MyGUI::xml::ElementEnumerator node = root->getElementEnumerator();
		while (node.next())
		{
			if (node->getName() == "Path")
			{
				bool root = false;
				if (node->findAttribute("root") != "")
				{
					root = MyGUI::utility::parseBool(node->findAttribute("root"));
					if (root) m_rootMedia = node->getContent();
				}
				addResourceLocation(node->getContent(), false);
			}
		}

		addResourceLocation(m_rootMedia + "/Common/Base");
	}

	void COGLES1MyGUIEnvirenment::addResourceLocation(const std::string& _name, bool _recursive)
	{
		m_pDataManager->addResourceLocation(_name, _recursive);
	}

	void COGLES1MyGUIEnvirenment::init(){
		m_pRenderManager->initialise();
		m_pDataManager->initialise();
		m_pGUI->initialise(m_resourceFileName);
		m_bIsInitialise=true;
	}
	void COGLES1MyGUIEnvirenment::shutdown(){
		m_pRenderManager->shutdown();
		m_pDataManager->shutdown();
		m_bIsInitialise=false;
	}
}
}
	IGUIEnvirenment* createGUIEnvirenment(io::IFileSystem* fs,video::IVideoDriver* driver,ITimer* timer){
		return new mygui::ogles1::COGLES1MyGUIEnvirenment(fs,driver,timer);
	}
}
}