#ifndef _MUI_SKINMANAGER_H_
#define _MUI_SKINMANAGER_H_

namespace mui{

	class SkinManager{
	private:
		core::stringc m_defaultName;
		bool m_bIsInitialise;

		void createDefault(const core::stringc& value);
		//void _load(xml::ElementPtr _node, const std::string& _file, Version _version);
		void load(xml::ElementPtr _node, const core::stringc& file);
	public:
		SkinManager();

		void initialise();
		void shutdown();

		/** Get ResourceSkin by name */
		//TODO rename getSkin
		ResourceSkin* getByName(const core::stringc& _name) const;

		/** Get default skin name.
		Default skin used when creating widget with skin that doesn't exist.
		*/
		const core::stringc& getDefaultSkin() const;
	};
}
#endif