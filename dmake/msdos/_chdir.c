/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/msdos/_chdir.c,v 1.1 1992/01/24 03:27:24 dvadura Exp $
-- SYNOPSIS -- Change directory.
-- 
-- DESCRIPTION
--	Under DOS change the current drive as well as the current directory.
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
--     $Log: _chdir.c,v $
 * Revision 1.1  1992/01/24  03:27:24  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include <dos.h>
#include "extern.h"

PUBLIC int
_chdir(path)
char *path;
{
   int   res;

   res = chdir(path);

#if defined(OS2)
   if (res != -1 && path[1] == ':') {
      unsigned new_drive;
      unsigned max_drives;

      /* for OS2 we must change drive without using intdos() */
      new_drive = (*path & ~0x20) - 'A' + 1;
      _dos_setdrive(new_drive, &max_drives);
   }
#else
   if (res != -1 && path[1] == ':') {
      union REGS  reg;

      /* we must change the logged drive, since the chdir worked. */
      reg.h.ah = 0x0E;
      reg.h.dl = (*path & ~0x20) - 'A';
      intdos(&reg, &reg);
   }
#endif /* OS2 */
   return (res);
}

