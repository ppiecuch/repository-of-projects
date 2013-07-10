#if defined( DEBUG_MEMORY )

#include "memdbg/memrecord.h"

MemRecord appMemory;

char DELETE_FILE[ FILENAME_LENGTH ] = {0};
int DELETE_LINE = 0;
CCommonMutex globalLock;
stack<DELINFOSTACK> globalStack;

void BuildStack()
{

	DELINFOSTACK stDis = {"",0};
	strcpy(stDis.Filename, DELETE_FILE);
	stDis.LineNum = DELETE_LINE;

	globalStack.push(stDis);
}

void* operator new( size_t nSize, char* pszFileName, int nLineNum )
{
	void *pResult;
	
	pResult = ::operator new( nSize );
	
	if ( pResult && nLineNum )
	{
#if VERBOSE_OUTPUT == 0
        MemOperation record;
		// record the alloc memory
		strncpy( record.Filename, pszFileName, FILENAME_LENGTH - 1 );
		record.Filename[ FILENAME_LENGTH - 1 ] = '\0';
		record.LineNum = nLineNum;
		record.AllocSize = nSize;
		record.OperationType = SINGLE_NEW;
	    record.errCode = 0;
		record.pBuffer = pResult;
        appMemory.Insert( pResult, &record );
#elif VERBOSE_OUTPUT == 1
        char tmp[MAX_PATH] = {0};
        sprintf(tmp, "%s%:%d",pszFileName, nLineNum);
        hash_map<string, MemOperation>::iterator it = appMemory.m_hashMemory.find(tmp);
        if( appMemory.m_hashMemory.end() != it)
        {
            ++(it->second.References);
            return pResult;
        }

        MemOperation record;
		// record the alloc memory
		strncpy( record.Filename, pszFileName, FILENAME_LENGTH - 1 );
		record.Filename[ FILENAME_LENGTH - 1 ] = '\0';
		record.LineNum = nLineNum;
        record.References = 1;
		record.AllocSize = nSize;
		record.OperationType = SINGLE_NEW;
	    record.errCode = 0;
        record.errCode = 0;
        appMemory.Insert( tmp, &record );
#endif


	}
	
	return pResult;
}

void* operator new[]( size_t nSize, char* pszFileName, int nLineNum )
{
	void *pResult;
	
	pResult = ::operator new[]( nSize );
	
	if ( pResult && nLineNum )
	{
#if VERBOSE_OUTPUT == 0
        MemOperation record;
		// record the alloc memory
		strncpy( record.Filename, pszFileName, FILENAME_LENGTH - 1 );
		record.Filename[ FILENAME_LENGTH - 1 ] = '\0';
		record.LineNum = nLineNum;
		record.AllocSize = nSize;
        record.pBuffer = pResult;
		record.OperationType = ARRAY_NEW;
        record.errCode = 0;
		appMemory.Insert( pResult, &record );
#elif VERBOSE_OUTPUT == 1
        char tmp[MAX_PATH] = {0};
        sprintf(tmp, "%s%:%d",pszFileName, nLineNum);
        hash_map<string, MemOperation>::iterator it = appMemory.m_hashMemory.find(tmp);
        if( appMemory.m_hashMemory.end() != it)
        {
            ++(it->second.References);
            return pResult;
        }

        MemOperation record;
		// record the alloc memory
		strncpy( record.Filename, pszFileName, FILENAME_LENGTH - 1 );
		record.Filename[ FILENAME_LENGTH - 1 ] = '\0';
		record.LineNum = nLineNum;
        record.References = 1;
		record.AllocSize = nSize;
		record.OperationType = ARRAY_NEW;
        record.errCode = 1;
        appMemory.Insert( tmp, &record );
#endif
	}

	return pResult;
}


