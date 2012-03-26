#include "CFileSystem.h"
#include "CReadFile.h"

#ifdef YON_COMPILE_WITH_WIN32
#include <io.h> // for _access
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
		return createAndOpenFile(filename);
	}
	io::path CFileSystem::getAbsolutePath(const io::path& filename) const{
		return filename;
	}

	IFileSystem* createFileSystem()
	{
		return new CFileSystem();
	}
}//io
}//yon