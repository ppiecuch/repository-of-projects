#pragma once 

#include <string> 

class Log 
{ 
public: 

	enum enumOutType 
	{ 
		ONull   = 0, 
		OFile   = 1, 
		OScreen = 2 
	}; 

	static void Initialize(const char* FileName = NULL,bool bDeleteOld = false); 

	static void Out( long Channel,const char* pFmt, ... ); 
	static void Outl( long Channel,const char* pFmt, ... ); 

	static void OutBinary(long Channel,const char *pMem,unsigned long Length); 

private: 
	//FILE* m_Logfile; 
	static char m_LogFileName[256]; 

	static void OutBinaryLine(std::string* str,const char *pAlignMem,const char *pMem,unsigned long Length); 


}; 
