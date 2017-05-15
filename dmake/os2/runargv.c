/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/os2/runargv.c,v 1.1 1992/01/24 03:29:19 dvadura Exp $
-- SYNOPSIS -- invoke a sub process, modified unix/runargv.c for OS/2.
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
 * Revision 1.1  1992/01/24  03:29:19  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#define INCL_DOSPROCESS
#include <os2.h>

#include <process.h>
#include <stdlib.h>
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
   char		*pr_dir;
} PR;

static PR  *_procs    = NIL(PR);
static int  _proc_cnt = 0;
static int  _abort_flg= FALSE;
static int  _use_i    = -1;
static int  _do_upd   = 0;

extern unsigned int _far _pascal DosSmSetTitle(char _far *s);
static  void	SetSessionTitle (char *s);
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

#ifdef SESSTITLE
   SetSessionTitle(target->CE_NAME);
#endif
   argv = Pack_argv( group, shell, cmd );

   if((pid=spawnvp((_osmode == DOS_MODE)?P_WAIT:P_NOWAIT,argv[0],argv)) == -1){
      Error("%s: %s", argv[0], sys_errlist[errno]);
      Handle_result(-1, ignore, _abort_flg, target);
      return(-1);
   }
   else if( _osmode == DOS_MODE ) {
     _add_child(4711, target, ignore, last);
     _finished_child(4711, pid);
   }
   else
     _add_child(pid, target, ignore, last);

   return(1);
}


#ifdef SESSTITLE
/* N.B. The system call used below is undocumented and therefore possibly
 * subject to change. It sets the session title even from a full screen
 * session, so you can see which target is being built.
 * If dubious about undocumented calls simply remove it.
 */
PUBLIC void
SetSessionTitle(char *s)
{
   char buff[128];
   strncpy(buff, Pname, sizeof(buff));
   buff[sizeof(buff)-1] = 0;
   strncat(buff, " - ", sizeof(buff));
   strncat(buff, s, sizeof(buff));
   buff[sizeof(buff)-1] = 0;
   DosSmSetTitle(buff);
}
#endif


PUBLIC int
Wait_for_child( abort_flg, pid )
int abort_flg;
int pid;
{
   int wid;
   int status;
   int waitchild;

   if( _osmode == DOS_MODE ) return(1);

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

   if( _osmode == DOS_MODE ) {
      _abort_flg = TRUE;
      _finished_child(4711, -1);
      return;
   }

   if( _procs != NIL(PR) ) {
      for( i=0; i<Max_proc; i++ )
	 if( _procs[i].pr_valid )
	    DosKillProcess(DKP_PROCESSTREE, _procs[i].pr_pid);

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
