/*
	CmdPacket.cpp: implementation of the CCmdPacket class.
*/

#include "cmdpacket.h"

CCmdPacket::CCmdPacket()
{
	m_nMaxSize = 0;
	m_nLen = 0;
	m_nReadOffset  = 0;
	m_nWriteOffset = 0;
	m_pData = NULL;
	/* set buffer to default size */
	SetSize(DEFAULT_CMD_PACKET_SIZE);
}

CCmdPacket::~CCmdPacket()
{
	delete []m_pData;
}

bool CCmdPacket::SetSize(int len)
{
	if(len > MAX_CMD_PACKET_SIZE) return false;
	delete []m_pData;
	m_pData = NULL;
	m_pData = new char[len];
	m_nMaxSize = len;
	return m_pData?true:false;
}

/* try write data to packet */
bool CCmdPacket::WriteData(void *data, int len)
{
	if ((m_nLen + len) > m_nMaxSize) return false;
	memcpy(m_pData+m_nWriteOffset,data,len);
	m_nLen += len;
	m_nWriteOffset += len;
	return true;
}

bool CCmdPacket::WriteByte(char c)
{
	return WriteData(&c,BYTE_SIZE);
}

void CCmdPacket::BeginRead(char *p,int len)
{
	m_pReadData = p;
	m_nLen = len;
	m_nReadOffset = 0;
}

void CCmdPacket::BeginRead()
{
	m_pReadData = m_pData;
	m_nReadOffset = 0;
}

void CCmdPacket::BeginWrite()
{
	m_nLen = 0;
	m_nWriteOffset = 0;
}

bool CCmdPacket::WriteBinary(char *data, int len)
{
	if (WriteShort(len)==false) 
		return false;
	return WriteData(data,len);
}

