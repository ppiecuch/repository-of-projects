#ifndef _YON_GUI_MYGUI_MYGUIDATAMANAGER_H_
#define _YON_GUI_MYGUI_MYGUIDATAMANAGER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_DataManager.h"
#include "IFileSystem.h"

namespace yon{
namespace gui{
namespace mygui{

	class MyGUIDataManager : public MyGUI::DataManager{
	protected:
		struct ArhivInfo
		{
			std::wstring name;
			bool recursive;
		};
		typedef std::vector<ArhivInfo> VectorArhivInfo;
		VectorArhivInfo m_paths;
		bool m_bIsInitialise;

		io::IFileSystem* m_pFileSystem;

	public:
		MyGUIDataManager(io::IFileSystem* fs);

		void initialise();
		void shutdown();

		/** @see DataManager::getData(const std::string& _name) */
		virtual MyGUI::IDataStream* getData(const std::string& _name);

		/** @see DataManager::isDataExist(const std::string& _name) */
		virtual bool isDataExist(const std::string& _name);

		/** @see DataManager::getDataListNames(const std::string& _pattern) */
		virtual const MyGUI::VectorString& getDataListNames(const std::string& _pattern);

		/** @see DataManager::getDataPath(const std::string& _name) */
		virtual const std::string& getDataPath(const std::string& _name);


		virtual void addResourceLocation(const std::string& _name, bool _recursive);

		static MyGUIDataManager& getInstance()
		{
			return *getInstancePtr();
		}
		static MyGUIDataManager* getInstancePtr()
		{
			return static_cast<MyGUIDataManager*>(MyGUI::DataManager::getInstancePtr());
		}
	};
}
}
}
#endif