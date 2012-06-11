#include "FileWriter.h"

CFileWriter::CFileWriter(const char* filePath,char *tag_string):m_filePath(filePath)
{
	m_pFile=fopen(filePath,tag_string);

	//if(m_pFile==NULL)
	//³ö´í
}
void CFileWriter::writeChar(char c)
{
	fwrite(&c,sizeof(char),1,m_pFile);
}
void CFileWriter::writeInt(int num)
{
	fwrite(&num,sizeof(int),1,m_pFile);
}
void CFileWriter::writeFloat(float num)
{
	fwrite(&num,sizeof(float),1,m_pFile);
}
void CFileWriter::writeBool(bool b)
{
	fwrite(&b,sizeof(bool),1,m_pFile);
}
void CFileWriter::writeString(const char* str,char sep)
{
	fwrite(str,sizeof(char),strlen(str),m_pFile);
	writeChar(sep);
}


void CFileWriter::close()
{
	if(m_pFile)
		fclose(m_pFile);
}
CFileWriter::~CFileWriter(void)
{
}

void CFileWriter::seek( int i )
{
	
 fseek(m_pFile,i,1);
}

void CFileWriter::writeIntArray( int * array_in,float count )
{
      fwrite(array_in,sizeof(int),count,m_pFile);
}

void CFileWriter::writeCharArray( char *array_in,float  count )
{
    
     fwrite(array_in,sizeof(char),count,m_pFile);

}

void CFileWriter::writeShort( short s )
{
	fwrite(&s,sizeof(short),1,m_pFile);
     
}