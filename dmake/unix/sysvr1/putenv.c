/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/unix/sysvr1/putenv.c,v 1.1 1992/01/24 03:28:26 dvadura Exp $
-- SYNOPSIS -- my own putenv for BSD like systems.
-- 
-- DESCRIPTION
-- 	This originally came from MKS, but I rewrote it to fix a bug with
--	replacing existing strings, probably never happened but the code
--	was wrong nonetheless.
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
--     $Log: putenv.c,v $
 * Revision 1.1  1992/01/24  03:28:26  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include <stdio.h>
#include <string.h>

int
putenv( str )/*
===============
   Take a string of the form NAME=value and stick it into the environment.
   We do this by allocating a new set of pointers if we have to add a new
   string and by replacing an existing pointer if the value replaces the value
   of an existing string. */
char *str;
{
   extern char **environ;		/* The current environment. */
   static char **ourenv = NULL;		/* A new environment	    */
   register char **p;
   register char *q;
   int      size;

   /* First search the current environment and see if we can replace a
    * string. */
   for( p=environ; *p; p++ ) {
      register char *s = str;

      for( q = *p; *q && *s && *s == *q; q++, s++ )
	 if( *s == '=' ) {
	    *p = str;
	    return(0);			/* replaced it so go away */
	 }
   }

   /* Ok, can't replace a string so need to grow the environment. */
   size = p - environ + 2;	/* size of new environment */
				/* size of old is size-1   */

   /* It's the first time, so allocate a new environment since we don't know
    * where the old one is comming from. */
   if( ourenv == NULL ) {
      if( (ourenv = (char **) malloc( sizeof(char *)*size )) == NULL )
	 return(1);

      memcpy( (char *)ourenv, (char *)environ, (size-2)*sizeof(char *) );
   }
   else if( (ourenv = (char **)realloc( ourenv, size*sizeof(char *))) == NULL )
      return(1);

   ourenv[--size] = NULL;
   ourenv[--size] = str;

   environ = ourenv;
   return(0);
}
