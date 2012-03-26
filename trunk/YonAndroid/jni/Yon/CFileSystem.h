#ifndef _YON_IO_CFILESYSTEM_H_
#define _YON_IO_CFILESYSTEM_H_

#include "IFileSystem.h"


namespace yon{
namespace io{

	class CFileSystem : public IFileSystem{
	public:
		CFileSystem();
		virtual ~CFileSystem();
		virtual bool existFile(const path& filename) const;
		virtual IReadFile* createAndOpenFile(const io::path& filename);
		virtual io::path getAbsolutePath(const io::path& filename) const;
	};
}//io
}//yon
#endif