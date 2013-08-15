#include "MUI_SkinManager.h"

namespace mui{


	SkinManager::SkinManager()
		:m_bInitialized(false){
	}

	void SkinManager::initialize(){
		/*
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		ResourceManager::getInstance().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &SkinManager::_load);
		FactoryManager::getInstance().registerFactory<ResourceSkin>(XML_TYPE_RESOURCE);

		mDefaultName = "skin_Default";
		createDefault(mDefaultName);

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
		*/

		YON_DEBUG_BREAK_IF(m_bInitialized);

		m_bInitialized=true;
	}

	void SkinManager::shutdown(){
		/*
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		ResourceManager::getInstance().unregisterLoadXmlDelegate(XML_TYPE);
		FactoryManager::getInstance().unregisterFactory<ResourceSkin>(XML_TYPE_RESOURCE);

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
		*/

		YON_DEBUG_BREAK_IF(!m_bInitialized);

		m_bInitialized=false;
	}

	void SkinManager::createDefault(const core::stringc& value){
		/*
		xml::Document doc;
		xml::ElementPtr root = doc.createRoot("MyGUI");
		xml::ElementPtr newnode = root->createChild("Resource");
		newnode->addAttribute("type", ResourceSkin::getClassTypeName());
		newnode->addAttribute("name", _value);

		ResourceManager::getInstance().loadFromXmlNode(root, "", Version());
		*/
	}

	void SkinManager::load(xml::ElementPtr node, const core::stringc& file){
#if 0
		// §Ò§Ö§â§Ö§Þ §Õ§Ö§ä§Ö§Û §Ú §Ü§â§å§ä§Ú§Þ§ã§ñ, §à§ã§ß§à§Ó§ß§à§Û §è§Ú§Ü§Ý §ã§à §ã§Ü§Ú§ß§Ñ§Þ§Ú
		xml::ElementEnumerator skin = _node->getElementEnumerator();
		while (skin.next(XML_TYPE))
		{
			/*std::string name = */skin->findAttribute("name");
			std::string type = skin->findAttribute("type");
			if (type.empty())
				type = "ResourceSkin";

			IObject* object = FactoryManager::getInstance().createObject(XML_TYPE_RESOURCE, type);
			if (object != nullptr)
			{
				ResourceSkin* data = object->castType<ResourceSkin>();
				data->deserialization(skin.current(), _version);

				ResourceManager::getInstance().addResource(data);
			}
		}
#endif
	}

	ResourceSkin* SkinManager::getSkin(const core::stringc& name) const{
		/*
		std::string skinName = BackwardCompatibility::getSkinRename(_name);
		IResource* result = nullptr;
		if (!skinName.empty() && skinName != RESOURCE_DEFAULT_NAME)
			result = ResourceManager::getInstance().getByName(skinName, false);

		if (result == nullptr)
		{
			result = ResourceManager::getInstance().getByName(mDefaultName, false);
			if (!skinName.empty() && skinName != RESOURCE_DEFAULT_NAME)
			{
				MYGUI_LOG(Error, "Skin '" << skinName << "' not found. Replaced with default skin." << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			}
		}

		return result ? result->castType<ResourceSkin>(false) : nullptr;
		*/
	}

	const core::stringc& SkinManager::getDefaultSkin() const{
		return m_defaultName;
	}

	void SkinManager::setDefaultSkin(const core::stringc& value){
		m_defaultName = value;
	}
}