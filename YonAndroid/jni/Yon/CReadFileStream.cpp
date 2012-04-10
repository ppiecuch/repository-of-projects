#include "CReadFileStream.h"

#include "ILogger.h"

namespace yon{
namespace io{

	
	CReadFileStream::CReadFileStream(const io::path& name,ENUM_ENDIAN_MODE mode)
		:IReadStream(name,ENUM_STREAM_TYPE_FILE,mode),m_pFile(NULL),m_fileSize(0){
			openFile();
			Logger->debug(YON_LOG_SUCCEED_FORMAT,"Instance CReadFileStream");
	}

	CReadFileStream::~CReadFileStream(){
		if (m_pFile)
			fclose(m_pFile);
		Logger->debug(YON_LOG_SUCCEED_FORMAT,"Release CReadFileStream");
	}

	void CReadFileStream::openFile()
	{
		if (m_path.length() == 0)
		{
			m_pFile = NULL;
			return;
		}
#ifdef YON_COMPILE_WITH_WIN32
#if defined(YON_WCHAR_FILESYSTEM)
		m_pFile = _wfopen(m_path.c_str(), L"rb");
#else
		errno_t result=fopen_s(&m_pFile,m_path.c_str(),"rb");
#endif
#elif defined(YON_COMPILE_WITH_ANDROID)
		m_pFile = fopen(m_path.c_str(),"rb");
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
			u8 temp[8];
			fread(temp, 1, sizeToRead, m_pFile);
			for(u32 i=0;i<sizeToRead;++i){
				((u8*)data)[sizeToRead-1-i]=temp[i];
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

	IReadStream* createReadFileStream(const io::path& name,ENUM_ENDIAN_MODE mode){
		return new CReadFileStream(name,mode);
	}
}
}