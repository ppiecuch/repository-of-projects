#if defined( DEBUG_MEMORY )

/**
 * file		: MemRecord.h
 * comment	:
 *
 */
#ifndef MemRecord_h
#define MemRecord_h 1

#include <string>
#include <map>
#include <hash_map>
#include <list>
#include <stack>
#include "memdbg/comheader.h"
#include "memdbg/commutex.h"

using namespace std;
using namespace stdext;

#define SINGLE_NEW		0x00		// indicate alloc memory with new type
#define ARRAY_NEW		0x01		// indicate alloc memory with new[] type

#define SINGLE_DELETE		0x02		// indicate free memory with delete type
#define ARRAY_DELETE		0x03		// indicate free memory with delete[] type

#define FILENAME_LENGTH		128    // the filename length

#define MEMORY_INFO		0X12345678	// indicate the message type on the message queue

#define VERBOSE_OUTPUT 0


typedef struct
{
	char		    Filename[ FILENAME_LENGTH ];
	unsigned long	LineNum;
	size_t 		    AllocSize;
	int		        OperationType;
#if VERBOSE_OUTPUT == 0
    void *	        pBuffer;	// stores the pointer
#else
    int             References;
#endif
	short		    errCode;	// 0 - released, 1 - delete array from new[] 
}MemOperation;

typedef struct 
{
	int	Type;			// message type, in this module must be MEMORY_INFO
	MemOperation Data;		// content of memory operation
} MsgBuffer;

class MemRecord
{
public:
	MemRecord();
	~MemRecord();

    void Insert( void *pBuffer, MemOperation *pRecord );
	int Erase( void *pBuffer, MemOperation *pRecord );

	void Insert( const char *pBuffer, MemOperation *pRecord );
	int Erase( const char *pBuffer, MemOperation *pRecord );

public:
#if VERBOSE_OUTPUT == 0
    hash_map<void*, MemOperation> m_hashMemory;
#else
    hash_map<string, MemOperation> m_hashMemory;
#endif
private:

	list<MemOperation> m_listMemory;
	CCommonMutex m_mutexRecord;
};

typedef struct 
{
	char		Filename[ FILENAME_LENGTH ];	// 
	unsigned long	LineNum;	// 
}DELINFOSTACK;

//#define THIS_FILE          __FILE__

void* operator new( size_t nSize, char* pszFileName, int nLineNum );
void* operator new[]( size_t nSize, char* pszFileName, int nLineNum );

void operator delete( void *ptr );
void operator delete[]( void *ptr );

#define DEBUG_NEW new( THIS_FILE, __LINE__ )

extern char DELETE_FILE[ FILENAME_LENGTH ];
extern int DELETE_LINE;
extern void BuildStack();
extern CCommonMutex globalLock;

			// if the DELETE_LINE is not 0, it means we need keep the older 
			// info in mind for subsequence call.


#define DEBUG_DELETE 	globalLock.Lock(); \
			if (DELETE_LINE != 0) BuildStack();\
			strncpy( DELETE_FILE, __FILE__,FILENAME_LENGTH - 1 ); \
			DELETE_FILE[ FILENAME_LENGTH - 1 ]= '\0'; \
			DELETE_LINE = __LINE__; \
			delete

#endif

#endif // DEBUG_MEMORY
