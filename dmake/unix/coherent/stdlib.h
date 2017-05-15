#ifndef _STDLIB_INCLUDED_
#define _STDLIB_INCLUDED_

extern /*GOTO*/ _exit();
extern /*GOTO*/ exit();
extern /*GOTO*/ abort();
extern int system();
extern char *getenv();
extern char *calloc();
extern char *malloc();
extern char *realloc();
extern free();
extern int errno;

#ifndef EIO
#	include <errno.h>
#endif

#endif /* _STDLIB_INCLUDED_ */
