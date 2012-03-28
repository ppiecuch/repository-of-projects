#include "CReadFile.h"

#include "ILogger.h"

namespace yon{
namespace io{

	CReadFile::CReadFile(const io::path& fileName)
		: m_pFile(NULL), m_fileSize(0), m_filename(fileName)
	{
		openFile();
		//Logger->debug(YON_LOG_SUCCEED_FORMAT,"Instance CReadFile");
	}

	CReadFile::~CReadFile()
	{
		if (m_pFile)
			fclose(m_pFile);
		//Logger->debug(YON_LOG_SUCCEED_FORMAT,"Release CReadFile");
	}

	void CReadFile::openFile()
	{
		if (m_filename.length() == 0)
		{
			m_pFile = NULL;
			return;
		}
#ifdef YON_COMPILE_WITH_WIN32
#if defined(YON_WCHAR_FILESYSTEM)
		m_pFile = _wfopen(m_filename.c_str(), L"rb");
#else
		errno_t result=fopen_s(&m_pFile,m_filename.c_str(),"rb");
#endif
#elif defined(YON_COMPILE_WITH_ANDROID)
		m_pFile = fopen(m_filename.c_str(),"rb");
#endif
		if (m_pFile)
		{
			fseek(m_pFile, 0, SEEK_END);
			m_fileSize = getPos();
			fseek(m_pFile, 0, SEEK_SET);
		}
	}

	s32 CReadFile::read(void* buffer, u32 sizeToRead){
		if (!isOpen())
			return 0;

		return (s32)fread(buffer, 1, sizeToRead, m_pFile);
	}

	bool CReadFile::seek(long offset, bool relative){
		if (!isOpen())
			return false;

		return fseek(m_pFile, offset, relative ? SEEK_CUR : SEEK_SET) == 0;
	}

	long CReadFile::getPos() const
	{
		return ftell(m_pFile);
	}

	IReadFile* createReadFile(const io::path& fileName)
	{
		CReadFile* file = new CReadFile(fileName);
		if (file->isOpen()){
			return file;
		}else{
			file->drop();
			return 0;
		}
	}
}//io
}//yon