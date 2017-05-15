/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/unix/runargv.c,v 1.1 1992/01/24 03:28:50 dvadura Exp $
-- SYNOPSIS -- invoke a sub process.
-- 
-- DESCRIPTION
-- 	Use the standard methods of executing a sub process.
--
-- AUTHOR
--      Dennis Vadura, dvadura@watdragon.uwaterloo.ca
--      CS DEPT, University of Waterloo, Waterloo, Ont., Canada
--
-- COPYRIGHT
--      Copyright (c) 1990 by Dennis Vadura.  All rights reserved.
-- 
--      This program is free software; you can redistribute it and/or
--      modify it under the terms of the GNU General Public License
--      (version 1), as published by the Free Software Foundation, and
--      found in the file 'LICENSE' included with this distribution.
-- 
--      This program is distributed in the hope that it will be useful,
--      but WITHOUT ANY WARRANTY; without even the implied warrant of
--      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--      GNU General Public License for more details.
-- 
--      You should have received a copy of the GNU General Public License
--      along with this program;  if not, write to the Free Software
--      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
--
-- LOG
--     $Log: runargv.c,v $
 * Revision 1.1  1992/01/24  03:28:50  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include <signal.h>
#include "extern.h"
#include "sysintf.h"

typedef struct prp {
   char *prp_cmd;
   int   prp_group;
   int   prp_ignore;
   int   prp_last;
   int	 prp_shell;
   struct prp *prp_next;
} RCP, *RCPPTR;

typedef struct pr {
   int		pr_valid;
   int		pr_pid;
   CELLPTR	pr_target;
   int		pr_ignore;
   int		pr_last;
   RCPPTR  	pr_recipe;
   RCPPTR  	pr_recipe_end;
   char        *pr_dir;
} PR;

static PR  *_procs    = NIL(PR);
static int  _proc_cnt = 0;
static int  _abort_flg= FALSE;
static int  _use_i    = -1;
static int  _do_upd   = 0;

static  void	_add_child ANSI((int, CELLPTR, int, int));
static  void	_attach_cmd ANSI((char *, int, int, CELLPTR, int, int));
static  void    _finished_child ANSI((int, int));
static  int     _running ANSI((CELLPTR));

PUBLIC int
runargv(target, ignore, group, last, shell, cmd)
CELLPTR target;
int     ignore;
int	group;
int	last;
int     shell;
char	*cmd;
{
   extern  int  errno;
   extern  char *sys_errlist[];
   int          pid;
   char         **argv;

   if( _running(target) /*&& Max_proc != 1*/ ) {
      /* The command will be executed when the previous recipe
       * line completes. */
      _attach_cmd( cmd, group, ignore, target, last, shell );
      return(1);
   }

   while( _proc_cnt == Max_proc )
      if( Wait_for_child(FALSE, -1) == -1 )  Fatal( "Lost a child" );

   argv = Pack_argv( group, shell, cmd );

   switch( pid=fork() ){
      int   wid;
      int   status;

   case -1: /* fork failed */
      Error("%s: %s", argv[0], sys_errlist[errno]);
      Handle_result(-1, ignore, _abort_flg, target);
      return(-1);

   case 0:  /* child */
      execvp(argv[0], argv);
      Continue = TRUE;   /* survive error message */
      Error("%s: %s", argv[0], sys_errlist[errno]);
      kill(getpid(), SIGTERM);
      /*NOTREACHED*/

   default: /* parent */
      _add_child(pid, target, ignore, last);
   }

   return(1);
}


PUBLIC int
Wait_for_child( abort_flg, pid )
int abort_flg;
int pid;
{
   int wid;
   int status;
   int waitchild;

   waitchild = (pid == -1)? FALSE : Wait_for_completion;

   do {
      if( (wid = wait(&status)) == -1 ) return(-1);

      _abort_flg = abort_flg;
      _finished_child(wid, status);
      _abort_flg = FALSE;
   }
   while( waitchild && pid != wid );

   return(0);
}


