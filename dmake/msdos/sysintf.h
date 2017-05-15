/*
** assorted bits of system interface
*/

#define STAT stat
#define VOID_LCACHE(l,m)
#define GETPID _psp

extern char * tempnam();
extern char * getcwd();

/*
** standard C items
*/

/*
** DOS interface standard items
*/
#define	chdir(p) _chdir(p)

/*
** make parameters
*/
#ifdef _POSIX_NAME_MAX
#undef _POSIX_NAME_MAX
#endif
#define _POSIX_NAME_MAX 12
#define _POSIX_PATH_MAX 64
#define	MAX_PATH_LEN	_POSIX_PATH_MAX
