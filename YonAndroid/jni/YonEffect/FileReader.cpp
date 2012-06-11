//#include "FileReader.h"

CFileReader::CFileReader(const char* filePath):m_filePath(filePath)
{
	m_pFile=fopen(filePath,"rb+");
	//if(m_pFile==NULL)
	//出错
}
bool CFileReader::exist()
{
	return m_pFile!=NULL;
}
int CFileReader::readInt()
{
	int num;
	fread(&num,sizeof(int),1,m_pFile);
	return num;
}
float CFileReader::readFloat()
{
	float num;
	fread(&num,sizeof(float),1,m_pFile);
	return num;
}
bool CFileReader::readBool()
{
	bool b;
	fread(&b,sizeof(bool),1,m_pFile);
	return b;
}
char* CFileReader::readString(int sep)
{
	char *pstr;
	int ch;
	int n=0,i=0,l=0;
	char buf[100];
	pstr=(char*)calloc(100,1);
	while((ch=fgetc(m_pFile))!=sep){
		n++;
		if(i<100){
			buf[i]=ch;
			i++;
		}else{
			l=strlen(pstr);
			pstr=(char*)realloc(pstr,n+100); // a bit more
			strncpy(pstr+l,buf,100);
			buf[0]=ch;
			i=1;
		}
	}
	l=strlen(pstr);
	//多分配两个字节，一个用于放'\0',一个用于free()函数调用（否则会报after Normal block)
	pstr=(char*)realloc(pstr,n+2);
	if(n%100!=0)
		strncpy(pstr+l,buf,i); // copy those in the current buff
	*(pstr+n+1)='\0';
	return pstr;
}
void CFileReader::close()
{
	if(m_pFile)
		fclose(m_pFile);
}
CFileReader::~CFileReader(void)
{
}

char CFileReader::readChar()
{
	char ch;
	fread(&ch,sizeof(char),1,m_pFile);
	return ch;

}

void CFileReader::readFloatArray( float *out,float count )
{
	
	fread(out,sizeof(float),count,m_pFile);
	

}

void CFileReader::seek( int i )
{
	
	fseek(m_pFile,i,1);

}

void CFileReader::readCharArray( char * out,float count )
{

	fread(out,sizeof(char),count,m_pFile);

}

void CFileReader::readIntArray( int *out,float count)
{

	fread(out,sizeof(int),count,m_pFile);

}

short CFileReader::readShort()
{
     short s;
	 fread(&s,sizeof(short),1,m_pFile);
	 return s;

}