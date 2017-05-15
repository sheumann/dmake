/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/mac/arlib.c,v 1.1 1992/01/24 03:29:41 dvadura Exp $
-- SYNOPSIS -- Library access code.
-- 
-- DESCRIPTION
--  This implementation uses the library timestamp inplace of the
--  library member timestamp.
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
--     $Log: arlib.c,v $
 * Revision 1.1  1992/01/24  03:29:41  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include "extern.h"

PUBLIC time_t
seek_arch(char * /* name */, char *lib) {
   static   int warned = FALSE;

   if (!warned && !(Glob_attr&A_SILENT))
    warned = TRUE,
    Warning("Can't extract library member timestamp;\n\
    using library timestamp instead.");
   return (Do_stat(lib, NULL, NULL));
}

PUBLIC int
touch_arch(char * /* name */, char *lib) {
   static   int warned = FALSE;

   if (!warned && !(Glob_attr&A_SILENT))
    warned = TRUE,
    Warning("Can't update library member timestamp;\n\
    touching library instead.");
   return (Do_touch(lib, NULL, NULL));
}
