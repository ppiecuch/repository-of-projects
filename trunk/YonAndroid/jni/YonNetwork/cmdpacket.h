
#ifndef __CMD_PACKET_H__
#define __CMD_PACKET_H__

#include "netconfig.h"

/* define default packet buffer size */
const int DEFAULT_CMD_PACKET_SIZE = 1024;
/* define max packet buffer size */
const int MAX_CMD_PACKET_SIZE	  = 1024 * 16;

/* define some length used in packet class */
const int BYTE_SIZE  = 1;
const int LONG_SIZE  = 4;
const int SHORT_SIZE = 2;
const int FLOAT_SIZE = 4;
const int LONG_LONG_SIZE  = 8;
const int WCHAR_SIZE = 2;		// !! Attention, sizeof(wchar_t) on android = 4


//�����
//����������������/д���ķ�װ
//NOTE: ��/д����ע���ֽ����С��
class CCmdPacket  
{
public:
	CCmdPacket();
	virtual ~CCmdPacket();

	void BeginWrite();
	void BeginRead(char *p,int len);	//��ʼ����״̬
	void BeginRead();

	bool ReadBinary(char **data,int *len);

	// �ַ�������UTF-8���봫��
	// ��ʽ�� | short | UTF8 string | '\0' | 
	//    ����һ��short��ʾ�������ݵĳ���+�ַ�������+0��β
	// Ϊ��Ч�ʿ��ǣ��ṩ��һ���ȡ��������ע��ʹ��˵����
	//
	// ReadString(char* pstr) 
	// - �û��Լ��ṩ�ַ������棬ע��Ҫ�ṩ�㹻��Ļ��棬����Խ��
	// - һ�������û�֪�����ַ�������󳤶ȣ������˺ţ������16���ַ��� 
	//
	// ReadString(char** ppstr) 
	// - �û����ṩ�ַ������棬�ɱ������ڲ����룬ʹ����Ϻ���Ҫ�û��ֶ�delete��
	// - һ�����ڲ�֪�����ַ�����ȷ�г��ȣ������ǱȽϳ����ַ�����
	//
	// ReadStringInPlace(char** ppstr) 
	// - �û����ṩ�ַ������棬�������ڲ�ֻ���޸ĵ�ַ��Ҳ����ֱ��ʹ������ģ���ڲ��Ļ��棬
	// - ȷ����ҪԽ������޸ģ�������ܵ�������ģ�������
	// - ��Ҫ�� ��ָ��ֻ�ڵ��ú�������Ч���˳����ú�����ʧЧ��
	// - �ú���Ч����ߣ����漰�κ�����ʱ��̬�ڴ���䣬Ψһ��Ҫ���ǽ���ʹ�ã�

	bool ReadString(char* pstr);
	bool ReadString(char** ppstr);
	bool ReadStringInPlace(char** ppstr);

	bool ReadFloat(float *f);
	bool ReadInt32(int *l);
	bool ReadLongLong(long long *ll);
	//	bool ReadInt(int *l);
	bool ReadShort(short *s);
	bool ReadByte(unsigned char *c);

	bool WriteBinary(char *data,int len);

	// д��0��β��c�ַ���
	// ��ʽ�ο�ReadString
	// ע�⣺��β0�ֽڲ�д��������
	bool WriteString(const char *str);
	//	bool WriteString(const xc::c16 *str);

	bool WriteFloat(float f);
	bool WriteInt32(int l);
	bool WriteShort(short s);
	bool WriteByte(char c);

	char *GetData();
	int   GetDataSize();
	int   GetMaxSize();

	bool  SetSize(int len);
	bool  CloneFrom(CCmdPacket *packet);

private :
	bool  CopyData(char *buf,int len);
	bool  WriteData(void *data,int len);
	bool  ReadData(void *data,int len);

	char *m_pData;			//д��ַ
	char *m_pReadData;		//����ַ
	int   m_nLen;			//���ݳ���
	int   m_nReadOffset;	//��ƫ��
	int   m_nWriteOffset;	//дƫ��
	int   m_nMaxSize;		//��󳤶�
};

#endif