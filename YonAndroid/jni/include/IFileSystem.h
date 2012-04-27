#ifndef _YON_IO_IFILESYSTEM_H_
#define _YON_IO_IFILESYSTEM_H_

#include "IReferencable.h"
#include "IReadFile.h"
#include "IStream.h"

namespace yon{
	namespace io{

		class IFileSystem : public virtual core::IReferencable{
		public:
			virtual ~IFileSystem(){}
			virtual bool existFile(const path& filename) const =0;
			//使用完后记得drop
			virtual YON_DEPRECATED IReadFile* createAndOpenFile(const io::path& filename) = 0;
			virtual IReadStream* createAndOpenReadFileStream(const io::path& filename,ENUM_ENDIAN_MODE mode=ENUM_ENDIAN_MODE_LITTLE) = 0;
			//获取绝对路径
			//参数inWorkingDirectory，是否按工作空间路径获取
			virtual io::path getAbsolutePath(const io::path& filename,bool inWorkingDirectory=true) const = 0;

			//设置当前工作空间路径
			//参数 newDirectory: 新工作空间的路径(前面没有斜杠，后面带斜杠)
			//格式如下： "<drive>:/<directory>/<sudirectory>/<..>". 如 "C:/Windows/","/sdcard/","../media"
			virtual void setWorkingDirectory(const io::path& newDirectory) =0;
			virtual const io::path& getWorkingDirectory() = 0;
		};

		IFileSystem* createFileSystem();
	}//io
}//yon
#endif