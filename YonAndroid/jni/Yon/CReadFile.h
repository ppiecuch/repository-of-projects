#ifndef _YON_IO_CREADFILE_H_
#define _YON_IO_CREADFILE_H_

#include <stdio.h>
#include "IReadFile.h"

namespace yon{
namespace io{

	class CReadFile : public IReadFile{
	private:
		void openFile();

		FILE* m_pFile;
		long m_fileSize;
		io::path m_filename;
	public:
		CReadFile(const io::path& fileName);
		virtual ~CReadFile();

		virtual s32 read(void* buffer, u32 sizeToRead);
		virtual bool seek(long offset, bool relative);
		virtual long getPos() const;

		virtual long getSize() const{
			return m_fileSize;
		}
		virtual const io::path& getFileName() const
		{
			return m_filename;
		}

		virtual bool isOpen() const
		{
			return m_pFile != NULL;
		}
	};
	
}//io
}//yon
#endif