void operator delete( void *ptr )
{
	if ( NULL == ptr )
	{
		globalLock.Unlock();
		return;
	}

    if (DELETE_LINE)
    {
#if VERBOSE_OUTPUT == 0
        MemOperation record;
        strncpy( record.Filename, DELETE_FILE, FILENAME_LENGTH - 1 );
        record.Filename[ FILENAME_LENGTH - 1 ] = '\0';
        record.LineNum = DELETE_LINE;
        record.AllocSize = 0; // Unknown, but compiler known
        record.OperationType = SINGLE_DELETE;
        record.errCode = 0;
        record.pBuffer = ptr;

        strcpy( DELETE_FILE , "" );
	    DELETE_LINE = 0 ;
    	globalLock.Unlock();

        appMemory.Erase( ptr, &record );
#elif VERBOSE_OUTPUT == 1
        char tmp[MAX_PATH] = {0};
        sprintf(tmp, "%s%:%d",DELETE_FILE, DELETE_LINE);
        hash_map<string, MemOperation>::iterator it = appMemory.m_hashMemory.find(tmp);
        if( appMemory.m_hashMemory.end() != it) {
            ++(it->second.References);
            strcpy( DELETE_FILE , "" );
            DELETE_LINE = 0;
            globalLock.Unlock();
        } else {

	        MemOperation record;
	        strncpy( record.Filename, DELETE_FILE, FILENAME_LENGTH - 1 );
	        record.Filename[ FILENAME_LENGTH - 1 ] = '\0';
	        record.LineNum = DELETE_LINE;
            record.References = 1;
	        record.AllocSize = 0; // Unknown, but compiler known
	        record.OperationType = SINGLE_DELETE;
            record.errCode = 2;

	        strcpy( DELETE_FILE , "" );
	        DELETE_LINE = 0 ;
    	
	        globalLock.Unlock();

            appMemory.Insert( tmp, &record );
      }
#endif
    }

	free( ptr );
}


void
operator delete[]( void *ptr )
{
	if ( NULL == ptr )
	{
		globalLock.Unlock();
		return;
	}

    if (DELETE_LINE)
    {
#if VERBOSE_OUTPUT == 0
        MemOperation record;
        strncpy( record.Filename, DELETE_FILE, FILENAME_LENGTH - 1 );
        record.Filename[ FILENAME_LENGTH - 1 ] = '\0';
        record.LineNum = DELETE_LINE;
        record.AllocSize = 0; // Unknown, but compiler known
        record.OperationType = ARRAY_DELETE;
        record.errCode = 0;
        record.pBuffer = ptr;

        strcpy( DELETE_FILE , "" );
	    DELETE_LINE = 0 ;

    	globalLock.Unlock();
        appMemory.Erase( ptr, &record );
#elif VERBOSE_OUTPUT == 1
        char tmp[MAX_PATH] = {0};
        sprintf(tmp, "%s%:%d",DELETE_FILE, DELETE_LINE);
        hash_map<string, MemOperation>::iterator it = appMemory.m_hashMemory.find(tmp);
        if( appMemory.m_hashMemory.end() != it) {
            ++(it->second.References);
            strcpy( DELETE_FILE , "" );
            DELETE_LINE = 0;
            globalLock.Unlock();
        } else {

	        MemOperation record;
	        strncpy( record.Filename, DELETE_FILE, FILENAME_LENGTH - 1 );
	        record.Filename[ FILENAME_LENGTH - 1 ] = '\0';
	        record.LineNum = DELETE_LINE;
            record.References = 1;
	        record.AllocSize = 0; // Unknown, but compiler known
	        record.OperationType = ARRAY_DELETE;
            record.errCode = 3;

	        strcpy( DELETE_FILE , "" );
	        DELETE_LINE = 0 ;

	        globalLock.Unlock();
            appMemory.Insert( tmp, &record );
      }
#endif
    }

	free( ptr );
}

MemRecord::MemRecord()
{
}

