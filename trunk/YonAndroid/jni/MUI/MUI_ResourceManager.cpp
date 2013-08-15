#include "MUI_ResourceManager.h"

namespace mui{

	ResourceManager::ResourceManager()
		:m_bInitialized(false){
	}

	void ResourceManager::initialize(){
		/*
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &ResourceManager::loadFromXmlNode);
		registerLoadXmlDelegate(XML_TYPE_LIST) = newDelegate(this, &ResourceManager::_loadList);

		// ��֧ԧ֧���ڧ��֧� �է֧��ݧ�ߧ�� ��֧�����
		FactoryManager::getInstance().registerFactory<ResourceImageSet>(XML_TYPE);

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
		*/
		YON_DEBUG_BREAK_IF(m_bInitialized);
		m_bInitialized=true;
	}

	void ResourceManager::shutdown(){
		/*
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		FactoryManager::getInstance().unregisterFactory<ResourceImageSet>(XML_TYPE);

		clear();
		unregisterLoadXmlDelegate(XML_TYPE);
		unregisterLoadXmlDelegate(XML_TYPE_LIST);

		mMapLoadXmlDelegate.clear();

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
		*/
		YON_DEBUG_BREAK_IF(!m_bInitialized);
		m_bInitialized=false;
	}

	bool ResourceManager::load(const core::stringc& file){

	}

	void ResourceManager::loadFromXmlNode(xml::ElementPtr node, const core::stringc& file){
		/*
		FactoryManager& factory = FactoryManager::getInstance();

		// �ҧ֧�֧� �է֧�֧� �� �ܧ���ڧާ��, ���ߧ�ӧߧ�� ��ڧܧ�
		xml::ElementEnumerator root = _node->getElementEnumerator();
		while (root.next(XML_TYPE))
		{
			// ��ѧ��ڧ� �ѧ��ڧҧ���
			std::string type, name;
			root->findAttribute("type", type);
			root->findAttribute("name", name);

			if (name.empty())
				continue;

			MapResource::iterator item = mResources.find(name);
			if (item != mResources.end())
			{
				MYGUI_LOG(Warning, "dublicate resource name '" << name << "'");

				// ��֧����ѧާ� �ާ�ԧ�� ���ݧ�٧�ӧѧ����
				mRemovedResoures.push_back((*item).second);
				mResources.erase(item);
			}

			IObject* object = factory.createObject(XML_TYPE, type);
			if (object == nullptr)
			{
				MYGUI_LOG(Error, "resource type '" << type << "' not found");
				continue;
			}

			IResourcePtr resource = object->castType<IResource>();
			resource->deserialization(root.current(), _version);

			mResources[name] = resource;
		}
		*/
	}

	void ResourceManager::addResource(IResource* item){

	}

	void ResourceManager::removeResource(IResource* item){

	}

	ResourceManager::LoadXmlDelegate& ResourceManager::registerLoadXmlDelegate(const core::stringc& key){

	}

	void ResourceManager::unregisterLoadXmlDelegate(const core::stringc& key){

	}

	bool ResourceManager::exist(const core::stringc& name) const{

	}

	IResource* ResourceManager::getResource(const core::stringc& name) const{

	}

	bool ResourceManager::remove(const core::stringc& name){

	}

	void ResourceManager::clear(){

	}
}