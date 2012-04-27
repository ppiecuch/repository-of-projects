#include "CFileSystem.h"
#include "CReadFile.h"
#include "CReadFileStream.h"

#ifdef YON_COMPILE_WITH_WIN32
#include <io.h> // for _access
#include <stdlib.h> //for _MAX_PATH
#include <direct.h> //for _getcwd
#define F_OK 00 //Existence only
#define W_OK 02 //Write-only
#define R_OK 04 //Read-only
#define X_OK 06 //Read and write
#elif defined(YON_COMPILE_WITH_ANDROID)
#include <unistd.h>
#endif

#include "ILogger.h"

namespace yon{
namespace io{

	CFileSystem::CFileSystem()
	{
		Logger->info(YON_LOG_SUCCEED_FORMAT,"Instance CFileSystem");
	}

	CFileSystem::~CFileSystem(){
		Logger->info(YON_LOG_SUCCEED_FORMAT,"Release CFileSystem");
	}

	bool CFileSystem::existFile(const path& filename) const{
#ifdef YON_COMPILE_WITH_WIN32
#if defined(YON_WCHAR_FILESYSTEM)
		return (_waccess(filename.c_str(), F_OK) != -1);
#else
		return (_access(filename.c_str(), F_OK) != -1);
#endif
#elif defined(YON_COMPILE_WITH_ANDROID)
		return (access(filename.c_str(), F_OK) != -1);
#endif
	}

	IReadFile* CFileSystem::createAndOpenFile(const io::path& filename){
		return createReadFile(getAbsolutePath(filename));
	}

	IReadStream* CFileSystem::createAndOpenReadFileStream(const io::path& filename,ENUM_ENDIAN_MODE mode){
		return createReadFileStream(getAbsolutePath(filename),mode);
	}


	io::path CFileSystem::getAbsolutePath(const io::path& filename,bool inWorkingDirectory) const{
		if(inWorkingDirectory){
#ifdef YON_COMPILE_WITH_WIN32
			core::stringc temp("%s%s",m_workingDirectory.c_str(),filename.c_str());
			fschar *p=0;
			fschar fpath[_MAX_PATH];
			p = _fullpath(fpath, temp.c_str(), _MAX_PATH);
			core::stringc tmp(p);
			tmp.replace('\\', '/');
			return tmp;
#elif defined(YON_COMPILE_WITH_ANDROID)
			core::stringc tmp("/sdcard/%s%s",m_workingDirectory.c_str(),filename.c_str());
			return tmp;
#endif
		}
#ifdef YON_COMPILE_WITH_WIN32
		fschar *p=0;
		fschar fpath[_MAX_PATH];
		p = _fullpath(fpath, filename.c_str(), _MAX_PATH);
		core::stringc tmp(p);
		tmp.replace('\\', '/');
		return tmp;
#elif defined(YON_COMPILE_WITH_ANDROID)
		core::stringc tmp("/sdcard/%s",filename.c_str());
		return tmp;
#endif
	}
	void CFileSystem::setWorkingDirectory(const io::path& newDirectory){
		m_workingDirectory=newDirectory;
		m_workingDirectory.replace('\\', '/');
	}
	const io::path& CFileSystem::getWorkingDirectory(){
		if(m_workingDirectory.length()==0){
#ifdef YON_COMPILE_WITH_WIN32
			fschar tmp[_MAX_PATH];
			_getcwd(tmp, _MAX_PATH);
			m_workingDirectory=tmp;
			m_workingDirectory.replace('\\', '/');
#elif defined(YON_COMPILE_WITH_ANDROID)
			u32 pathSize=256;
			char *tmpPath = new char[pathSize];
			while ((pathSize < (1<<16)) && !(getcwd(tmpPath,pathSize)))
			{
				delete [] tmpPath;
				pathSize *= 2;
				tmpPath = new char[pathSize];
			}
			if (tmpPath)
			{
				m_workingDirectory = tmpPath;
				delete [] tmpPath;
			}
#endif
			Logger->debug("workingDirection:%s\n",m_workingDirectory.c_str());
		}
		return m_workingDirectory;
	}


	IFileSystem* createFileSystem()
	{
		return new CFileSystem();
	}
}//io
}//yon