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
			virtual io::path getAbsolutePath(const io::path& filename) const = 0;
			virtual const io::path& getWorkingDirectory() = 0;
		};

		IFileSystem* createFileSystem();
	}//io
}//yon
#endif