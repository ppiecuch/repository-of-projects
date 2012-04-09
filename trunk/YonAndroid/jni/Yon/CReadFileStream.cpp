#include "CReadFileStream.h"

#include "ILogger.h"

namespace yon{
namespace io{

	
	CReadFileStream::CReadFileStream(const io::path& name,ENUM_ENDIAN_MODE mode)
		:IReadStream(name,ENUM_STREAM_TYPE_FILE,mode),m_pFile(NULL),m_fileSize(0){
			openFile();
			//Logger->debug(YON_LOG_SUCCEED_FORMAT,"Instance CReadFileStream");
	}

	CReadFileStream::~CReadFileStream(){
		if (m_pFile)
			fclose(m_pFile);
		//Logger->debug(YON_LOG_SUCCEED_FORMAT,"Release CReadFile");
	}

	void CReadFileStream::openFile()
	{
		if (m_name.length() == 0)
		{
			m_pFile = NULL;
			return;
		}
#ifdef YON_COMPILE_WITH_WIN32
#if defined(YON_WCHAR_FILESYSTEM)
		m_pFile = _wfopen(m_name.c_str(), L"rb");
#else
		errno_t result=fopen_s(&m_pFile,m_name.c_str(),"rb");
#endif
#elif defined(YON_COMPILE_WITH_ANDROID)
		m_pFile = fopen(m_name.c_str(),"rb");
#endif
		if (m_pFile)
		{
			fseek(m_pFile, 0, SEEK_END);
			m_fileSize = getPos();
			fseek(m_pFile, 0, SEEK_SET);
		}
	}

	void CReadFileStream::readDataInEndianMode(void* data,u32 sizeToRead){
		if (m_pFile==NULL)
			return;
		if(m_endianMode==ENUM_ENDIAN_MODE_LITTLE){
			fread(data, 1, sizeToRead, m_pFile);
		}else{
			for(u32 i=sizeToRead-1;i>=0;--i){
				fread(&((u8*)data)[i], 1, 1, m_pFile);
			}
		}
	}

	s32 CReadFileStream::read(u8* buffer,u32 sizeToRead){
		if (m_pFile==NULL)
			return 0;

		return (s32)fread(buffer, 1, sizeToRead, m_pFile);
	}
	bool CReadFileStream::seek(long offset, bool relative = false){
		if (m_pFile==NULL)
			return false;

		return fseek(m_pFile, offset, relative ? SEEK_CUR : SEEK_SET) == 0;
	}
	u32 CReadFileStream::getPos() const{
		return ftell(m_pFile);
	}
}
}