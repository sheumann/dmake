/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/tos/runargv.c,v 1.1 1992/01/24 03:27:13 dvadura Exp $
-- SYNOPSIS -- run a sub process.
-- 
-- DESCRIPTION
--	Use spawn to run a subprocess.
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
 * Revision 1.1  1992/01/24  03:27:13  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include <process.h>
#include <errno.h>
#include "extern.h"
#include "sysintf.h"

static int  _abort_flg = FALSE;
static void _add_child ANSI((CELLPTR, int));
static void _finished_child ANSI((int));

PUBLIC int
runargv(target, ignore, group, last, shell, cmd)
CELLPTR target;
int     ignore;
int	group;
int	last;
int	shell;
char	*cmd;
{
   int status;
   char **argv;
   char path[MAX_PATH_LEN+1];

   argv = Pack_argv( group, shell, cmd );
   _add_child(target, ignore);

   /* save and restore current working directory across a spawn call */
   strcpy(path, Get_current_dir());
   status = spawnvp(P_WAIT, *argv, argv);
   Set_dir(path);

   if( status == -1 ) Error("%s: %s", argv[0], strerror(errno));
   _finished_child(status);
   if( last && !Doing_bang ) Update_time_stamp( target );

   return( 0 );
}


PUBLIC void
Clean_up_processes()
{
   _abort_flg = TRUE;
   _finished_child(-1);
}


PUBLIC int
Wait_for_child( abort_flg, pid )
int abort_flg;
int pid;
{
   return(1);
}


static int     _valid = -1;
static CELLPTR _tg;
static int     _ignore;

static void
_add_child( target, ignore )
CELLPTR target;
int	ignore;
{
   _tg = target;
   _ignore = ignore;
   _valid = 0;

   Current_target = NIL(CELL);
}


static void
_finished_child(status)
int	status;
{
   if( _valid == -1 ) return;
   Unlink_temp_files( _tg );
   _valid = -1;
   Handle_result( status, _ignore, _abort_flg, _tg );
}
