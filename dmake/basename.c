/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/RCS/basename.c,v 1.1 1992/01/24 03:26:56 dvadura Exp $
-- SYNOPSIS -- return pointer to last pathname component
-- 
-- DESCRIPTION
--	take a file name like /fred/foo/hoe/mary.k, and return the 'mary.k'
--	portion
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
--     $Log: basename.c,v $
 * Revision 1.1  1992/01/24  03:26:56  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include "extern.h"

PUBLIC char*
basename( path )
char *path;
{
   char *p;
   char *q;

   if( *(q = path) ) {
      for(; *(p=_strpbrk(q, DirBrkStr)) != '\0'; q = p+1 );
      if( !*q ) {
	 for( p=q-1; p != path; --p )
	    if( strchr( DirBrkStr, *p ) == NIL(char) ) return( p+1 );
	 return( strchr(DirBrkStr, *p)?path:(p+1) );
      }
      path = q;
   }
   return( path );
}
