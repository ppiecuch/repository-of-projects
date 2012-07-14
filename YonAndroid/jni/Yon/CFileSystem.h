#ifndef _YON_IO_CFILESYSTEM_H_
#define _YON_IO_CFILESYSTEM_H_

#include "IFileSystem.h"
#include "yonArray.h"


namespace yon{
namespace io{

	class CFileSystem : public IFileSystem{
	protected:
		io::path m_workingDirectory;
		core::array<io::path> m_workingDirectories;
	public:
		CFileSystem();
		virtual ~CFileSystem();
		virtual bool existFile(const path& filename) const;
		virtual IReadStream* createAndOpenReadFileStream(const io::path& filename,ENUM_ENDIAN_MODE mode);
		virtual IReadStream* createAndOpenReadMemoryStream(const io::path& name,void* data, long size,bool deleteMemoryWhenDropped,ENUM_ENDIAN_MODE mode);
		virtual IWriteStream* createAndOpenWriteFileStream(const path& filename, bool append, ENUM_ENDIAN_MODE mode);
		virtual XMLReader* createXMLReader(IReadStream* stream);
		//virtual io::path getAbsolutePath(const io::path& filename,bool inWorkingDirectory) const;
		//virtual void setWorkingDirectory(const io::path& newDirectory);
		//virtual const io::path& getWorkingDirectory();

		virtual void addWorkingDirectory(const io::path& newDirectory);
		virtual s32 getWorkingDirectoryCount(){
			return m_workingDirectories.size();
		}
		virtual const io::path& getWorkingDirectory(s32 index){
			YON_DEBUG_BREAK_IF(index>=m_workingDirectories.size());
			return m_workingDirectories[index];
		}
		virtual io::path getAbsolutePath(const io::path& filename) const;
		virtual io::path getResourcePath(const io::path& filename) const;
	};
}
}
#endif