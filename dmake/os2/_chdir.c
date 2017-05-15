/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/os2/_chdir.c,v 1.1 1992/01/24 03:29:24 dvadura Exp $
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
 * Revision 1.1  1992/01/24  03:29:24  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include <dos.h>
#include <os2.h>
#include "extern.h"

PUBLIC int
_chdir(path)
char *path;
{
   int res;

   res = chdir(path);

   if (res == 0 && path[1] == ':')
#ifdef OS2
      DosSelectDisk((*path & ~0x20) - '@');
#else
      unsigned new_drive;
      unsigned max_drives;

      new_drive = (*path & ~0x20) - 'A' + 1;
      _dos_setdrive(new_drive, &max_drives);
#endif

   return (res);
}
