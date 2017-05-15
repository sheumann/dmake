#ifndef _STDLIB_INCLUDED_
#define _STDLIB_INCLUDED_

#ifndef _AIX
extern /*GOTO*/ _exit();
extern /*GOTO*/ exit();
extern /*GOTO*/ abort();
extern int system();
extern char *getenv();
extern char *calloc();
extern char *malloc();
extern char *realloc();

/* The AIX compiler dies on illegal redefinition of free */
extern free();
#endif

/* AIX doesn't use NAME_MAX anylonger... */
#ifdef _AIX
#include <unistd.h>
#define NAME_MAX pathconf(name,_PC_NAME_MAX)
#endif

extern int errno;

#ifndef EIO
#	include <errno.h>
#endif

#endif /* _STDLIB_INCLUDED_ */
