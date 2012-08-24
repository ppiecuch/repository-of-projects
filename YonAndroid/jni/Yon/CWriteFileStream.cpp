#include "CWriteFileStream.h"
#include "yonUtil.h"
#include "ILogger.h"

namespace yon{
namespace io{


	CWriteFileStream::CWriteFileStream(const io::path& name,bool append, ENUM_ENDIAN_MODE mode)
		:IWriteStream(name,ENUM_STREAM_TYPE_FILE,mode),m_pFile(NULL){
			openFile(append);
			Logger->debug(YON_LOG_SUCCEED_FORMAT,"Instance CWriteFileStream");
	}

	CWriteFileStream::~CWriteFileStream(){
		if (m_pFile)
			fclose(m_pFile);
		Logger->debug(YON_LOG_SUCCEED_FORMAT,"Release CWriteFileStream");
	}

	void CWriteFileStream::openFile(bool append)
	{
		if (m_path.length() == 0)
		{
			m_pFile = NULL;
			Logger->warn(YON_LOG_WARN_FORMAT,"path is empty,ignore!");
			return;
		}

		core::mkdirs(core::getParentName(m_path));

#ifdef YON_COMPILE_WITH_WIN32
#if defined(YON_WCHAR_FILESYSTEM)
		m_pFile = _wfopen(m_path.c_str(), append ? L"ab" : L"wb");
#else
		errno_t result=fopen_s(&m_pFile,m_path.c_str(),append ? "ab" : "wb");
#endif
#elif defined(YON_COMPILE_WITH_ANDROID)
		m_pFile = fopen(m_path.c_str(),append ? "ab" : "wb");
#endif
		if (m_pFile==NULL)
		{
			Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("file:%s open failed!",m_path.c_str()).c_str());
		}
	}

	void CWriteFileStream::writeDataInEndianMode(void* data,u32 sizeToWrite){
		if (m_pFile==NULL){
			Logger->warn(YON_LOG_WARN_FORMAT,"No file is opened!");
			return ;
		}
		if(m_endianMode==ENUM_ENDIAN_MODE_LITTLE){
			fwrite(data, 1, sizeToWrite, m_pFile);
		}else{
			/*for(u32 i=sizeToWrite-1;i>=0;--i){
				fwrite(&((u8*)data)[i], 1, 1, m_pFile);
			}*/
			u8 temp[8];
			for(u32 i=0;i<sizeToWrite;++i){
				temp[sizeToWrite-1-i]=((u8*)data)[i];
			}
			fwrite(temp, 1, sizeToWrite, m_pFile);
		}
	}

	s32 CWriteFileStream::write(const u8* buffer,u32 sizeToWrite){
		if (m_pFile==NULL){
			Logger->warn(YON_LOG_WARN_FORMAT,"No file is opened!");
			return 0;
		}

		return (s32)fwrite(buffer, 1, sizeToWrite, m_pFile);
	}
	bool CWriteFileStream::seek(long offset, bool relative = false){
		if (m_pFile==NULL)
			return false;

		return fseek(m_pFile, offset, relative ? SEEK_CUR : SEEK_SET) == 0;
	}
	u32 CWriteFileStream::getPos() const{
		return ftell(m_pFile);
	}

	IWriteStream* createWriteFileStream(const io::path& name,bool append, ENUM_ENDIAN_MODE mode){
		return new CWriteFileStream(name,append,mode);
	}
}
}