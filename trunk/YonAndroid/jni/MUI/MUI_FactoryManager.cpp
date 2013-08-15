#include "MUI_FactoryManager.h"
#include "ILogger.h"

namespace mui{

	template<> FactoryManager* Singleton<FactoryManager>::s_pSingleton = NULL;

	FactoryManager::FactoryManager()
		:m_bInitialized(false){
	}

	FactoryManager::~FactoryManager(){
		m_generators.clear();
	}

	void FactoryManager::initialize(){
		/*
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
		*/
		YON_DEBUG_BREAK_IF(m_bInitialized);
		m_bInitialized=true;
	}

	void FactoryManager::shutdown(){
		/*
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
		*/
		YON_DEBUG_BREAK_IF(!m_bInitialized);
		m_bInitialized=false;
	}

	void FactoryManager::registerFactory(const core::stringc& category, const core::stringc& type, Delegate::IDelegate* dg){
		//mRegisterFactoryItems[_category][_type] = _delegate;
		GeneratorId id(category,type);
		GeneratorMap::Node* node=m_generators.find(id);
		if(dg!=NULL)
		{
			YON_WARN(YON_LOG_WARN_FORMAT,"registerFactory failed for already exist a generator with category:%s,type:%s\r\n",category.c_str(),type.c_str());
			return;
		}
		m_generators.set(id,dg);
	}

	void FactoryManager::unregisterFactory(const core::stringc& category, const core::stringc& type){
		GeneratorId id(category,type);
		Delegate::IDelegate* dg=NULL;
		m_generators.remove(id,&dg);
		if(dg==NULL)
			YON_WARN(YON_LOG_WARN_FORMAT,"unregisterFactory failed for not found generator with category:%s,type:%s\r\n",category.c_str(),type.c_str());
		
	}

	void FactoryManager::unregisterFactory(const core::stringc& category){
		//TODO use Iterator
	}


	bool FactoryManager::existFactory(const core::stringc& category, const core::stringc& type){
		GeneratorId id(category,type);
		return m_generators.find(id)!=NULL;
	}


	IObject* FactoryManager::createObject(const core::stringc& category, const core::stringc& type){
		GeneratorId id(category,type);
		GeneratorMap::Node* node=m_generators.find(id);
		if(node==NULL)
		{
			YON_WARN(YON_LOG_WARN_FORMAT,"createObject failed for not found generator with category:%s,type:%s\r\n",category.c_str(),type.c_str());
			return NULL;
		}
		IObject* result = NULL;
		Delegate::IDelegate* dg=node->getValue();
		dg(result);
		return result;
	}

	void FactoryManager::destroyObject(IObject* object){
		delete object;
	}
}