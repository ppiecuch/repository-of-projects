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
		virtual IReadStream* createAndOpenReadFileStream(const io::path& filename,ENUM_ENDIAN_MODE mode);
		virtual IWriteStream* createAndOpenWriteFileStream(const path& filename, bool append, ENUM_ENDIAN_MODE mode);
		virtual XMLReader* createXMLReader(IReadStream* stream);
		virtual io::path getAbsolutePath(const io::path& filename,bool inWorkingDirectory=true) const;
		virtual void setWorkingDirectory(const io::path& newDirectory);
		virtual const io::path& getWorkingDirectory();
	};
}//io
}//yon
#endif