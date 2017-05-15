/*
** assorted bits of system interface, for common routines inside dmake.
** System specific code can be found in the config.h files for each
** of the system specifications.
*/
#define STAT stat
#define VOID_LCACHE(l,m)
#define Hook_std_writes(A)
#define GETPID 1

/*
** standard C items
*/

/*
** Mac interface standard items
*/
#define getswitchar()   '-'

/*
** make parameters
*/
#define MAX_PATH_LEN    1024
