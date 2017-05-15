/* RCS      -- $Header$
-- SYNOPSIS -- define the directory separator string.
-- 
-- DESCRIPTION
-- 	Define this string for any character that may appear in a path name
--	and can be used as a directory separator.  Also provide a function
--	to indicate if a given path begins at the root of the file system.
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
 * Revision 1.1  1992/01/24  03:28:27  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include "extern.h"

#if !defined(GNO)
#error "This version of dirbrk.c is for use with the Apple IIgs, only!"
#endif

/* allowable directory separators */
char*	DirBrkStr = "/:";

/* allowable start of full file/device/prefix names */
static char* PfxBrkStr = "/:0123456789.*@";

/*
** Return TRUE if the name is the full specification of a path name to a file
** starting at the root of the file system, otherwise return FALSE
**
** GS/OS Note:  The rules here aren't quite right for the above definition,
**              but they match the Unix' version functionality.  In other
**              words, if it starts like a GS/OS volume or device name would
**              start, then it returns TRUE.  Caveat:  It will return FALSE
**              if the name is one character long and isn't "*" or "@" or a
**              digit.
*/
PUBLIC int
If_root_path(char *name)
{
	char *p;

   p = strchr(PfxBrkStr, *name);
   if (p == NULL) return FALSE;

   if ((*p == '/') || (*p == ':') || (*p == '.')) {
	   if (*(p+1) == '\0') {
      	return FALSE;	/* "zero" length volume/prefix names not allowed */
      } else return TRUE;
   }

   return TRUE;
}
