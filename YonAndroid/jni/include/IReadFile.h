#ifndef _YON_IO_IREADFILE_H_
#define _YON_IO_IREADFILE_H_

#include "IReferencable.h"
#include "path.h"

namespace yon{
namespace io{

	class IReadFile : public virtual core::IReferencable{
	public:
		//返回读取的字节数
		virtual s32 read(void* buffer, u32 sizeToRead) = 0;

		//offset:偏移量
		//relative:相对于当前位置
		virtual bool seek(long offset, bool relative = false) = 0;

		virtual FILE* pointer() const = 0;

		//获取文件大小，以字节为单位
		virtual long getSize() const = 0;

		//当前位置，以字节为单位
		virtual long getPos() const = 0;

		//文件名
		virtual const io::path& getFileName() const = 0;
		//路径名
		virtual const io::path& getPathName() const = 0;
	};

	IReadFile* createReadFile(const io::path& fileName);
}//io
}//yon
#endif