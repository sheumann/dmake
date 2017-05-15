/* if gsh $PATH parsing ever gets straighted out, remove this */
#define BACKWARDS

#include <sys/wait.h>
#include <errno.h>
#include "extern.h"
#include "sysintf.h"

static int  _abort_flg = FALSE;
static void _add_child ANSI((CELLPTR, int));
static void _finished_child ANSI((int));

static int child_status; /* the exit value of the child process */

/* These two are from lenviron */
extern char *build_cmd (char *const *argv);
extern char *build_path (const char *file);

#pragma databank 1

static void child_process(char *path, char *comd) {
	_execve(path, comd);
	return;
}
#pragma databank 0

/*
 * static int execp(const char *file, char *const argv[]);
 *
 * This is like the Gno/ME exec(2) routine in that it waits for the child
 * to exit before returning.  The difference is that it will find <file>
 * if it is in the $PATH, and expects an array of strings (vice a single
 * string) as the second argument.
 *
 * This is very much a _non_ standard function ...
 *
 * returns:
 *   -1 and errno set if execp call failed (child_status undefined)
 *   otherwise return 0, errno=0, and child_status is the return value
 *    of the called program <file>
 */

static int execp(const char *file, char *const argv[]) {
   char *comd;
   char *path;
	int result;
	int w_result;
   union wait w_status;
	int terrno;

   /* errno = 0;  (This is set in runargv() just before calling execp()) */

	/*
	 * Are we using a full or partial pathname?  If partial, then we have
    * to build it.
    */

	if (If_root_path(file)) {
	   /* full pathname */
      path = strdup(file);
      if (!path) return -1;
   } else {
	   /* partial pathname:  is $PATH defined in the environment? */
	   path = getenv("PATH");
   	if (path) {              /* PATH exists; good start ... */
         path = build_path (file);
         if (path == NULL) return -1;
   	} else {                 /* no PATH; use default */
#ifdef BACKWARDS
      	if (setenv("PATH","/usr/bin /bin",1) != 0) return -1;
#else
      	if (setenv("PATH","/bin /usr/bin",1) != 0) return -1;
#endif
      	path = build_path (file);
      	result = (path == NULL) ? -1 : 0;
      	terrno = errno;     	/* unsetenv may affect errno */
      	unsetenv("PATH");
      	errno = terrno;
      	if (result!=0) return -1;
   	}
   }                         

   /* build the command line */
   comd = build_cmd (argv);
   if (comd == NULL) {
	   terrno = errno;
      free(path);
      errno = terrno;
   	return -1;
   }

   /* execute it */
  	result = fork2(child_process, 1536, 0, "forked child of dmake", 4,
   					path, comd);

   /* the parent has to wait for the child to exit so we can get the status */
   if (result != -1) {
   	
      /* this method will only work if there are no other children! */
      for (;;) {
	   	w_result = wait (&w_status);
         if (w_result == -1) {
	         result = -1;
         	break;
         }
         /*
          * we can ignore WIFSIGNALLED in the following test because we
          * have a signal handler installed
          */
         if (WIFEXITED(w_status) || WIFSIGNALED(w_status)) {
	      	child_status = w_status.w_retcode;
         	result = 0;
         	break;
         }
      }
   }

   /*
    * At this point, "result" has been set under all conditions, and there
    * is no child process left.
    */
    
   /* free the memory */
   terrno = errno;
   free(path);
   free(comd);
   errno = terrno;

   return result;
}

PUBLIC int
runargv(CELLPTR target, int ignore, int group, int last, int shell, char *cmd)
{
   char **argv;
   int pid;

   argv = Pack_argv( group, shell, cmd );
   _add_child(target, ignore);

   errno = 0;
	if (execp (*argv, argv) == -1)
   	Error("%s: %s", argv[0], strerror(errno));

   _finished_child(child_status);
   if( last && !Doing_bang ) Update_time_stamp( target );

   return 0;  /* means: all children exited */
}


PUBLIC void
Clean_up_processes(void)
{
   _abort_flg = TRUE;
   _finished_child(-1);
}


PUBLIC int
Wait_for_child( int abort_flg, int pid )
{
   return(1);
}


static int     _valid = -1;
static CELLPTR _tg;
static int     _ignore;

static void
_add_child( CELLPTR target, int ignore )
{
   _tg = target;
   _ignore = ignore;
   _valid = 0;

   Current_target = NIL(CELL);
}


static void
_finished_child( int status)
{
   if( _valid == -1 ) return;
   Unlink_temp_files( _tg );
   _valid = -1;
   Handle_result( status, _ignore, _abort_flg, _tg );
}
