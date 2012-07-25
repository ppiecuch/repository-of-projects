#ifndef _YON_IO_IFILESYSTEM_H_
#define _YON_IO_IFILESYSTEM_H_

#include "IReferencable.h"
#include "IStream.h"
#include "yonXML.h"

namespace yon{
namespace io{

	class IFileSystem : public virtual core::IReferencable{
	public:
		virtual ~IFileSystem(){}
		virtual bool existFile(const path& filename) const =0;
		//使用完后记得drop
		virtual IReadStream* createAndOpenReadFileStream(const io::path& filename,ENUM_ENDIAN_MODE mode=ENUM_ENDIAN_MODE_LITTLE) = 0;
		virtual IReadStream* createAndOpenReadMemoryStream(const io::path& name,void* data, long size,bool deleteMemoryWhenDropped,ENUM_ENDIAN_MODE mode=ENUM_ENDIAN_MODE_LITTLE) = 0;
		//打开文件，如果路径不存在，将递归创建父目录
		virtual IWriteStream* createAndOpenWriteFileStream(const path& filename, bool append=false, ENUM_ENDIAN_MODE mode=ENUM_ENDIAN_MODE_LITTLE) =0;

		virtual XMLReader* createXMLReader(IReadStream* stream) =0;

		//获取绝对路径
		//参数inWorkingDirectory，是否按工作空间路径获取
		//virtual YON_DEPRECATED io::path getAbsolutePath(const io::path& filename,bool inWorkingDirectory=true) const = 0;

		//设置当前工作空间路径
		//参数 newDirectory: 新工作空间的路径(前面没有斜杠，后面带斜杠)
		//格式如下： "<drive>:/<directory>/<sudirectory>/<..>". 如 "C:/Windows/","/sdcard/","../media/"
		//virtual YON_DEPRECATED void setWorkingDirectory(const io::path& newDirectory) =0;
		//virtual YON_DEPRECATED const io::path& getWorkingDirectory() = 0;

		//支持多个工作目录
		//参数 newDirectory: 新工作空间的路径(前面没有斜杠，后面带斜杠)
		//格式如下： "<drive>:/<directory>/<sudirectory>/<..>". 如 "C:/Windows/","/sdcard/","../media/"
		virtual void addWorkingDirectory(const io::path& newDirectory) = 0;
		virtual s32 getWorkingDirectoryCount(){return 0;}
		virtual const io::path& getWorkingDirectory(s32 index) = 0;
		//获取绝对路径
		virtual io::path getAbsolutePath(const io::path& filename) const = 0;
		//获取资源绝对路径
		//参数noEmpty，如果为true,指示如果不存在匹配资源，则返回使用第一个资源目录拼接的路径（用于createStream），否则返回""
		//遍历工作目录，如果存在匹配的资源，返回路径，否则根据noEmpty指示返回
		virtual io::path getResourcePath(const io::path& filename,bool noEmpty=false) const = 0;
	};

	IFileSystem* createFileSystem();
}
}
#endif