bool CCmdPacket::ReadBinary(char **data, int *len)
{
	short dataLen;
	if (!ReadShort(&dataLen)) return false;
	if (dataLen <= 0) return false;
	if ((m_nReadOffset + dataLen) > m_nLen) return false;
	*data = m_pReadData + m_nReadOffset;
	*len = dataLen;
	m_nReadOffset += dataLen;
	return true;
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

bool CCmdPacket::WriteInt32(int l)
{
#ifdef STREAM_ORDER_BIG_ENDIANNESS
	unsigned int t = l;
	t = (t << 24) | ((t & 0xFF00)  << 8) | ((t & 0x00FF0000) >> 8) | (t >> 24);
	l = t;
#endif
	return WriteData(&l,LONG_SIZE);
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

bool CCmdPacket::ReadByte(unsigned char *c)
{
	return ReadData(c,BYTE_SIZE);
}

bool CCmdPacket::ReadShort(short *s)
{
#ifdef STREAM_ORDER_BIG_ENDIANNESS
	unsigned short t;
	bool bRet = ReadData(&t, SHORT_SIZE);
	*s = (t << 8) | (t >> 8);
	return bRet;
#else
	return ReadData(s,SHORT_SIZE);
#endif
}

bool CCmdPacket::ReadInt32(int *l)
{
#ifdef STREAM_ORDER_BIG_ENDIANNESS
	unsigned int t;
	bool bRet = ReadData(&t, LONG_SIZE);
	*l = (t << 24) | ((t & 0xFF00)  << 8) | ((t & 0x00FF0000) >> 8) | (t >> 24);
	return bRet;
#else
	return ReadData(l,LONG_SIZE);
#endif
}

bool CCmdPacket::ReadLongLong(long long *ll)
{
#ifdef STREAM_ORDER_BIG_ENDIANNESS
	unsigned long long t;
	bool bRet = ReadData(&t, LONG_LONG_SIZE);
	*ll = (t << 56) | ((t & 0xFF00)  << 40) | ((t & 0xFF0000) << 24) | ((t & 0xFF000000) << 8)
		| ((t & 0xFF00000000LL) >> 8) | ((t & 0xFF0000000000LL) >> 24)  | ((t & 0xFF000000000000LL) >> 40) | ((t & 0xFF00000000000000LL) >> 56);
	return bRet;
#else
	return ReadData(ll,LONG_SIZE);
#endif
}

#if 0
bool CCmdPacket::ReadInt32(int *i)
{
#ifdef STREAM_ORDER_BIG_ENDIANNESS
	unsigned int t;
	bool bRet = ReadData(&t, LONG_SIZE);
	*i = (t << 24) | ((t & 0xFF00)  << 8) | ((t & 0x00FF0000) >> 8) | (t >> 24);
	return bRet;
#else
	return ReadData(i,LONG_SIZE);
#endif
}
#endif

bool CCmdPacket::ReadFloat(float *f)
{
#ifdef STREAM_ORDER_BIG_ENDIANNESS
	int t;
	bool bRet = ReadData(&t, FLOAT_SIZE);
	t = (t << 24) | ((t & 0xFF00)  << 8) | ((t & 0x00FF0000) >> 8) | (t >> 24);
	float* tf = (float*) &t;
	*f = *tf;
	return bRet;
#else
	return ReadData(f,FLOAT_SIZE);
#endif
}
	
bool CCmdPacket::ReadString(char* pstr)
{
	if (pstr == 0)	return false;

	short len;
	if (!ReadShort(&len)) return false;
	if (len <= 0) return false;
	if ((m_nReadOffset + len) > m_nLen) return false;
	
	/* set end avoid overflow */
	*(pstr + len - 1) = '\0';
	
	return ReadData(pstr, len);
}

bool CCmdPacket::ReadString(char** ppstr)
{
	short len;
	if (!ReadShort(&len)) return false;
	if (len <= 0) return false;
	if ((m_nReadOffset + len) > m_nLen) return false;
	
	*ppstr = new char[len];
	if (*ppstr == 0)	return false;
	
	/* set end avoid overflow */
	*(*ppstr+len-1) = '\0';

	return ReadData(*ppstr, len);
}

bool CCmdPacket::ReadStringInPlace(char** ppstr)
{
	short len;
	if (!ReadShort(&len)) return false;
	if (len <= 0) return false;
	if ((m_nReadOffset + len) > m_nLen) return false;
	
	/* set end avoid overflow */
	*(m_pReadData+m_nReadOffset+len-1) = '\0';
	*ppstr = m_pReadData + m_nReadOffset;
	m_nReadOffset += len;
	return true;
}

#if 0	//·ÏÆú£¬²ÉÓÃË«×Ö½Úwchar_t±àÂë´«Êä
xc::c16* CCmdPacket::ReadString(xc::c16 *buf)
{
	xc::c16 *str = buf;
	short len;
	if (!ReadShort(&len))	return 0;
	if (len <= 0)			return 0;
	if ( (m_nReadOffset + len * WCHAR_SIZE ) > m_nLen) return 0;

	if (str == 0)
		str = GLL_NEW xc::c16[len+1];

#if 0
	/* set end avoid overflow */
	*(m_pReadData+m_nReadOffset+len-1) = '\0';
	*str = (wchar_t*)(m_pReadData + m_nReadOffset);
#endif
	xc::c16* p = str;
	for (short i = 0; i < len; i++)
	{	
		ReadShort((short*)p++);
	}
	*p = L'\0';	//½áÎ²null

	return str;
}

bool CCmdPacket::WriteString(const xc::c16 *str)
{
//	short len = wcslen(str) + 1;
	short len = strlenc16(str);
	if (!WriteShort(len)) return false;

	for (short i = 0; i < len; i++)
	{
		WriteShort(*str++);
	}
	return true;
}

#endif

bool CCmdPacket::WriteString(const char *str)
{
	short len = strlen(str);
	if (!WriteShort(len)) return false;
	return WriteData((void*)str,len);
}

bool CCmdPacket::ReadData(void *data, int len)
{
	if ((m_nReadOffset + len)>m_nLen) return false;
	
	memcpy(data,m_pReadData+m_nReadOffset,len);
	m_nReadOffset += len;
	return true;
}

bool CCmdPacket::CopyData(char *buf,int len)
{
	if(!SetSize(len)) return false;
	memcpy(m_pData,buf,len);
	m_nLen = len;
	return true;
}

bool CCmdPacket::CloneFrom(CCmdPacket *packet)
{
	return CopyData(packet->GetData(),packet->GetDataSize());
}

char *CCmdPacket::GetData()
{
	return m_pData;
}

int CCmdPacket::GetDataSize()
{
	return m_nLen;
}

int CCmdPacket::GetMaxSize()
{
	return m_nMaxSize;
}

