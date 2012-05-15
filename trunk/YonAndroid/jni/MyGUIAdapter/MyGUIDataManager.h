#ifndef _MYGUI_MYGUIDATAMANAGER_H_
#define _MYGUI_MYGUIDATAMANAGER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_DataManager.h"
#include "IFileSystem.h"

using namespace yon;

namespace MyGUI{

	class MyGUIDataManager : public DataManager{
	protected:

		const io::IFileSystem* m_pFileSystem;

	public:
		MyGUIDataManager(const io::IFileSystem* fs);

		/** @see DataManager::getData(const std::string& _name) */
		virtual IDataStream* getData(const std::string& _name);

		/** @see DataManager::isDataExist(const std::string& _name) */
		virtual bool isDataExist(const std::string& _name);

		/** @see DataManager::getDataListNames(const std::string& _pattern) */
		virtual const VectorString& getDataListNames(const std::string& _pattern);

		/** @see DataManager::getDataPath(const std::string& _name) */
		virtual const std::string& getDataPath(const std::string& _name);

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
#endif