/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/RCS/quit.c,v 1.1 1992/01/24 03:27:53 dvadura Exp $
-- SYNOPSIS -- end the dmake session.
-- 
-- DESCRIPTION
-- 	Handles dmake termination.
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
--     $Log: quit.c,v $
 * Revision 1.1  1992/01/24  03:27:53  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#if defined (__ORCAC__)
# pragma databank 1
#endif

#include "extern.h"

static	void	_handle_quit ANSI((char*));
static	int	_dont_quit = 0;


PUBLIC void
Quit(void)/*
======== Error or quit */
{
   if( _dont_quit ) return;

   while( Closefile() != NIL( FILE ) );
   Clean_up_processes();

   if( Current_target != NIL(CELL) )
      Unlink_temp_files(Current_target);

   if( _dont_quit == 0 ) _handle_quit( ".ERROR" );

   Set_dir( Makedir );		/* No Error message if we can't do it */

   Epilog( ERROR_EXIT_VALUE );
}


static void
_handle_quit( char *err_target )/*
============================
   Called by quit and the others to handle the execution of termination code
   from within make */
{
   HASHPTR hp;
   CELLPTR cp;

   if( (hp = Get_name(err_target, Defs, FALSE)) != NIL(HASH) ) {
      cp = hp->CP_OWNR;
      Glob_attr |= A_IGNORE;

      _dont_quit = 1;
      cp->ce_flag |= F_TARGET;
      Make( cp, NIL(CELL) );
   }
}

#if defined (__ORCAC__)
# pragma databank 0
#endif
