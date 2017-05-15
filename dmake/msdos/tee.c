/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/msdos/tee.c,v 1.1 1992/01/24 03:27:37 dvadura Exp $
-- SYNOPSIS -- Hook_std_writes() dummy call for non swapping MSDOS versions.
-- 
-- DESCRIPTION
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
--     $Log: tee.c,v $
 * Revision 1.1  1992/01/24  03:27:37  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include "extern.h"

PUBLIC void
Hook_std_writes( file )
char *file;
{
}
