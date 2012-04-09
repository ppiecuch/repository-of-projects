#ifndef _YON_IO_CREADSTREAM_H_
#define _YON_IO_CREADSTREAM_H_

#include "IStream.h"

namespace yon{
namespace io{

	class CReadStream : public IReadStream{
	protected:
		virtual readData(void* data,u32 num);
	public:
		virtual bool seek(long offset, bool relative = false);
		virtual long getPos() const;
		virtual s32 read(void* buffer, u32 sizeToRead);
	};
}
}
#endif