MemRecord::~MemRecord()
{
	if (!m_hashMemory.empty() || !m_listMemory.empty())
	{
		printf("MemRecord : We think there's some memory leak happened, please check the leak.rec file for detail.\r\n");
		// Truncate  file  to  zero  length or create text file for writing
		FILE *fp = fopen( "leak.rec", "w" ); 
		if ( NULL == fp ) // if fail to open the file, assignment stdout to it
		{
			fp = stdout;
		}
		
#if VERBOSE_OUTPUT == 0
		fprintf( fp, "Memory leak %d times, description as below:\n\n",
			m_hashMemory.size() + m_listMemory.size() );
		fprintf( fp, "%-60s%-16s%-16s%-16s%-8s%-s\n",
			"FILE NAME", "LINE NUMBER", "MEMORY SIZE",
			"POINTER", "ALLOC", "LEAK REASON" );
		fprintf( fp, "------------------------------------------------"
			"-------------------------------------------------------------------------------\n" );
		
		// print the content of m_hashMemory and m_listMemory to leak.rec file
        hash_map<void*, MemOperation>::iterator mapIterator = m_hashMemory.begin();
		char *types[2] = { "new", "new[]" };
		char *err[2] = { "not free", "use delete" };
		
		// output the content of m_hashMemory to file
		while( mapIterator != m_hashMemory.end() )
		{
			fprintf( fp, "%-60s%-16d%-16d%-#16X%-8s%s\n",
				 mapIterator->second.Filename, mapIterator->second.LineNum,
				 mapIterator->second.AllocSize, 
				 mapIterator->second.pBuffer,
				 types[ mapIterator->second.OperationType % 2 ],
				 err[ mapIterator->second.errCode ] );
			fflush( fp );
			mapIterator++;
		}
		list<MemOperation>::iterator record;
		// output the content of m_listMemory to file
		for ( record = m_listMemory.begin(); record != m_listMemory.end(); record++ )
		{
			fprintf( fp, "%-60s%-16d%-16d%-#16X%-8s%s\n",
				 record->Filename, record->LineNum,
				 record->AllocSize,
				 record->pBuffer,
				 types[ record->OperationType % 2 ],
				 err[ record->errCode ] );
			fflush( fp );
		}
		fprintf( fp, "------------------------------------------------"
			"-------------------------------------------------------------------------------\n" );
#elif VERBOSE_OUTPUT == 1
        fprintf( fp, "Memory statistik %d times, description as below:\n\n",
			m_hashMemory.size() + m_listMemory.size() );
		fprintf( fp, "%-60s%-16s%-16s%-10s%-10s%-10s\n",
			"FILE NAME", "LINE NUMBER", "MEMORY SIZE",
			"POINTER", "ALLOC", "REFERENCES" );
		fprintf( fp, "------------------------------------------------"
			"-------------------------------------------------------------------------------\n" );
		
		// print the content of m_hashMemory and m_listMemory to leak.rec file
		hash_map<string, MemOperation>::iterator mapIterator = m_hashMemory.begin();
		char *types[4] = { "new", "new[]", "delete", "delete[]" };
		
		// output the content of m_hashMemory to file
		while( mapIterator != m_hashMemory.end() )
		{
            const MemOperation& r = mapIterator->second; 
			fprintf( fp, "%-60s%-16d%-16d%-#10X%-10s%-10d\n",
				 r.Filename, r.LineNum,
				 r.AllocSize, 
	             0,
				 types[ r.OperationType % 4 ],
                 r.References );
			fflush( fp );
			mapIterator++;
		}
		list<MemOperation>::iterator record;
		// output the content of m_listMemory to file
		for ( record = m_listMemory.begin(); record != m_listMemory.end(); ++record )
		{
			fprintf( fp, "%-60s%-16d%-16d%-#10X%-10s%-10d\n",
				 record->Filename, record->LineNum,
				 record->AllocSize,
				 0,
				 types[ record->OperationType % 4 ],
                 record->References );
			fflush( fp );
		}
		fprintf( fp, "------------------------------------------------"
			"-------------------------------------------------------------------------------\n" );
#endif
    }
}

