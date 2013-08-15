#ifndef _MUI_RESOURCEMANAGER_H_
#define _MUI_RESOURCEMANAGER_H_

namespace mui{

	class ResourceManager{
	public:
		//typedef delegates::CDelegate3<xml::ElementPtr, const std::string&, Version> LoadXmlDelegate;
		typedef core::CDelegate3<xml::ElementPtr, const core::stringc&> LoadXmlDelegate;

		//typedef std::map<std::string, IResource*> MapResource;
		typedef core::map<core::stringc, IResource*> ResourceMap;
	private:
		//typedef std::map<std::string, LoadXmlDelegate> MapLoadXmlDelegate;
		//MapLoadXmlDelegate mMapLoadXmlDelegate;
		typedef core::map<core::stringc,LoadXmlDelegate> LoadXmlDelegateMap;
		LoadXmlDelegate m_loadXMLDelegates;

		//MapResource mResources;
		ResourceMap m_resources;

		//typedef std::vector<IResource*> VectorResource;
		//VectorResource mRemovedResoures;
		core::array<IResource*> m_removedResources;

		//bool mIsInitialise;
		bool m_bInitialized;
	public:
		ResourceManager();

		void initialize();
		void shutdown();

		/** 
		* @brief Load additional MUI *_resource.xml file 
		*/
		bool load(const core::stringc& file);

		//void loadFromXmlNode(xml::ElementPtr _node, const std::string& _file, Version _version);
		void loadFromXmlNode(xml::ElementPtr node, const core::stringc& file);

		/** Add resource item to resources */
		void addResource(IResource* item);

		/** Remove resource item from resources */
		void removeResource(IResource* item);

		/** Register delegate that parse XML node with specified tag (_key) */
		LoadXmlDelegate& registerLoadXmlDelegate(const core::stringc& key);

		/** Unregister delegate that parse XML node with specified tag (_key) */
		void unregisterLoadXmlDelegate(const core::stringc& key);

		/** Check is resource exist */
		bool exist(const core::stringc& name) const;

		/** Find resource by name*/
		//IResource* findByName(const std::string& _name) const;

		/** Get resource by name*/
		//IResource* getByName(const std::string& _name, bool _throw = true) const;
		IResource* getResource(const core::stringc& name) const;

		//bool removeByName(const std::string& _name);
		bool remove(const core::stringc& name);

		void clear();

		//size_t getCount() const;
		u32 size() const;
	};
}
#endif