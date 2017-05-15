#ifdef __CCFRONT__
#include <14:pragma.h>
#endif
/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/RCS/path.c,v 1.1 1992/01/24 03:27:54 dvadura Exp $
-- SYNOPSIS -- pathname manipulation code
-- 
-- DESCRIPTION
--	Pathname routines to handle building and pulling appart
--	pathnames.
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
--     $Log: path.c,v $
 * Revision 1.1  1992/01/24  03:27:54  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include "extern.h"

/*
** Return the suffix portion of a filename, assumed to begin with a `.'.
*/
PUBLIC char *
Get_suffix(char *name)
{
   char *suff;

   if(name == NIL(char)  || (suff = strrchr(name, '.')) == NIL(char))
      suff = ".NULL";

   return (suff);
}



/*
** Take dir and name, and return a path which has dir as the directory
** and name afterwards.
**
** N.B. Assumes that the dir separator string is in DirSepStr.
**      Return path is built in a static buffer, if you need to use it
**      again you must _strdup the result returned by Build_path.
*/
PUBLIC char *
Build_path(char *dir, char *name)
{
   register char *p;
   register char *q;
   static char     *path  = NIL(char);
   static unsigned int buflen = 0;
   int  plen = 0;
   int  dlen = 0;
   int  len;

   if( dir  != NIL(char) ) dlen = strlen( dir  );
   if( name != NIL(char) ) plen = strlen( name );
   len = plen+dlen+strlen(DirSepStr)+1;

   if( len > buflen ) {
      buflen = (len+16) & ~0xf;		/* buf is always multiple of 16 */

      if( path == NIL(char) )
         path = MALLOC( buflen, char );
      else
         path = realloc( path, (unsigned int) (buflen*sizeof(char)) );
   }
   
   *path = '\0';

   if( dlen ) {
      strcpy( path, dir );
      if( *path && strchr(DirBrkStr, dir[dlen-1]) == NIL(char) )
	 strcat( path, DirSepStr );
   }
   strcat( path, name );

   q=path;
   while( *q ) {
      char *t;

      p=_strpbrk(q,DirBrkStr);
      t=_strpbrk(p+1,DirBrkStr);
      if( !*p || !*t ) break;

      if(    !(p-q == 2 && strncmp(q,"..",2) == 0)
          && t-p-1 == 2 && strncmp(p+1,"..",2) == 0 ) {
	 t = _strspn(t,DirBrkStr);
	 strcpy(q,t);
      }
      else
	 q = p+1;
   }

   return( path );
}