PUBLIC void
Clean_up_processes()
{
   register int i;

   if( _procs != NIL(PR) ) {
      for( i=0; i<Max_proc; i++ )
	 if( _procs[i].pr_valid )
	    kill(_procs[i].pr_pid, SIGTERM);

      while( Wait_for_child(TRUE, -1) != -1 );
   }
}


static void
_add_child( pid, target, ignore, last )
int	pid;
CELLPTR target;
int	ignore;
int     last;
{
   register int i;
   register PR *pp;

   if( _procs == NIL(PR) ) {
      TALLOC( _procs, Max_proc, PR );
   }

   if( (i = _use_i) == -1 )
      for( i=0; i<Max_proc; i++ )
	 if( !_procs[i].pr_valid )
	    break;

   pp = _procs+i;

   pp->pr_valid  = 1;
   pp->pr_pid    = pid;
   pp->pr_target = target;
   pp->pr_ignore = ignore;
   pp->pr_last   = last;
   pp->pr_dir    = _strdup(Get_current_dir());

   Current_target = NIL(CELL);

   _proc_cnt++;

   if( Wait_for_completion ) Wait_for_child( FALSE, pid );
}


static void
_finished_child(pid, status)
int	pid;
int	status;
{
   register int i;
   register PR *pp;
   char     *dir;

   for( i=0; i<Max_proc; i++ )
      if( _procs[i].pr_valid && _procs[i].pr_pid == pid )
	 break;

   /* Some children we didn't make esp true if using /bin/sh to execute a
    * a pipe and feed the output as a makefile into dmake. */
   if( i == Max_proc ) return;
   _procs[i].pr_valid = 0;
   _proc_cnt--;
   dir = _strdup(Get_current_dir());
   Set_dir( _procs[i].pr_dir );

   if( _procs[i].pr_recipe != NIL(RCP) && !_abort_flg ) {
      RCPPTR rp = _procs[i].pr_recipe;


      Current_target = _procs[i].pr_target;
      Handle_result( status, _procs[i].pr_ignore, FALSE, _procs[i].pr_target );
      Current_target = NIL(CELL);

      _procs[i].pr_recipe = rp->prp_next;

      _use_i = i;
      runargv( _procs[i].pr_target, rp->prp_ignore, rp->prp_group,
	       rp->prp_last, rp->prp_shell, rp->prp_cmd );
      _use_i = -1;

      FREE( rp->prp_cmd );
      FREE( rp );

      if( _proc_cnt == Max_proc ) Wait_for_child( FALSE, -1 );
   }
   else {
      Unlink_temp_files( _procs[i].pr_target );
      Handle_result(status,_procs[i].pr_ignore,_abort_flg,_procs[i].pr_target);

      if( _procs[i].pr_last ) {
	 FREE(_procs[i].pr_dir );

	 if( !Doing_bang ) Update_time_stamp( _procs[i].pr_target );
      }
   }

   Set_dir(dir);
   FREE(dir);
}


static int
_running( cp )
CELLPTR cp;
{
   register int i;

   if( !_procs ) return(FALSE);

   for( i=0; i<Max_proc; i++ )
      if( _procs[i].pr_valid &&
	  _procs[i].pr_target == cp  )
	 break;
	 
   return( i != Max_proc );
}


static void
_attach_cmd( cmd, group, ignore, cp, last, shell )
char    *cmd;
int	group;
int     ignore;
CELLPTR cp;
int     last;
int     shell;
{
   register int i;
   RCPPTR rp;

   for( i=0; i<Max_proc; i++ )
      if( _procs[i].pr_valid &&
	  _procs[i].pr_target == cp  )
	 break;

   TALLOC( rp, 1, RCP );
   rp->prp_cmd   = _strdup(cmd);
   rp->prp_group = group;
   rp->prp_ignore= ignore;
   rp->prp_last  = last;
   rp->prp_shell = shell;

   if( _procs[i].pr_recipe == NIL(RCP) )
      _procs[i].pr_recipe = _procs[i].pr_recipe_end = rp;
   else {
      _procs[i].pr_recipe_end->prp_next = rp;
      _procs[i].pr_recipe_end = rp;
   }
}
