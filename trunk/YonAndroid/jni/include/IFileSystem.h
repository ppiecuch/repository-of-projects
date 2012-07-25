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
		//ʹ�����ǵ�drop
		virtual IReadStream* createAndOpenReadFileStream(const io::path& filename,ENUM_ENDIAN_MODE mode=ENUM_ENDIAN_MODE_LITTLE) = 0;
		virtual IReadStream* createAndOpenReadMemoryStream(const io::path& name,void* data, long size,bool deleteMemoryWhenDropped,ENUM_ENDIAN_MODE mode=ENUM_ENDIAN_MODE_LITTLE) = 0;
		//���ļ������·�������ڣ����ݹ鴴����Ŀ¼
		virtual IWriteStream* createAndOpenWriteFileStream(const path& filename, bool append=false, ENUM_ENDIAN_MODE mode=ENUM_ENDIAN_MODE_LITTLE) =0;

		virtual XMLReader* createXMLReader(IReadStream* stream) =0;

		//��ȡ����·��
		//����inWorkingDirectory���Ƿ񰴹����ռ�·����ȡ
		//virtual YON_DEPRECATED io::path getAbsolutePath(const io::path& filename,bool inWorkingDirectory=true) const = 0;

		//���õ�ǰ�����ռ�·��
		//���� newDirectory: �¹����ռ��·��(ǰ��û��б�ܣ������б��)
		//��ʽ���£� "<drive>:/<directory>/<sudirectory>/<..>". �� "C:/Windows/","/sdcard/","../media/"
		//virtual YON_DEPRECATED void setWorkingDirectory(const io::path& newDirectory) =0;
		//virtual YON_DEPRECATED const io::path& getWorkingDirectory() = 0;

		//֧�ֶ������Ŀ¼
		//���� newDirectory: �¹����ռ��·��(ǰ��û��б�ܣ������б��)
		//��ʽ���£� "<drive>:/<directory>/<sudirectory>/<..>". �� "C:/Windows/","/sdcard/","../media/"
		virtual void addWorkingDirectory(const io::path& newDirectory) = 0;
		virtual s32 getWorkingDirectoryCount(){return 0;}
		virtual const io::path& getWorkingDirectory(s32 index) = 0;
		//��ȡ����·��
		virtual io::path getAbsolutePath(const io::path& filename) const = 0;
		//��ȡ��Դ����·��
		//����noEmpty�����Ϊtrue,ָʾ���������ƥ����Դ���򷵻�ʹ�õ�һ����ԴĿ¼ƴ�ӵ�·��������createStream�������򷵻�""
		//��������Ŀ¼���������ƥ�����Դ������·�����������noEmptyָʾ����
		virtual io::path getResourcePath(const io::path& filename,bool noEmpty=false) const = 0;
	};

	IFileSystem* createFileSystem();
}
}
#endif