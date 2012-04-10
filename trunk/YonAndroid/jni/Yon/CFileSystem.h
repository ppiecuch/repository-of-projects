#ifndef _YON_IO_CFILESYSTEM_H_
#define _YON_IO_CFILESYSTEM_H_

#include "IFileSystem.h"


namespace yon{
namespace io{

	class CFileSystem : public IFileSystem{
	protected:
		io::path m_workingDirectory;
	public:
		CFileSystem();
		virtual ~CFileSystem();
		virtual bool existFile(const path& filename) const;
		virtual IReadFile* createAndOpenFile(const io::path& filename);
		virtual IReadStream* createAndOpenReadFileStream(const io::path& filename,ENUM_ENDIAN_MODE mode);
		virtual io::path getAbsolutePath(const io::path& filename) const;
		virtual const io::path& getWorkingDirectory();
	};
}//io
}//yon
#endif