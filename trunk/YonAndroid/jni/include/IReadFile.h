#ifndef _YON_IO_IREADFILE_H_
#define _YON_IO_IREADFILE_H_

#include "IReferencable.h"
#include "path.h"

namespace yon{
namespace io{

	class IReadFile : public virtual core::IReferencable{
	public:
		//���ض�ȡ���ֽ���
		virtual s32 read(void* buffer, u32 sizeToRead) = 0;

		//offset:ƫ����
		//relative:����ڵ�ǰλ��
		virtual bool seek(long offset, bool relative = false) = 0;

		virtual FILE* pointer() const = 0;

		//��ȡ�ļ���С�����ֽ�Ϊ��λ
		virtual long getSize() const = 0;

		//��ǰλ�ã����ֽ�Ϊ��λ
		virtual long getPos() const = 0;

		//�ļ���
		virtual const io::path& getFileName() const = 0;
		//·����
		virtual const io::path& getPathName() const = 0;
	};

	IReadFile* createReadFile(const io::path& fileName);
}//io
}//yon
#endif