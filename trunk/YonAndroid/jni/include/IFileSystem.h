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
			//ʹ�����ǵ�drop
			virtual YON_DEPRECATED IReadFile* createAndOpenFile(const io::path& filename) = 0;
			virtual IReadStream* createAndOpenReadFileStream(const io::path& filename,ENUM_ENDIAN_MODE mode=ENUM_ENDIAN_MODE_LITTLE) = 0;
			//��ȡ����·��
			//����inWorkingDirectory���Ƿ񰴹����ռ�·����ȡ
			virtual io::path getAbsolutePath(const io::path& filename,bool inWorkingDirectory=true) const = 0;

			//���õ�ǰ�����ռ�·��
			//���� newDirectory: �¹����ռ��·��(ǰ��û��б�ܣ������б��)
			//��ʽ���£� "<drive>:/<directory>/<sudirectory>/<..>". �� "C:/Windows/","/sdcard/","../media"
			virtual void setWorkingDirectory(const io::path& newDirectory) =0;
			virtual const io::path& getWorkingDirectory() = 0;
		};

		IFileSystem* createFileSystem();
	}//io
}//yon
#endif