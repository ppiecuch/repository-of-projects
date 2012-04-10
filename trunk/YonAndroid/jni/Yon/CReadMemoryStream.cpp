#include "CReadMemoryStream.h"

namespace yon{
namespace io{

	
	CReadMemoryStream::CReadMemoryStream(const io::path& name,void* data, long size,bool deleteMemoryWhenDropped,ENUM_ENDIAN_MODE mode)
		:IReadStream(name,ENUM_STREAM_TYPE_MEMORY,mode),m_pData((u8*)data),m_uPos(0),m_uDataSize(size),m_bDeleteWhenDropped(deleteMemoryWhenDropped){}

	CReadMemoryStream::~CReadMemoryStream(){
		if(m_bDeleteWhenDropped)
			delete[] m_pData;
	}
	void CReadMemoryStream::readDataInEndianMode(void* data,u32 sizeToRead){
		if(m_endianMode==ENUM_ENDIAN_MODE_LITTLE){
			memcpy(data, m_pData, sizeToRead);
		}else{
			for(u32 i=0;i<sizeToRead;++i){
				((u8*)data)[sizeToRead-1-i]=m_pData[m_uPos+i];
			}
		}
		m_uPos+=sizeToRead;
	}
	bool CReadMemoryStream::seek(long offset, bool relative){
		m_uPos=relative?m_uPos+offset:offset;
		return true;
	}
	s32 CReadMemoryStream::read(u8* buffer,u32 sizeToRead){
		memcpy(buffer, m_pData, sizeToRead);
		m_uPos+=sizeToRead;
		return sizeToRead;
	}

	IReadStream* createReadMemoryStream(const io::path& name,void* memory, long size,bool deleteMemoryWhenDropped,ENUM_ENDIAN_MODE mode){
		return new CReadMemoryStream(name,memory,size,deleteMemoryWhenDropped,mode);
	}
}
}