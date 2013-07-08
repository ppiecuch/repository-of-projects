/*
 * exception.c
 *
 *  Created on: 2013-2-1
 *      Author: bnus
 */
#include "Exception.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static JmpPointInf* g_pJmpPointStack = NULL;

void pushJmpPoint(JmpPointInf* pJmpPointInf) {
	pJmpPointInf->next = g_pJmpPointStack;
	g_pJmpPointStack = pJmpPointInf;
}

JmpPointInf* popJmpPoint(const char* file, const int line) {
	JmpPointInf* pJmpPointInf = g_pJmpPointStack;
	if (pJmpPointInf == NULL) {
		fprintf(stderr, "NULL pJmpPointInf,Line[%d],File[%s]\n", line, file);
		fprintf(stderr, "abort!\n");
		abort();
	}
	g_pJmpPointStack = g_pJmpPointStack->next;

	return pJmpPointInf;
}

void throwExcetion(EXCEPTION e, const char* file, const int line, const char* message) {
	JmpPointInf* pJmpPoint = g_pJmpPointStack;
	if (pJmpPoint == NULL) {
		fprintf(stderr, "Uncaught Exception[%d],Line[%d],File[%s],Message[%s]\n", e, line, file, message);
		fprintf(stderr, "abort!\n");
		abort();
	}
	pJmpPoint->file = file;
	pJmpPoint->line = line;
	strncpy((char*)(pJmpPoint->message), message, sizeof(pJmpPoint->message));
	*((char*)(pJmpPoint->message) + (sizeof pJmpPoint->message) - 1) = '\0';

	g_pJmpPointStack = g_pJmpPointStack->next;

	longjmp(pJmpPoint->env, e);
}
