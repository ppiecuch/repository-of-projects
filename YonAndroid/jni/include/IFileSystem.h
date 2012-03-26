#ifndef _YON_IO_IFILESYSTEM_H_
#define _YON_IO_IFILESYSTEM_H_

#include "IReferencable.h"
#include "IReadFile.h"

namespace yon{
	namespace io{

		class IFileSystem : public virtual core::IReferencable{
		public:
			virtual ~IFileSystem(){}
			virtual bool existFile(const path& filename) const =0;
			//使用完后记得drop
			virtual IReadFile* createAndOpenFile(const io::path& filename) = 0;
			virtual io::path getAbsolutePath(const io::path& filename) const = 0;
		};
	}//io
}//yon
#endif