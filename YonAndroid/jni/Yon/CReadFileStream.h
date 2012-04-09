#ifndef _YON_IO_CREADFILESTREAM_H_
#define _YON_IO_CREADFILESTREAM_H_

#include <stdio.h>
#include "IStream.h"

namespace yon{
namespace io{

	class CReadFileStream : public IReadStream{
	protected:
		FILE* m_pFile;
		u32 m_fileSize;
		void openFile();
		virtual void readDataInEndianMode(void* data,u32 sizeToRead);
	public:
		CReadFileStream(const io::path& name,ENUM_ENDIAN_MODE mode=ENUM_ENDIAN_MODE_LITTLE);
		virtual ~CReadFileStream();
		virtual bool seek(long offset, bool relative);
		virtual u32 getPos() const;
		virtual s32 read(u8* buffer, u32 sizeToRead);
		virtual u32 getSize() const{return m_fileSize;}
		virtual void* pointer() const{return m_pFile;}
	};
}
}
#endif