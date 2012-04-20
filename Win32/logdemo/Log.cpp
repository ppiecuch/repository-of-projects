//	#########################################################################
/**
	@file			log.cpp
	@author			bluehe

	@remarks		

	(C) 2006-2007 U-Soft Co., Ltd.
*/
//	##########################################################################

#include "stdafx.h"
#include "assert.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "Log.h"
#include <string>

char Log::m_LogFileName[256] = "log.txt";

void Log::Initialize(const char* FileName,bool bDeleteOld)
{
    if(FileName)
    {
        long size = strlen(FileName);
        if(size > 255)
            size = 255;
        strncpy(m_LogFileName,FileName,size);
    }

    if( bDeleteOld )
    {
        FILE* file= fopen( m_LogFileName, "w" );
        fclose(file);
    }
}

void Log::Out( long Channel,const char* pFmt, ... )
{
    if( Channel & OFile )
    {
    	FILE * pFile;
        pFile = fopen( m_LogFileName, "ab" );
    	if ( pFile != NULL )
	    {
		    va_list arg;
    		va_start( arg, pFmt );
	    	vfprintf( pFile, pFmt, arg );
		    va_end( arg );
		    fclose( pFile );
        }
	}
    
    if( Channel & OScreen)
    {
	    va_list arg;
		va_start( arg, pFmt );
    	vprintf( pFmt, arg );
	    va_end( arg );
    }
}

void Log::OutBinaryLine(std::string* str,const char *pAlignMem,const char *pMem,unsigned long Length)
{
    unsigned long Align = (pMem - pAlignMem);
    assert( Length + Align <= 16);

    char buff[256];

    sprintf(buff,"%08X ",(const int)pAlignMem);
    *str = buff;

    unsigned long i;

    {
        for(i=0; i< Align; i++)
            *str += "   ";

        for(i=0; i< Length; i++)
        {
            sprintf(buff,"%02X", (unsigned char)pMem[i]);
            *str += buff;

            *str += (i == 7 - Align) ? "-" : " ";
        }

        for(i=0; i< 16 - Length - Align; i++)
            *str += "   ";
    }
    *str += "  ";

    {
        for(i=0; i< Align; i++)
            *str += " ";

        for(i=0; i< Length; i++)
        {
            sprintf(buff,"%c",  isgraph( pMem[i] ) ? (unsigned char)pMem[i] : '.');
            *str += buff;
        }

        for(i=0; i< 16 - Length - Align; i++)
            *str += " ";
    }

    *str += "\n";
}

void Log::OutBinary(long Channel,const char *pMem,unsigned long Length)
{
    if( Channel == ONull )
        return;

    FILE * pFile = NULL;
    if( Channel & OFile)
        pFile = fopen( m_LogFileName, "ab" );


    std::string str;
    unsigned long Align = ((unsigned long)pMem) % 16;

    const char *pMemPoint = pMem;
    unsigned long LineLength = Align + Length > 16 ? 16-Align : (pMem + Length - pMemPoint);

    OutBinaryLine( &str,pMemPoint - Align,pMemPoint,LineLength);

    if( Channel & OScreen)
        printf("%s",str.c_str());
    if( (Channel & OFile) && pFile)
        fprintf(pFile,"%s",str.c_str());


    pMemPoint += LineLength;

    if( pMemPoint - pMem < (long)Length)
    {
        LineLength = (pMem + Length - pMemPoint) > 16 ? 16 : (pMem + Length - pMemPoint);

        do
        {
            OutBinaryLine( &str,pMemPoint,pMemPoint,LineLength);

            if( Channel & OScreen)
                printf("%s",str.c_str());
            if( (Channel & OFile) && pFile)
                fprintf(pFile,"%s",str.c_str());

            pMemPoint += LineLength;

            LineLength = (pMem + Length - pMemPoint) > 16 ? 16 : (pMem + Length - pMemPoint);
        }while( pMemPoint - pMem < (long)Length);
    }

    if( (Channel & OFile) && pFile)
        fclose( pFile );
}

/* ********************************************************************** *\
	History :
				$Log: log.cpp,v $
				Revision 1.16  2007/02/15 06:29:23  bluehe
				移除LOGC_xxx定义,转移到logcannel中去
				
				Revision 1.15  2007/02/12 07:57:46  bluehe
				重写OutBinary()
				
				Revision 1.14  2007/02/09 13:48:06  oscarsong
				no message
				
				Revision 1.13  2007/02/09 12:10:51  oscarsong
				no message
				
				Revision 1.12  2007/02/09 09:24:50  bluehe
				no message
				
				Revision 1.11  2007/02/09 02:19:00  oscarsong
				no message
				
				Revision 1.10  2007/02/09 01:44:53  bluehe
				no message
				
				Revision 1.9  2007/02/08 01:41:07  bluehe
				no message
				
				Revision 1.8  2007/02/07 09:32:47  oscarsong
				no message
				
				Revision 1.7  2007/02/06 12:03:01  bluehe
				no message
				
				Revision 1.6  2007/02/05 10:51:21  bluehe
				no message
				
				Revision 1.5  2007/02/02 02:05:17  oscarsong
				no message
				
				Revision 1.4  2007/02/01 09:46:43  bluehe
				将LOGC_xxx 实现为常量，以后在cpp里修改
				
				Revision 1.3  2007/01/26 03:30:46  bluehe
				no message
				
				Revision 1.2  2007/01/18 09:14:10  bluehe
				no message
				
				Revision 1.1  2007/01/18 06:55:34  bluehe
				no message
				
	- End -
\* ********************************************************************** */