void
MemRecord::Insert( void *pBuffer, MemOperation *pRecord )
{
#if VERBOSE_OUTPUT == 0

	m_mutexRecord.Lock();
	pair<void*, MemOperation> value(pBuffer, *pRecord);

	m_hashMemory.insert( value );
	m_mutexRecord.Unlock();
#endif
}

void
MemRecord::Insert( const char *pBuffer, MemOperation *pRecord )
{
#if VERBOSE_OUTPUT == 1

	m_mutexRecord.Lock();
	pair<string, MemOperation> value(pBuffer, *pRecord);

	m_hashMemory.insert( value );
	m_mutexRecord.Unlock();
#endif
}

int
MemRecord::Erase( void *pBuffer, MemOperation *pRecord )
{
#if VERBOSE_OUTPUT == 0
	hash_map<void*, MemOperation>::iterator record;

	if((!strcmp(pRecord->Filename,""))&&(pRecord->LineNum == 0))
	{
		record = m_hashMemory.find( pBuffer );
		// if the global delete info is empty and cannot find this
		// pointer in our map, that mean the pointer of this delete request
		// is not "new" by us. so just drop it
		if ( record == m_hashMemory.end() )
		{
			return -1;
		}

		// if we can find the pointer in our map but the global delete info
		// is zero, it maybe a "recursion(ตÝน้) delete", so if stack isn't empty we will
		// pop the corresponding delete info in the stack and go on.
		if (!globalStack.empty())
		{
			DELINFOSTACK stDis = globalStack.top();
			
			strcpy(pRecord->Filename, stDis.Filename);
			pRecord->LineNum = stDis.LineNum;
			
			globalStack.pop();
		}
	}


	m_mutexRecord.Lock();


	record = m_hashMemory.find( pBuffer );

	if ( record != m_hashMemory.end() )
	{
		
		bool bError = 
			( SINGLE_DELETE == pRecord->OperationType ) && ( ARRAY_NEW == record->second.OperationType );
		MemOperation temp;
		
		if ( bError ) // error if alloc with new[] type and free with delete type
		{
			memcpy( &temp, &( record->second ), sizeof( MemOperation ) );
			temp.errCode = 1;
			m_listMemory.push_back( temp );
		}		
		m_hashMemory.erase( record );

		m_mutexRecord.Unlock();
		return 0;
	}
	else
	{
		
		// delete a  memory pointer that no-existed or alloc with new[] type
		// it will cause the uncertain result, so I will give a warning message on screen
		printf( "MemRecord : *****WARNING: At %s line %ld -- %s, You delete a pointer that cannot find in MAP.******\n",
			pRecord->Filename, pRecord->LineNum , pBuffer);
		m_mutexRecord.Unlock();
		
		return -2;
		
	}
#elif VERBOSE_OUTPUT == 1
    return 0;
#endif
}

int
MemRecord::Erase( const char *pBuffer, MemOperation *pRecord )
{
#if VERBOSE_OUTPUT == 1
    string strBuffer(pBuffer);

	if((!strcmp(pRecord->Filename,""))&&(pRecord->LineNum == 0))
	{
        hash_map<string, MemOperation>::iterator record;
		record = m_hashMemory.find( strBuffer );
		// if the global delete info is empty and cannot find this
		// pointer in our map, that mean the pointer of this delete request
		// is not "new" by us. so just drop it
		if ( record == m_hashMemory.end() )
		{
			return -1;
		}

		// if we can find the pointer in our map but the global delete info
		// is zero, it maybe a "recursion(ตÝน้) delete", so if stack isn't empty we will
		// pop the corresponding delete info in the stack and go on.
		if (!globalStack.empty())
		{
			DELINFOSTACK stDis = globalStack.top();
			
			strcpy(pRecord->Filename, stDis.Filename);
			pRecord->LineNum = stDis.LineNum;
			
			globalStack.pop();
		}
	}

#elif VERBOSE_OUTPUT == 0
    return 0;
#endif
}

#endif




          
                                          
