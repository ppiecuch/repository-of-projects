#include "cmdpacket.h"
#include "gamenetwork.h"

io::IFileSystem* CCmdPacket::s_pFileSystem=NULL;

void CCmdPacket::setFileSystem(io::IFileSystem* fs)
{
	s_pFileSystem=fs;
}

CCmdPacket::CCmdPacket()
:m_pBuffer(NULL),m_pReadStream(NULL)
{
}

CCmdPacket::~CCmdPacket()
{
	//if(m_pBuffer)
	//	GameNetwork()->recycleBuffer(m_pBuffer);
	if(m_pReadStream)
		m_pReadStream->drop();
}

void CCmdPacket::BeginRead(char *p,int len)
{
	YON_DEBUG_BREAK_IF(len<0);
	
#ifdef STREAM_ORDER_BIG_ENDIANNESS
	io::ENUM_ENDIAN_MODE mode=io::ENUM_ENDIAN_MODE_BIG;
#else
	io::ENUM_ENDIAN_MODE mode=io::ENUM_ENDIAN_MODE_LITTLE;
#endif
	if(m_pReadStream)
	{
		m_pReadStream->drop();
		m_pReadStream=NULL;
	}
	m_pReadStream=s_pFileSystem->createAndOpenReadMemoryStream("cmdpacket",p,(u32)len,false,mode);
}

bool CCmdPacket::ReadString(char* pstr)
{
	if(m_pReadStream)
	{
		short len;
		bool result=ReadShort(&len);
		YON_DEBUG_BREAK_IF(!result||len<0||(u32)len>m_pReadStream->getSize()-m_pReadStream->getPos());
		if(!result||len<=0)
			return false;
		s32 length=m_pReadStream->read((u8*)pstr,len);
		YON_DEBUG_BREAK_IF(length<len);
		if(length<len)
			return false;

		core::stringc str;
		for(short i=0;i<len;++i)
			str.append(core::stringc("%d,",((c8*)pstr)[i]));
		YON_DEBUG("ReadString:%s\r\n",str.c_str());


		return true;
	}
	return false;
}

bool CCmdPacket::ReadStringInPlace(char** ppstr)
{
	short len;
	if (!ReadShort(&len)) return false;
	//if (len <= 0) return false;
	//if ((m_nReadOffset + len) > m_nLen) return false;
	YON_DEBUG_BREAK_IF(len<=0||(u32)len>m_pReadStream->getSize()-m_pReadStream->getPos());

	/* set end avoid overflow */
	//*(m_pReadData+m_nReadOffset+len-1) = '\0';
	//*ppstr = m_pReadData + m_nReadOffset;
	//m_nReadOffset += len;

	char* p=(char*)m_pReadStream->pointer();
	p[m_pReadStream->getPos()+len-1]='\0';
	*ppstr=p+m_pReadStream->getPos();
	m_pReadStream->seek(len,true);

	/*inPlaces.push_back(core::stringc(""));
	core::stringc& str=inPlaces[inPlaces.size()-1];

	str.reserve(len);
	char* p=(char*)m_pReadStream->pointer();
	memcpy(str.pointer(),p+m_pReadStream->getPos(),len);
	str[len-1]='\0';
	str.repair(len);
	*ppstr=str.pointer();
	m_pReadStream->seek(len,true);*/
	return true;
}

bool CCmdPacket::ReadFloat(float *f)
{
	if(m_pReadStream)
	{
		*f=m_pReadStream->readFloat();
		return true;
	}
	return false;
}
bool CCmdPacket::ReadInt32(int *l)
{
	if(m_pReadStream)
	{
		*l=m_pReadStream->readInt();
		return true;
	}
	return false;
}
bool CCmdPacket::ReadLongLong(long long *ll)
{
	if(m_pReadStream)
	{
		*ll=m_pReadStream->readLong();
		return true;
	}
	return false;
}
bool CCmdPacket::ReadShort(short *s)
{
	if(m_pReadStream)
	{
		*s=m_pReadStream->readShort();
		return true;
	}
	return false;
}
bool CCmdPacket::ReadByte(unsigned char *c)
{
	if(m_pReadStream)
	{
		*c=m_pReadStream->readUnsignedByte();
		return true;
	}
	return false;
}

void CCmdPacket::BeginWrite()
{
	YON_DEBUG_BREAK_IF(m_pBuffer!=NULL);

	m_pBuffer=GameNetwork()->getBuffer();
	m_pBuffer->Buffer.append("   ");
}

bool CCmdPacket::WriteData(void *data,int len)
{
	YON_DEBUG_BREAK_IF(m_pBuffer==NULL||len<0);
	m_pBuffer->Buffer.append((c8*)data,len);
	return true;
}

bool CCmdPacket::WriteBinary(char *data,int len)
{
	if (WriteShort(len)==false) 
	{
		YON_WARN(YON_LOG_WARN_FORMAT,"WriteShort in WriteBinary failed!");
		return false;
	}
	return WriteData(data,len);
}

bool CCmdPacket::WriteString(const char *str)
{
	short len = strlen(str);
	if (!WriteShort(len))
	{
		YON_WARN(YON_LOG_WARN_FORMAT,"WriteShort in WriteString failed!");
		return false;
	}
	return WriteData((void*)str,len);
}

bool CCmdPacket::WriteFloat(float f)
{
#ifdef STREAM_ORDER_BIG_ENDIANNESS
	unsigned int* l = (unsigned int*)&f;
	*l = (*l << 24) | ((*l & 0xFF00)  << 8) | ((*l & 0x00FF0000) >> 8) | (*l >> 24);
	return WriteData(l,FLOAT_SIZE);
#endif
	return WriteData(&f,FLOAT_SIZE);
}

bool CCmdPacket::WriteInt32(int l)
{
#ifdef STREAM_ORDER_BIG_ENDIANNESS
	unsigned int t = l;
	t = (t << 24) | ((t & 0xFF00)  << 8) | ((t & 0x00FF0000) >> 8) | (t >> 24);
	l = t;
#endif
	return WriteData(&l,LONG_SIZE);
}

bool CCmdPacket::WriteShort(short s)
{
#ifdef STREAM_ORDER_BIG_ENDIANNESS
	unsigned short t = s;
	t = (t << 8) | (t >> 8);
	s = t;
#endif
	return WriteData(&s,SHORT_SIZE);
}

bool CCmdPacket::WriteByte(char c)
{
	return WriteData(&c,BYTE_SIZE);
}

char* CCmdPacket::GetData()
{
	YON_DEBUG_BREAK_IF(m_pBuffer==NULL);
	return (char*)m_pBuffer->Buffer.pointer();
}
int CCmdPacket::GetDataSize()
{
	YON_DEBUG_BREAK_IF(m_pBuffer==NULL);
	return (int)m_pBuffer->Buffer.length();
}

bool CCmdPacket::SetSize(int len)
{
	m_pBuffer->setSize((u32)len);
	return true;
}