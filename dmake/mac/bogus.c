/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/mac/bogus.c,v 1.1 1992/01/24 03:29:42 dvadura Exp $
-- SYNOPSIS -- Write the shell of subroutines we can't or don't
--             need to implement
--
-- DESCRIPTION
--  dmake uses a couple of functions which I am either unable to figure out
--  how to implement or which are not needed.  The shells of these routines
--  are in this file.
--
-- AUTHOR
--      Dennis Vadura, dvadura@watdragon.uwaterloo.ca
--      CS DEPT, University of Waterloo, Waterloo, Ont., Canada
--
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
--     $Log: bogus.c,v $
 * Revision 1.1  1992/01/24  03:29:42  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include "extern.h"



/*
 * tzset() is a Microsoft "extension" to ANSI C.  It sets global
 * variables telling if we are in dayling savings time, the time
 * zone, and difference between the current time and GMT.
 * None of these globals are used by dmake, so this routine is
 * not needed
 */
PUBLIC void tzset () {}



/*
 * Add an environmental variable that child processes can use.
 * Since MPW currently doesn't allow child processes, this isn't
 * needed.
 */
PUBLIC int putenv (char * /* pEnvString */) {return (0);}



/*
 * Execute a child process.  This may be able to be done with
 * the MPW system() call someday, but cannot be done currently.
 */
PUBLIC int runargv (CELLPTR /* target */, int /* ignore */, int /* group */,
                    int /* last */, int /* shell */, char * /* pCmd */) {
    static int warned = FALSE;

    if (!warned && !(Glob_attr & A_SILENT)) {
        warned = TRUE;
        Fatal ("Can't execute any targets:  use '-n' option.");
    } /* if */

    return (0);
} /* int runargv () */



/*
 * Wait for the child process to complete.  Only needed to be implemented
 * if we could executing multiple processes at once.
 */
PUBLIC int Wait_for_child(int /* abort_flg */, int /* pid */) {return (1);}



/*
 * Do any cleanup for any processes when we quit.
 */
PUBLIC void Clean_up_processes() {}
