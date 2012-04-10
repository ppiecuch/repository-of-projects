#ifndef _YON_IO_CREADMEMORYSTREAM_H_
#define _YON_IO_CREADMEMORYSTREAM_H_

#include <memory.h>
#include "IStream.h"

namespace yon{
namespace io{

	class CReadMemoryStream : public IReadStream{
	protected:
		u8* m_pData;
		u32 m_uDataSize;
		u32 m_uPos;
		bool m_bDeleteWhenDropped;
		virtual void readDataInEndianMode(void* data,u32 sizeToRead);
	public:
		CReadMemoryStream(const io::path& name,void* data, long size,bool deleteMemoryWhenDropped,ENUM_ENDIAN_MODE mode=ENUM_ENDIAN_MODE_LITTLE);
		virtual ~CReadMemoryStream();
		virtual bool seek(long offset, bool relative);
		virtual u32 getPos() const{return m_uPos;}
		virtual s32 read(u8* buffer, u32 sizeToRead);
		virtual u32 getSize() const{return m_uDataSize;}
		virtual void* pointer() const{return m_pData;}
	};

	IReadStream* createReadMemoryStream(const io::path& name,void* memory, long size,bool deleteMemoryWhenDropped,ENUM_ENDIAN_MODE mode);
}
}
#endif