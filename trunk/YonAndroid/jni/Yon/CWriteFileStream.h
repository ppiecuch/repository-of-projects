#ifndef _YON_IO_CWRITEFILESTREAM_H_
#define _YON_IO_CWRITEFILESTREAM_H_

#include <stdio.h>
#include "IStream.h"

namespace yon{
namespace io{

	class CWriteFileStream : public IWriteStream{
	protected:
		FILE* m_pFile;
		void openFile(bool append);
		virtual void writeDataInEndianMode(void* data,u32 sizeToWrite);
	public:
		CWriteFileStream(const io::path& name, bool append=false, ENUM_ENDIAN_MODE mode=ENUM_ENDIAN_MODE_LITTLE);
		virtual ~CWriteFileStream();
		virtual bool seek(long offset, bool relative);
		virtual u32 getPos() const;
		virtual s32 write(u8* buffer, u32 sizeToWrite);
		virtual void* pointer() const{return m_pFile;}
	};

	IWriteStream* createWriteFileStream(const io::path& name,bool append, ENUM_ENDIAN_MODE mode);
}
}
#endif