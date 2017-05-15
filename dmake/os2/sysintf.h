/*
** assorted bits of system interface
*/

#define STAT stat
#define VOID_LCACHE(l,m)
#define Hook_std_writes(A)
#define GETPID getpid()

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
#define	MAX_PATH_LEN	256

