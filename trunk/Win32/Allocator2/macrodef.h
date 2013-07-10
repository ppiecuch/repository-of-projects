#if defined( DEBUG_MEMORY )

#ifndef _MACRO_H
#define _MACRO_H

#ifndef WIN32
/*Define Boolean Value*/
#define TRUE		1
#define FALSE		0


/*Define RtnCode To User*/
#define	SUCCESS		0x00
#endif

typedef unsigned char		U8 ;
typedef unsigned short		U16 ;

#ifndef WIN32
typedef unsigned int		UINT ;
typedef bool			BOOL ;
typedef unsigned long		DWORD;
#endif

typedef unsigned long		timeout_t;

#endif

#endif // DEBUG_MEMORY