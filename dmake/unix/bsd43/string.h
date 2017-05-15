/*
** BSD does this wrong
*/
#include <strings.h>

#include "stdmacs.h"
extern	char*	strpbrk ANSI((char* src, char* any));

#ifndef DBUG
#define	strchr(str,c)	index(str,c)
#define	strrchr(str,c)	rindex(str,c)
#else
char *strchr ANSI((char*, char));
char *strrchr ANSI((char*, char));
#endif

