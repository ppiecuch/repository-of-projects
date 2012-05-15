#include "MyGUIDataManager.h"
#include "MyGUI_DataFileStream.h"

#include <MyGUI.h>
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
#include <windows.h>
#include <io.h>
#else
#include <unistd.h>
#include <dirent.h>
#endif

#include <string>
#include <vector>

#include "ILogger.h"

namespace MyGUI{

	MyGUIDataManager::MyGUIDataManager(const io::IFileSystem* fs)
		:m_pFileSystem(fs){
	}

	MyGUI::IDataStream* MyGUIDataManager::getData(const std::string& _name){
		io::path fullpath=m_pFileSystem->getAbsolutePath(io::path(_name.c_str()));

		Logger->debug("start getData:%s\n",_name.c_str());

		std::ifstream* stream = new std::ifstream();
		stream->open(fullpath.c_str(), std::ios_base::binary);

		if (!stream->is_open())
		{
			delete stream;
			Logger->info(YON_LOG_FAILED_FORMAT,core::stringc("open %s",fullpath.c_str()).c_str());
			return nullptr;
		}

		MyGUI::DataFileStream* data = new MyGUI::DataFileStream(stream);

		Logger->debug("end getData:%s\n",_name.c_str());

		return data;
	}

	bool MyGUIDataManager::isDataExist(const std::string& _name){
		return true;
	}

	const MyGUI::VectorString& MyGUIDataManager::getDataListNames(const std::string& _pattern){
		static MyGUI::VectorString result;
		return result;
	}

	const std::string& MyGUIDataManager::getDataPath(const std::string& _name){
		static std::string path;
		return path;
	}
}