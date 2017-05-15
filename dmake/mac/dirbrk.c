/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/mac/dirbrk.c,v 1.1 1992/01/24 03:29:44 dvadura Exp $
-- SYNOPSIS -- define the directory separator string.
-- 
-- DESCRIPTION
--  Define this string for any character that may appear in a path name
--  and can be used as a directory separator.
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
--     $Log: dirbrk.c,v $
 * Revision 1.1  1992/01/24  03:29:44  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include "extern.h"

/* mac only uses ':' */
char*   DirBrkStr = ":";

/*
** Return TRUE if the name is the full specification of a path name to a file
** starting at the root of the file system, otherwise return FALSE
*/
PUBLIC int
If_root_path(name)
char *name;
{
   name = Unix2MacFName (name);
   return( (strchr(name, ':') != NIL(char)) &&
           (name[0] != ':') );
}
