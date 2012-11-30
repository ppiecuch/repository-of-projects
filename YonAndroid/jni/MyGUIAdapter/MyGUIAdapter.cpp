#include "MyGUIAdapter.h"

namespace MyGUI{

	MyGUIAdapter::MyGUIAdapter(const io::IFileSystem* fs,video::IVideoDriver* driver,ITimer* timer,const scene::IGeometryFactory* geometryFty)
		:m_pDataManager(NULL),m_pRenderManager(NULL),m_pLogManager(new MyGUI::LogManager()),m_resourceFileName("MyGUI_Core.xml"){
			m_pRenderManager = new MyGUIRenderManager(driver,timer,geometryFty);
			m_pDataManager = new MyGUIDataManager(fs);

			m_pGUI = new MyGUI::Gui();
			m_pGUI->initialise(m_resourceFileName);

			MyGUI::LogManager::getInstance().createDefaultSource("MyGUI.log");

			YON_INFO(YON_LOG_SUCCEED_FORMAT,"Instance MyGUIAdapter");
	}
	MyGUIAdapter::~MyGUIAdapter(){
		m_pGUI->shutdown();
		delete m_pGUI;
		m_pGUI=nullptr;

		delete m_pRenderManager;
		delete m_pDataManager;
		delete m_pLogManager;
		m_pRenderManager=nullptr;
		m_pDataManager=nullptr;
		m_pLogManager=nullptr;

		YON_INFO(YON_LOG_SUCCEED_FORMAT,"Release MyGUIAdapter");
	}

	void MyGUIAdapter::onResize(const core::dimension2du& size){
		m_pRenderManager->setViewSize(size.w,size.h);
	}

	void MyGUIAdapter::render(){
		m_pRenderManager->drawOneFrame();
	}

	MyGUIAdapter* createMyGUIAdapter(const io::IFileSystem* fs,video::IVideoDriver* driver,ITimer* timer,const scene::IGeometryFactory* geometryFty){
		return new MyGUIAdapter(fs,driver,timer,geometryFty);
	}

}