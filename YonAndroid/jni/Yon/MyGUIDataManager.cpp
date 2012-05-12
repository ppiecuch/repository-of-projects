
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

namespace yon{
namespace gui{
namespace mygui{

	
	MyGUIDataManager::MyGUIDataManager(io::IFileSystem* fs)
		:m_bIsInitialise(false),m_pFileSystem(fs){

	}

	void MyGUIDataManager::initialise(){
		/*
		MYGUI_PLATFORM_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
		*/
		m_bIsInitialise=true;
		Logger->info(YON_LOG_SUCCEED_FORMAT,"initialize MyGUIDataManager");
	}
	void MyGUIDataManager::shutdown(){
		/*
		MYGUI_PLATFORM_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
		*/
		m_bIsInitialise=false;
		Logger->info(YON_LOG_SUCCEED_FORMAT,"shutdown MyGUIDataManager");
	}

	MyGUI::IDataStream* MyGUIDataManager::getData(const std::string& _name){
		/*
		std::string filepath = getDataPath(_name);
		if (filepath.empty())
			return nullptr;

		std::ifstream* stream = new std::ifstream();
		stream->open(filepath.c_str(), std::ios_base::binary);

		if (!stream->is_open())
		{
			delete stream;
			return nullptr;
		}

		DataFileStream* data = new DataFileStream(stream);

		return data;
		*/

		io::path fullpath=m_pFileSystem->getAbsolutePath(io::path(_name.c_str()));

		Logger->debug("start getData:%s\n",_name.c_str());

		std::ifstream* stream = new std::ifstream();
		stream->open(fullpath.c_str(), std::ios_base::binary);

		if (!stream->is_open())
		{
			delete stream;
			Logger->info(YON_LOG_FAILED_FORMAT,core::stringc("open %s",fullpath.c_str()));
			return nullptr;
		}

		MyGUI::DataFileStream* data = new MyGUI::DataFileStream(stream);

		Logger->debug("end getData:%s\n",_name.c_str());

		return data;
	}

	bool MyGUIDataManager::isDataExist(const std::string& _name){
		/*const MyGUI::VectorString& files = getDataListNames(_name);
		if(files.size()==0)
			Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("%s not exist",_name.c_str()));
		return files.size() == 1;*/
		return true;
	}

	const MyGUI::VectorString& MyGUIDataManager::getDataListNames(const std::string& _pattern){
		/*
		static VectorString result;
		common::VectorWString wresult;
		result.clear();

		for (VectorArhivInfo::const_iterator item = mPaths.begin(); item != mPaths.end(); ++item)
		{
			common::scanFolder(wresult, (*item).name, (*item).recursive, MyGUI::UString(_pattern).asWStr(), false);
		}

		for (common::VectorWString::const_iterator item = wresult.begin(); item != wresult.end(); ++item)
		{
			result.push_back(MyGUI::UString(*item).asUTF8());
		}

		return result;
		*/
		static MyGUI::VectorString result;
		/*VectorWString wresult;
		result.clear();

		for (VectorArhivInfo::const_iterator item = m_paths.begin(); item != m_paths.end(); ++item)
		{
			scanFolder(wresult, (*item).name, (*item).recursive, MyGUI::UString(_pattern).asWStr(), false);
		}

		for (VectorWString::const_iterator item = wresult.begin(); item != wresult.end(); ++item)
		{
			result.push_back(MyGUI::UString(*item).asUTF8());
		}*/

		return result;
	}

	const std::string& MyGUIDataManager::getDataPath(const std::string& _name){
		/*
		static std::string path;
		VectorString result;
		common::VectorWString wresult;

		for (VectorArhivInfo::const_iterator item = mPaths.begin(); item != mPaths.end(); ++item)
		{
			common::scanFolder(wresult, (*item).name, (*item).recursive, MyGUI::UString(_name).asWStr(), true);
		}

		for (common::VectorWString::const_iterator item = wresult.begin(); item != wresult.end(); ++item)
		{
			result.push_back(MyGUI::UString(*item).asUTF8());
		}

		path = result.size() == 1 ? result[0] : "";
		return path;
		*/
		static std::string path;
		/*MyGUI::VectorString result;
		VectorWString wresult;

		for (VectorArhivInfo::const_iterator item = m_paths.begin(); item != m_paths.end(); ++item)
		{
			scanFolder(wresult, (*item).name, (*item).recursive, MyGUI::UString(_name).asWStr(), true);
		}

		for (VectorWString::const_iterator item = wresult.begin(); item != wresult.end(); ++item)
		{
			result.push_back(MyGUI::UString(*item).asUTF8());
		}

		path = result.size() == 1 ? result[0] : "";*/
		return path;
	}

	void MyGUIDataManager::addResourceLocation(const std::string& _name, bool _recursive){
		/*ArhivInfo info;
		info.name = MyGUI::UString(_name).asWStr();
		info.recursive = _recursive;
		m_paths.push_back(info);*/
	}
}
}
}