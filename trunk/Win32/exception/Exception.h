/*
 * exception.h
 *
 *  Created on: 2013-2-1
 *  Author: OneThin
 */
#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include "ExceptionType.h"
#include <setjmp.h>

#define MAX_MESSAGE_LEN (63)

typedef enum {
	EXCEPTION_CAUGHT_FALSE = 0,
	EXCEPTION_CAUGHT_TRUE = 1
} EXCEPTION_CAUGHT;

typedef struct _JmpPointInf {
	jmp_buf env;
	const char message[MAX_MESSAGE_LEN+1];
	const char* file;
	int line;
	struct _JmpPointInf* next;
} JmpPointInf;

#define EXCEPTION_FILE (jmpPointInf.file)
#define EXCEPTION_LINE (jmpPointInf.line)
#define EXCEPTION_MESSAGE (jmpPointInf.message)

#define TRY { \
	JmpPointInf jmpPointInf; \
	pushJmpPoint(&jmpPointInf); \
	EXCEPTION_CAUGHT isCaught = EXCEPTION_CAUGHT_FALSE; \
	EXCEPTION exception = setjmp(jmpPointInf.env); \
	if (exception == 0) {

#define CATCH(e) } else if (exception == (e)) { \
		isCaught = EXCEPTION_CAUGHT_TRUE;

#define FINALLY } {

#define END_TRY } \
		if (exception == 0) { \
			popJmpPoint(__FILE__, __LINE__); \
		} else if (exception != 0 && isCaught == EXCEPTION_CAUGHT_FALSE) { \
			throwExcetion(exception, jmpPointInf.file, jmpPointInf.line, jmpPointInf.message); \
		} \
	}

#define THROW(e, message) throwExcetion((e), __FILE__, __LINE__, message);

void throwExcetion(EXCEPTION e, const char* file, const int line, const char* message);
void pushJmpPoint(JmpPointInf* pJmpPointInf);
JmpPointInf* popJmpPoint(const char* file, const int line);

#endif /* EXCEPTION_H_ */
