#ifndef _MUI_SKINMANAGER_H_
#define _MUI_SKINMANAGER_H_

namespace mui{

	class SkinManager{
	private:
		core::stringc m_defaultName;
		bool m_bInitialized;

		void createDefault(const core::stringc& value);
		//void _load(xml::ElementPtr _node, const std::string& _file, Version _version);
		void load(xml::ElementPtr node, const core::stringc& file);
	public:
		SkinManager();

		void initialize();
		void shutdown();

		/** 
		* @brief Get ResourceSkin by name 
		*/
		//ResourceSkin* getByName(const std::string& _name) const;
		ResourceSkin* getSkin(const core::stringc& name) const;

		/** 
		* @brief Get default skin name.
		*
		* Default skin used when creating widget with skin that doesn't exist.
		*/
		const core::stringc& getDefaultSkin() const;

		/** 
		* @brief Set default skin name.
		*
		* Default skin used when creating widget with skin that doesn't exist.
		*/
		void setDefaultSkin(const core::stringc& value);
	};
}
#endif